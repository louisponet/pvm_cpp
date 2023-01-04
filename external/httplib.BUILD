cc_library(
    name = "main",
    hdrs = glob([
        "cpp-httplib-0.11.3/httplib.h",
    ]),
	includes = ["cpp-httplib-0.11.3"],
	copts = ["-Iexternal/httplib/cpp-httplib-0.11.3"],
    linkopts = ["-pthread"],
    visibility = ["//visibility:public"],
alwayslink=True,
)
