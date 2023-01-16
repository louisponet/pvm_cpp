#include <boost/program_options.hpp>
#include "pvm_cpp/client.hpp"
#include "pvm_cpp/server.hpp"
#include "pvm_cpp/co_resource.hpp"
#include <fmt/format.h>

co_resource<std::string> greeting() {
	fmt::print("Enter");
	co_yield "Hello!";
	fmt::print("Exit");
}

int main(int argc, char** argv)
{
	namespace po = boost::program_options;
	po::options_description cmd_desc("Allowed options");
	cmd_desc.add_options()("help", "produce help message")("child,c", po::value<int>(),"Child instance")("path,p", po::value<std::string>(), "path");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, cmd_desc),vm);
	po::notify(vm);

	co_resource<std::string> r = greeting();

	std::cout << *r;



	return 0;



	// if (vm.count("child")) {
	// 	Server server(vm["child"].as<int>());
	// 	server.loop();
	// 	return 0;
	// } else {
	// 	Client c(1);
	// 	c.connect();
	// 	c.load_data("test_data", "/home/ponet/.julia/environments/TradingDEV/data.json");
	// 	std::cout << c.list_data()[0] << std::endl;
	// 	c.load_plugin("test_plugin", "/home/ponet/Software/pvm_cpp/bazel-bin/plugins/0/libplugin_0.so");
	// 	c.init_plugin("test_plugin", "test_data");
	// 	c.run_plugin("test_plugin", "test_data");
	// 	c.finalize_plugin("test_plugin", "test_data");
	// 	c.kill();
	// 	return 0;
	// }
}
