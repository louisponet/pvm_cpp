#define CROW_ENABLE_DEBUG
#include "pvm_cpp/utils.hpp"
#include "pvm_cpp/server.hpp"
#include "pvm_cpp/plugin.hpp"
#include "pvm_cpp/tick.hpp"
#include "pvm_cpp/chrono.hpp"
#include <cstdio>
#include <string>
#include <thread>
#include "nlohmann/json.hpp"
#include "fmt/core.h"

using json = nlohmann::json;

Server::Server() : id(0), should_stop(false) {
	if (!utils::ispath("servers"))
		std::filesystem::create_directory(std::filesystem::path("servers"));
}

Server::Server(int i) : id(i), should_stop(false){
	if (!utils::ispath("servers"))
		std::filesystem::create_directory(std::filesystem::path("servers"));

    CROW_ROUTE(server, "/alive")([](){ // 
        return true; //
    });
    CROW_ROUTE(server, "/kill").methods(crow::HTTPMethod::POST)([this](){ // 
        should_stop = true; //
        return true;
    });
    CROW_ROUTE(server, "/data/load/<string>/<path>").methods(crow::HTTPMethod::POST)
    ([this](std::string name, std::string path){
	    datasets.insert(std::make_pair(name, Dataset(path)));
	    return 200;
    });
    CROW_ROUTE(server, "/data/add/<string>/").methods(crow::HTTPMethod::POST)
    ([this](const crow::request& req, std::string name){
	    nlohmann::json dat = nlohmann::json::parse(req.body);
	    auto t = dat.get<Tick>();
		datasets[name].add(t);	
	    return 200;
    });
    CROW_ROUTE(server, "/data/read/<string>/<int>").methods(crow::HTTPMethod::GET)
    ([this](std::string name, int id){
	    if (id >= datasets[name].size()) 
		    return crow::response(500);
		json j = datasets[name].ticks[id]; 
	    return crow::response(200, j.dump());
    });
    CROW_ROUTE(server, "/data/list")
    ([this](){
	    std::vector<std::string> out;
		for (auto const& kv: datasets){
			out.push_back(kv.first);
		}
		json to_ret;
		to_ret["names"] = out;
	    return to_ret.dump();
    });

    CROW_ROUTE(server, "/plugin/load/<string>/<path>").methods(crow::HTTPMethod::POST)
    ([this](std::string name, std::string path){
	    plugins[name] = Plugin(path);
	    int tries = 0;
	    while (!plugins[name].loaded || tries == 1000) {
			utils::thread_ms_sleep(50);
			tries++;
	    }
	    if (tries == 1000) {
		    plugins.erase(name);
			CROW_LOG_WARNING << "Cannot load plugin: "<< name;
		    return 500;
	    } else
		    return 200;
    });

    CROW_ROUTE(server, "/plugin/unload/<string>").methods(crow::HTTPMethod::POST)
    ([this](std::string name){
		if (plugins.find(name) == plugins.end())
			return 500;
		plugins.erase(name);
		return 200;
    });

    CROW_ROUTE(server, "/plugin/init/<string>/<string>").methods(crow::HTTPMethod::POST)
    ([this](std::string plugin_name, std::string dataset_name){
		if (plugins.find(plugin_name) == plugins.end() ||datasets.find(dataset_name) == datasets.end() )
			return 500;
		if (plugins[plugin_name].init(datasets[dataset_name])){
			return 500;
		} else {
			return 200;
		}
    });
    CROW_ROUTE(server, "/plugin/run/<string>/<string>").methods(crow::HTTPMethod::POST)
    ([this](std::string plugin_name, std::string dataset_name){
		if (plugins.find(plugin_name) == plugins.end() ||datasets.find(dataset_name) == datasets.end() )
			return 500;
		auto ds = datasets[dataset_name];
		if (plugins[plugin_name].run(datasets[dataset_name])){
			return 500;
		} else {
			return 200;
		}
    });
    CROW_ROUTE(server, "/plugin/finalize/<string>/<string>").methods(crow::HTTPMethod::POST)
    ([this](std::string plugin_name, std::string dataset_name){
		if (plugins.find(plugin_name) == plugins.end() ||datasets.find(dataset_name) == datasets.end() )
			return 500;

		if (plugins[plugin_name].finalize(datasets[dataset_name])){
			return 500;
		} else {
			return 200;
		}
    });

}
Server::~Server(){
	if (utils::ispath(portpath()))
		remove(portpath().c_str());
	server.stop();
}

void Server::loop() {
	auto dir = storage_dir();
	if (!std::filesystem::exists(dir)) 
		std::filesystem::create_directory(dir);

	auto pp = portpath(); 
	if (utils::ispath(pp)) 
		remove(pp.c_str());

	server.port(0);
	// server.loglevel(crow::LogLevel::Debug);
	server.concurrency(2);
	auto server_proc = server.run_async();

	while (server.port() == 0) {
		utils::thread_ms_sleep(50);
	}

	std::ofstream portfile(pp);
	portfile << port();
	portfile.close();

	while (!should_stop) {
		// Very ugly but apparently dlopen needs to happen on the main thread...
		for (auto &kv : plugins) {
			if (!kv.second.loaded) {
				CROW_LOG_INFO << "loading: "<< kv.first;
				kv.second.load();
			}
		}
		utils::thread_ms_sleep(50);
	}
	server.stop();
}


int Server::port() {
	return server.port();
}
std::filesystem::path Server::portpath() {
	return utils::portpath(id);
}

std::filesystem::path Server::storage_dir() {
	return std::filesystem::path(fmt::format("servers/{}", id));
}
