cc_library(
    name = "mainlib",
    hdrs= glob(["*.hpp", "brokers/*.hpp"]),
	srcs = ["bar.cpp","client.cpp", "dataset.cpp", "plugin.cpp", "server.cpp", "task.cpp", "utils.cpp", "websocket.cpp","brokers/alpaca.cpp"],
    visibility = ["//visibility:public"],
	deps = ["@crow//:main",
            "@nlohmann_json//:json",
			"@entt//:entt",
			"@boost//:program_options",
			"@boost//:dll",
			"@boost//:operators",
			"@boost//:container",
			"@boost//:asio_ssl",
			"@boost//:system",
			"@boost//:filesystem",
			"@httplib//:main",
			"@fmt//:main",
            "@date//:main",
			"@websocketpp//:main"],
	defines = [
	        "CPPHTTPLIB_OPENSSL_SUPPORT",
			
	    ],
	copts = ["-std=c++20"],
    linkopts = ["-ldl"]
)

cc_binary(
        name = "main",
        srcs = ["main.cpp"],
        deps = [":mainlib"],
		data = ["//:templates"],
)

