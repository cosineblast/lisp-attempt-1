
const std = @import("std");
const c = @import("c");

const assert = std.debug.assert;
const Allocator = std.mem.Allocator;


const Value = c.Value;
const State = c.LA1_State;

const LinkedList = std.SinglyLinkedList(*Value);


// The main garbage collector of LAY.
const GC = struct {

    const THRESHOLD: usize = 100;

    const Self = @This();

    allocator: Allocator,
    spawned_values: LinkedList,
    spawned_value_count: u32, // boo, zig, boo
    values_since_last_gc: u32,
    enabled: bool,
    safe_stack: std.ArrayListUnmanaged(*Value),


    pub fn init(allocator: Allocator) !GC {
        const reserved_size: usize = 32;
        return GC {
            .allocator = allocator,
            .spawned_values = LinkedList { .first = null },
            .spawned_value_count = 0,
            .values_since_last_gc = 0,
            .enabled = false,
            .safe_stack = try std.ArrayListUnmanaged(*Value).initCapacity(allocator, reserved_size),
        };
    }


    pub fn spawn(self: *Self, state: *State, from_stack: *Value) !*Value {
        if (self.values_since_last_gc >= THRESHOLD) {
            self.performGC(state);
            self.values_since_last_gc = 1;
        }

        const result: *Value = try self.allocator.create(Value);
        result.* = from_stack.*;
        result.gc_tag = 0;

        self.values_since_last_gc += 1;
        self.spawned_value_count += 1;

        self.spawned_values.prepend(blk: {
            const node = try self.allocator.create(LinkedList.Node);
            node.data = result;
            break :blk node;
        });

        return result;
    }

    pub fn disable(self: *Self) void {
        self.enabled = false;
    }

    pub fn enable(self: *Self) void {
        self.enabled = true;
    }

    pub fn performGC(self: *Self, state: *State) void {
        std.debug.print("Performing GC\n", .{});

        assert(self == @as(*Self, @alignCast(@ptrCast(state.gc))));

        if (self.enabled) {
            mark(self, state);
            sweep(self);
        }
    }

    fn mark(self: *Self, state: *State) void {
        markValue(state.true_value);
        markValue(state.false_value);
        markValue(state.nil);

        markBindings(state.global_bindings);

        var current: ?*c.LinkedList = state.binding_stack.*.list;

        while (current) |current_| : (current = current_.*.next) {
            markBindings(@alignCast(@ptrCast(current_.*.item)));
        }

        for (self.safe_stack.items) |value| {
            markValue(value);
        }
    }

    fn markValue(value: *Value) void {
        // mark the value itself as safe from GC
        value.gc_tag = 1;

        // mark the references of the value as well
        if (value.type == c.LA1_VALUE_CONS) {
            const cell_: ?*c.ConsCell = value.content.cons;

            if (cell_) |cell| {
                markValue(cell.*.item);
                markValue(cell.*.next);
            }
        } else if (value.type == c.LA1_VALUE_CLOSURE) {
            const closure: *c.Closure = value.content.closure;

            if (closure.type == c.LA1_CLOSURE_DATA) {
                const data_closure: *c.DataClosure = @alignCast(@ptrCast(closure.extra));
                markValue(data_closure.body_source);

                var list: ?*c.LinkedList = @alignCast(@ptrCast(data_closure.environment));

                while (list) |list_| : (list = list_.next) {
                    markBindings(@alignCast(@ptrCast(list_.item)));
                }
            }
        }

        // TODO: make GC mark C function's void* extra through some kind of API
    }

    fn markBindings(bindings: *c.Bindings) void {
        for (0..bindings.size) |i| {
            markValue(bindings.content[i].value);
        }
    }

    fn sweep(self: *Self) void {
        var previous: ?*LinkedList.Node = null;
        var current_: ?*LinkedList.Node = self.spawned_values.first;
        var next: ?*LinkedList.Node = null;

        while (current_) |current| : (current_ = next) {
            const value: *Value = @alignCast(@ptrCast(current.*.data));
            next = current.next;

            // if value has been marked
            if (value.gc_tag != 0) {
                value.gc_tag = 0;

                previous = current;
            } else {
                // bye bye value
                self.freeValue(value);

                if (previous) |previous_| {
                    previous_.next = current.next;
                } else {
                    self.spawned_values.first = current.next;
                }

                self.allocator.destroy(current);

                self.spawned_value_count -= 1;

                // TODO: when 0.12.0 comes out, please, PLEASE
                // open an issue about the fact that the error anytype message
                // is B R O K E N when you try to allocator.free a goddam pointer
            }
        }
    }

    fn freeValue(self: *Self, value: *Value) void {
        if (value.type == c.LA1_VALUE_CONS) {
            const cell: ?*c.ConsCell = value.content.cons;

            c.free(cell);
        } else if (value.type == c.LA1_VALUE_CLOSURE) {
            const closure: *c.Closure = value.content.closure;

            if (closure.type == c.LA1_CLOSURE_DATA) {
                const data_closure: *c.DataClosure = @alignCast(@ptrCast(closure.extra));

                c.la1_bindings_decrement_ref(data_closure.environment);

                freeNodes(data_closure.parameters);

                c.free(data_closure);
            }

            c.free(closure);
        }

        self.allocator.destroy(value);
    }

    fn freeNodes(nodes: *c.LinkedList) void {
        var current: ?*c.LinkedList = nodes;

        while (current) |current_| {
            var next: ?*c.LinkedList = current_.next;

            c.free(current_);

            current = next;
        }
    }

    // TODO: move safe_stack from gc to state, as it is more
    // of a state specific thing.
    //
    pub fn safeStackPush(self: *Self, value: *Value) !*Value {
        try self.safe_stack.append(self.allocator, value);

        return value;
    }

    pub fn safeStackPop(self: *Self) void {
        _ = self.safe_stack.pop();
    }

    pub fn safeStackPopN(self: *Self, n: u32) void {
        for (0..n) |_| {
            self.safeStackPop();
        }
    }

    /// This operation deletes all values that have been
    /// spawned by the GC, effectivelly freeing all *Values,
    /// EVEN THOSE THAT ARE STILL REACHABLE.
    pub fn deinit(self: *Self) void {
        // TODO: move this assert to somewhere else,
        // as this is not a realy expectation of this function
        assert(self.safe_stack.items.len == 0);

        var node_ = self.spawned_values.first;

        while (node_) |node| {
            const next = node.next;

            self.freeValue(node.data);

            self.allocator.destroy(node);

            node_ = next;
        }

        self.safe_stack.deinit(self.allocator);
    }
};

