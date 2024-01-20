const std = @import("std");
const c = @import("c");

const data_stack_size: usize = 1024 * 1024;

const Allocator = std.mem.Allocator;
const default_allocator = std.heap.raw_c_allocator;

const DataStack = struct {
    const Self = @This();

    data: []*c.Value,
    index: usize,

    fn init(allocator: Allocator) !DataStack {
        return DataStack{ .data = try allocator.alloc(*c.Value, data_stack_size), .index = 0 };
    }

    fn push(self: *Self, value: *c.Value) !void {
        if (self.index >= self.data.len) {
            return error.DataStackOverflow;
        }

        self.index += 1;

        self.data[self.index] = value;
    }

    fn pop(self: *Self) !*c.Value {
        if (self.index == 0) {
            return error.DataStackUnderflow;
        }

        const result = self.data[self.index];

        self.index -= 1;

        return result;
    }

    fn deinit(self: *Self, allocator: Allocator) void {
        allocator.free(self.data);
    }
};

export fn la1_data_stack_create() ?*DataStack {
    const result = default_allocator.create(DataStack) catch unreachable;
    result.* = DataStack.init(default_allocator) catch unreachable;
    return result;
}

export fn la1_data_stack_push(stack: *DataStack, value: *c.Value) void {
    stack.push(value) catch {
        c.la1_die("Data Stack Overflow!!!\n");
        unreachable;
    };
}

export fn la1_data_stack_pop(stack: *DataStack) *c.Value {
    return stack.pop() catch {
        c.la1_die("Data Stack Underflow!!!\n");
        unreachable;
    };
}

export fn la1_data_stack_free(stack: *DataStack) void {
    stack.deinit(default_allocator);
    default_allocator.destroy(stack);
}
