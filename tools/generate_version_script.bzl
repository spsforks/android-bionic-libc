# Rule that generates the version script for a library.

def _generate_version_script_impl(ctx):
    ctx.actions.run(
        inputs = [ctx.file.master],
        outputs = [ctx.outputs.out],
        executable = ctx.executable._script,
        arguments = [
            ctx.attr.arch,
            ctx.file.master.path,
            ctx.outputs.out.path,
        ],
        progress_message = "Generating version script for %s from %s" % (ctx.attr.arch, ctx.file.master.short_path),
    )

generate_version_script = rule(
    implementation = _generate_version_script_impl,
    attrs = {
        "master": attr.label(allow_single_file = True, mandatory = True),
        "arch": attr.string(mandatory = True),
        "out": attr.output(mandatory = True),
        "_script": attr.label(
            executable = True,
            cfg = "exec",
            default = "//bionic/tools:generate_version_script_tool",
        ),
    },
)