// TODO: instead, use an allocator that aborts when malloc fails

export fn la1_gc_create() *anyopaque {
    const default_allocator = std.heap.c_allocator;

    const result = default_allocator.create(GC) catch unreachable;
    result.* = GC.init(default_allocator) catch unreachable;

    return result;
}

export fn la1_gc_destroy(gc: *GC) void {
    const default_allocator = std.heap.c_allocator;

    gc.deinit();

    default_allocator.destroy(gc);
}

export fn la1_gc_value_count(gc: *GC) u32 {
    return gc.spawned_value_count;
}

export fn la1_gc_disable(gc: *GC) void {
    gc.disable();
}

export fn la1_gc_enable(gc: *GC) void {
    gc.enable();
}

export fn la1_perform_gc(state: *State) void {
    const gc: *GC = @alignCast(@ptrCast(state.gc));

    gc.performGC(state);
}


export fn la1_safe_stack_push(state: *State, value: *Value) *Value {
    const gc: *GC = @alignCast(@ptrCast(state.gc));

    return gc.safeStackPush(value) catch unreachable;
}

export fn la1_safe_stack_pop(state: *State) void {
    const gc: *GC = @alignCast(@ptrCast(state.gc));

    gc.safeStackPop();
}

export fn la1_safe_stack_pop_n(state: *State, n: u32) void {
    const gc: *GC = @alignCast(@ptrCast(state.gc));

    gc.safeStackPopN(n);
}

export fn la1_gc_spawn(state: *State, from_stack: *Value) *Value {
    const gc: *GC = @alignCast(@ptrCast(state.gc));

    return gc.spawn(state, from_stack) catch unreachable;
}

