#include <iostream>
#include "pvm_cpp/brokers/alpaca.hpp"
#include "pvm_cpp/websocket.hpp"
#include "pvm_cpp/utils.hpp"
#include <date/date.h>
#include <chrono>
#include <vector>
#include <nlohmann/json.hpp>

int main(int argc, char** argv) {

	WebsocketEndpoint endpoint;
	int id = endpoint.connect("wss://paper-api.alpaca.markets/stream");
    std::string alpaca_id = std::getenv("ALPACA_KEY_ID");
    std::string alpaca_secret = std::getenv("ALPACA_SECRET");


	nlohmann::json body;
	body["action"] = "auth";
	body["key"] = alpaca_id;
	body["secret"] = alpaca_secret;

	ConnectionMetadata::WebsocketMetadataPointer connection = endpoint.get_metadata(id); 
	while (connection -> get_status() != "Open") {
		utils::thread_ms_sleep(100);
	}

	std::string message = body.dump();
	endpoint.send(id, message);	

	while (connection -> n_messages() == 1){ 
		utils::thread_ms_sleep(100);
	}

	int close_code = websocketpp::close::status::normal;

	endpoint.close(id, close_code);

	std::cout << *connection; 




    AlpacaBroker broker(alpaca_id, alpaca_secret);

    std::cout << date::format("%D %T %Z", date::floor<std::chrono::milliseconds>(broker.get_bars("MSFT", "2023-04-05T00:00:00.000Z",
                    "2023-04-06T00:00:00.000Z", "1Min")[0].timestamp)) << std::endl;
    std::cout << broker.get_bars("MSFT", "2023-04-05T00:00:00.000Z",
                    "2023-04-06T00:00:00.000Z", "1Min")[0].volume << std::endl;

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
