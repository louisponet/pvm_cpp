#include "crow.h"
#include "entt/entt.hpp"
#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>
#include <boost/format.hpp>
#include "pvm_cpp/client.hpp"
#include "pvm_cpp/server.hpp"
#include "pvm_cpp/utils.hpp"
#include "pvm_cpp/dataset.hpp"
#include <vector>
#include <cstdio>
#include <filesystem>
#include <dlfcn.h>


int main(int argc, char** argv)
{
	namespace po = boost::program_options;
	po::options_description cmd_desc("Allowed options");
	cmd_desc.add_options()("help", "produce help message")("child,c", po::value<int>(),"Child instance")("path,p", po::value<std::string>(), "path");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, cmd_desc),vm);
	po::notify(vm);
	if (!utils::ispath("servers"))
		std::filesystem::create_directory(std::filesystem::path("servers"));

	if (vm.count("child")) {
		Server server(vm["child"].as<int>());
		// Plugin p(vm["path"].as<std::string>());
		// server.plugins["test"] = p;
		server.loop();
		return 0;
	} else {
		Client c(1);
		c.connect();
		c.load_data("test", "/home/ponet/.julia/environments/TradingDEV/data.json");
		c.load_plugin("test", "/home/ponet/Software/pvm_cpp/bazel-bin/plugins/0/libplugin_0.so");
		c.init_plugin("test", "test");
		c.kill();
		return 0;
	}
}
