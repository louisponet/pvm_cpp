#include "pvm_cpp/brokers/alpaca.hpp"
#include <iostream>
int main(int argc, char** argv)
{
	std::string id = std::getenv("ALPACA_KEY_ID");
	std::string secret = std::getenv("ALPACA_SECRET");
	AlpacaBroker broker(id, secret);
	return 0;
}
// #include <boost/program_options.hpp>
// #include "pvm_cpp/client.hpp"
// #include "pvm_cpp/server.hpp"
// // #include "pvm_cpp/co_resource.hpp"
// #include "pvm_cpp/chrono.hpp"
// #include "fmt/format.h"
// #include <ctime>
// int main(int argc, char** argv)
// {
// 	namespace po = boost::program_options;
// 	po::options_description cmd_desc("Allowed options");
// 	cmd_desc.add_options()("help", "produce help message")("child,c", po::value<int>(),"Child instance")("path,p", po::value<std::string>(), "path");

//     po::variables_map vm;
//     po::store(po::parse_command_line(argc, argv, cmd_desc),vm);
//     po::notify(vm);

// 	if (vm.count("child")) {
// 		Server server(vm["child"].as<int>());
// 		server.loop();
// 		return 0;
// 	} else {
// 		Client c(1);
// 		c.connect();
// 		c.load_data("test_data");

// 		c.load_plugin("test_plugin", "/home/lponet/Software/pvm_cpp/bazel-bin/plugins/0/libplugin_0.so");
// 		c.init_plugin("test_plugin", "test_data");
// 		c.run_plugin("test_plugin", "test_data");
// 		c.add_tick("test_data", Tick{hrc::now(), 10, 203.0, 112.0, 123.9, 12.30}); 

// 		c.add_tick("test_data", Tick{hrc::now(), 10, 203.0, 112.0, 123.9, 12.30}); 
// 		c.add_tick("test_data", Tick{hrc::now(), 10, 203.0, 112.0, 123.9, 12.30}); 
// 		c.add_tick("test_data", Tick{hrc::now(), 10, 203.0, 112.0, 123.9, 12.30}); 
// 		c.add_tick("test_data", Tick{hrc::now(), 10, 203.0, 112.0, 123.9, 12.30}); 
// 		sleep(2);
// 		c.finalize_plugin("test_plugin", "test_data");
// 		c.kill();
// 		return 0;
// 	}
// }
