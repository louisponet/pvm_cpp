cc_binary(
    name = "plugin_0",
    srcs= glob(["plugin.cpp"]),
    copts = ["-fPIC"],
    visibility = ["//visibility:public"],
	deps = ["//pvm_cpp:mainlib"],
    linkshared=1,
    linkstatic=1
)
