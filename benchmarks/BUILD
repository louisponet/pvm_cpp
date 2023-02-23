cc_binary(
        name = "spmc_queue",
        srcs = ["spmc_queue.cpp"],
		deps = ["//pvm_cpp:mainlib"],
        copts = ["-std=c++20", "-O3", "-march=native", "-fPIC", "-fopenmp=libomp", "-DNDEBUG", "-fomit-frame-pointer"],
        linkopts = ["-lpthread", "-lbenchmark", "-lm","-ldl"],
)
