cc_library(
    name = "mainlib",
    hdrs= glob(["*.hpp", "brokers/*.hpp"]),
	srcs = ["bar.cpp", "server.cpp", "client.cpp", "utils.cpp", "dataset.cpp", "plugin.cpp", "task.cpp", "brokers/alpaca.cpp"],
    visibility = ["//visibility:public"],
	deps = ["@crow//:main",
            "@nlohmann_json//:json",
			"@entt//:entt",
			"@boost//:program_options",
			"@boost//:dll",
			"@boost//:operators",
			"@boost//:container",
			"@httplib//:main",
			"@fmt//:main",
            "@date//:main"],
	defines = [
	        "CPPHTTPLIB_OPENSSL_SUPPORT",
	    ]
)

cc_binary(
        name = "main",
        srcs = ["main.cpp"],
        deps = [":mainlib"],
		data = ["//:templates"],
)

