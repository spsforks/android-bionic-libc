load(":experimental_cc_shared_library.bzl", "cc_shared_library", "CcSharedLibraryInfo")

def full_cc_library(name, srcs, copts, linkopts, export_deps, transitive_export_deps,
                    user_link_flags, **kwargs):
    static_name = name + "_static"
    shared_name = name + "_shared"
    _cc_library_proxy(name = name,
                      static = static_name,
                      shared = shared_name)

    native.cc_library(
        name = static_name,
        srcs = srcs,
        copts = copts,
        linkopts = linkopts,
        deps = export_deps,
    )

    # Note this is a pretty bad hack we should fix upstream: It'd be great if a shared library
    # could declare a transitive exported static dep instead of needing to declare each target
    # transitively.
    static_deps = export_deps + transitive_export_deps

    cc_shared_library(
        name = shared_name,
        user_link_flags = user_link_flags,
        exports_filter = static_deps,
        static_deps = static_deps,
        roots = [static_name],
    )


def _cc_library_proxy_impl(ctx):
    static_files = ctx.attr.static[DefaultInfo].files.to_list()
    shared_files = ctx.attr.shared[DefaultInfo].files.to_list()

    return [
        ctx.attr.shared[CcSharedLibraryInfo],
        ctx.attr.static[CcInfo],
        DefaultInfo(files = depset(direct = static_files + shared_files))
    ]

_cc_library_proxy = rule(
    implementation = _cc_library_proxy_impl,
    attrs = {
        "shared": attr.label(mandatory = True, providers = [CcSharedLibraryInfo]),
        "static": attr.label(mandatory = True, providers = [CcInfo]),
    },
)
