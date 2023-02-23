cc_binary(
    name = "plugin_2",
    srcs= glob(["plugin.cpp"]),
    copts = ["-std=c++20", "-O3", "-march=native", "-fPIC", "-shared", "-fopenmp", "-fomit-frame-pointer"],
	linkopts = ["-fopenmp","-lpthread", "-lbenchmark", "-lm","-ldl"],
    visibility = ["//visibility:public"],
	deps = ["//pvm_cpp:mainlib"],
    linkshared=1,
    linkstatic=1
)
