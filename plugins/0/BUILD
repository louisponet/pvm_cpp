cc_binary(
    name = "plugin_0",
    srcs= glob(["plugin.cpp"]),
    copts = ["-std=c++17", "-O0", "-march=native", "-fPIC", "-shared"],
    visibility = ["//visibility:public"],
	deps = ["//pvm_cpp:mainlib"],
    linkshared=1,
    linkstatic=1
)
