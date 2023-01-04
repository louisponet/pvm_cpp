cc_library(
    name = "main",
    hdrs = glob([
        "include/**/*.h",
        "include/**/*.hpp",
		"include/*.h",
    ]),
	includes = ["include"],
	copts = ["-Iexternal/crow/include"],
    linkopts = ["-pthread"],
    visibility = ["//visibility:public"],
alwayslink=True,
)

