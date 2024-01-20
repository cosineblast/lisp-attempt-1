const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = "lisp-attempt-1",
        .target = target,
        .optimize = optimize,
    });

    exe.addCSourceFiles(&.{
        "src/main.c", //
        "src/common/list.c",
        "src/parsing/lexer.c",
        "src/common/alloc.c",
        "src/parsing/parser.c",
        "src/parsing/parse_value.c",
        "src/execution/execution.c",
        "src/execution/value.c",
        "src/execution/binding.c",
        "src/execution/special_forms.c",
        "src/execution/realize.c",
        "src/common/die.c",
        "src/execution/builtin.c",
        "src/execution/gc.c",
    }, &.{ "-Werror", "-Wall" });

    exe.linkLibC();

    b.installArtifact(exe);

    const run_cmd = b.addRunArtifact(exe);

    run_cmd.step.dependOn(b.getInstallStep());

    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);

    const tests_activated = false;

    if (tests_activated) {
        const unit_tests = b.addTest(.{
            .root_source_file = .{ .path = "src/main.zig" },
            .target = target,
            .optimize = optimize,
        });

        const run_unit_tests = b.addRunArtifact(unit_tests);

        const test_step = b.step("test", "Run unit tests");
        test_step.dependOn(&run_unit_tests.step);
    }
}
