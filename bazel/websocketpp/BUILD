cc_library(
    name = "main",
    hdrs = glob([
        "websocketpp-0.8.2/*.hpp",
    ]),
	includes = ["websocketpp-0.8.2"],
	copts = ["-Iexternal/websocketpp/websocketpp-0.8.2"],
    linkopts = ["-pthread"],
    visibility = ["//visibility:public"],
alwayslink=True,
)
