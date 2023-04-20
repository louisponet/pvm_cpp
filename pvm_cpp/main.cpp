#include <iostream>
#include "pvm_cpp/brokers/alpaca.hpp"
#include "date/date.h"
#include <chrono>
#include <vector>

struct Test{
	int i;
	Test() : i(0){};
	Test(int i) : i(i){std::cout << " constructed\n";};
	Test(const Test& i) : i(i.i){std::cout << " copy constructed\n";};
	Test(Test&& o) : i(std::move(o.i)){std::cout << "move constructed\n";};
	Test& operator =(const Test& other){
		i = other.i;
		std::cout << " copy assinged\n";
		return *this;
	}
	Test& operator =(Test&& other){
		i = std::move(other.i);
		std::cout << " move assinged\n";
		return *this;
	}

};

int main(int argc, char** argv) {
    // std::string id = std::getenv("ALPACA_KEY_ID");
    // std::string secret = std::getenv("ALPACA_SECRET");
    // AlpacaBroker broker(id, secret);

    // std::cout << date::format("%D %T %Z", date::floor<std::chrono::milliseconds>(broker.get_bars("MSFT", "2023-04-05T00:00:00.000Z",
    //                 "2023-04-06T00:00:00.000Z", "1Min")[0].timestamp)) << std::endl;
    // std::cout << broker.get_bars("MSFT", "2023-04-05T00:00:00.000Z",
    //                 "2023-04-06T00:00:00.000Z", "1Min")[0].volume << std::endl;

	std::vector<Test> tests(5);

	for (int i = 0; i < 5; i++){
		tests.emplace(tests.begin()+i, Test{i});
	}

	for (int i = 0; i < 5; i++){
		tests[i] = Test(i);
	}

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
// 		c.add_bar("test_data", Bar{hrc::now(), 10, 203.0, 112.0, 123.9, 12.30});

// 		c.add_bar("test_data", Bar{hrc::now(), 10, 203.0, 112.0, 123.9, 12.30});
// 		c.add_bar("test_data", Bar{hrc::now(), 10, 203.0, 112.0, 123.9, 12.30});
// 		c.add_bar("test_data", Bar{hrc::now(), 10, 203.0, 112.0, 123.9, 12.30});
// 		c.add_bar("test_data", Bar{hrc::now(), 10, 203.0, 112.0, 123.9, 12.30});
// 		sleep(2);
// 		c.finalize_plugin("test_plugin", "test_data");
// 		c.kill();
// 		return 0;
// 	}
// }
