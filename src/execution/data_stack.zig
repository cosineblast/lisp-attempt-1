const std = @import("std");
const c = @import("c");

export fn foo() i32 {
    c.free(c.la1_malloc(2048));
    return 42;
}
