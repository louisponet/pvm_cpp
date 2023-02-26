cc_test(
	name = "spmc_queue",
	size = "small",
    srcs = ["spmc_queue.cpp"],
	deps = ["@com_google_googletest//:gtest_main",
	        "//pvm_cpp:mainlib"],
	linkopts = ["-lm", "-pthread"],
    copts = ["-std=c++20", "-march=native", "-fPIC"],
)
