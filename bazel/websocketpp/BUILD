cc_library(
    name = "main",
    hdrs = glob([
        "websocketpp/*.hpp",
		"websocketpp/*/*.hpp",
		"websocketpp/*/*/*.hpp",
		"websocketpp/*/*/*/*.hpp",
    ]),
	includes = [""],
	copts = ["-std=c++11"],
    # linkopts = ["-pthread", "-lboost_system"],
    visibility = ["//visibility:public"],
)
