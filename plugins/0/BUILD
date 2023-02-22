cc_binary(
    name = "plugin_0",
    srcs= glob(["plugin.cpp"]),
    copts = ["-std=c++20", "-O0", "-march=native", "-fPIC", "-shared", "-fopenmp"],
	linkopts = ["-fopenmp","-lpthread", "-lbenchmark", "-lm","-ldl"],
    visibility = ["//visibility:public"],
	deps = ["//pvm_cpp:mainlib"],
    linkshared=1,
    linkstatic=1
)
