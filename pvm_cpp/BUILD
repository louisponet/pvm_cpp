cc_library(
    name = "mainlib",
    hdrs= glob(["*.hpp"]),
	srcs = ["tick.cpp", "server.cpp", "client.cpp", "utils.cpp", "dataset.cpp", "plugin.cpp", "task.cpp"],
    copts = ["-std=c++20", "-O3", "-march=native", "-fPIC", "-fopenmp=libomp", "-DNDEBUG", "-fomit-frame-pointer"],
    linkopts = ["-lpthread", "-lbenchmark", "-lm","-ldl"],
    visibility = ["//visibility:public"],
	deps = ["@crow//:main",
            "@com_github_google_benchmark//:benchmark",
            "@nlohmann_json//:json",
            "@gtest//:gtest",
			"@entt//:entt",
			"@boost//:program_options",
			"@boost//:operators",
			"@boost//:container",
			"@httplib//:main",
			"@fmt//:main"]
)
# cc_binary(
#     name = "mainlib",
#     # hdrs= glob(["*.hpp"]),
# 	srcs = ["server.cpp", "client.cpp", "utils.cpp", "dataset.cpp", "plugin.cpp", ],
#     copts = ["-std=c++20", "-O0", "-march=native", "-fPIC"],
#     linkopts = ["-lpthread", "-lbenchmark", "-lm","-ldl"],
#     visibility = ["//visibility:public"],
# 	deps = [":mainlib_"],
# )

cc_binary(
        name = "main",
        srcs = ["main.cpp"],
        deps = [":mainlib"],
        copts = ["-std=c++20", "-O3", "-march=native", "-fPIC", "-fopenmp=libomp", "-DNDEBUG", "-fomit-frame-pointer"],
        linkopts = ["-lpthread", "-lbenchmark", "-lm","-ldl"],
		data = ["//:templates"],
)

## Replace workspace_name and dir_path as per your setup.
load("@com_grail_bazel_compdb//:defs.bzl", "compilation_database")
load("@com_grail_bazel_output_base_util//:defs.bzl", "OUTPUT_BASE")

compilation_database(
    name = "pvm_cpp_compdb",
    targets = ["//pvm_cpp:main","//pvm_cpp:mainlib"],
    # OUTPUT_BASE is a dynamic value that will vary for each user workspace.
    # If you would like your build outputs to be the same across users, then
    # skip supplying this value, and substitute the default constant value
    # "__OUTPUT_BASE__" through an external tool like `sed` or `jq` (see
    # below shell commands for usage).
    output_base = OUTPUT_BASE,
)

