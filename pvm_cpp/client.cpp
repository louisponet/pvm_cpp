#include "pvm_cpp/client.hpp"
#include "pvm_cpp/utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "nlohmann/json.hpp"
#include "fmt/core.h"

Client::Client() : id(0), port(0), cli(nullptr){}
Client::Client(int i) : id(i), cli(nullptr) {}

Client::~Client() {
	if (cli)
		delete cli;
}

std::filesystem::path Client::portpath() {
	return utils::portpath(id);
}

void Client::connect(){
	if (isalive()) {
		std::cout << "Already connected" << std::endl;
		return;
	}

	auto pp = portpath();
	if (utils::ispath(pp)) {
		set_port();
		cli = new httplib::Client(fmt::format("http://0.0.0.0:{}", port));

		if (isalive()){
			std::cout << "Client " << id << " connected to port " << port << "\n";
			return;
		} else {
			delete cli;
			cli = nullptr;
			remove(pp.c_str());
		}
	}

	system(fmt::format("/home/ponet/Software/pvm_cpp/bazel-bin/pvm_cpp/main -c{} -p{}&", id,"/home/ponet/Software/pvm_cpp/bazel-bin/plugins/0/libplugin_0.so").c_str());

	int tries = 0;
	while (!utils::ispath(pp) && tries < 1000) {
		utils::thread_ms_sleep(50);
		tries++;
	}
	if (tries == 1000) 
		throw "Something went wrong starting server";
	
	set_port();
	cli = new httplib::Client("http://0.0.0.0:" + std::to_string(port));

	tries = 0;
	while (!isalive() && tries < 1000) {
		utils::thread_ms_sleep(50);
		tries++;
	}
	if (tries == 1000) 
		throw "Something went wrong starting server";
	
	std::cout << "Client " << id << " connected to port " << port << "\n";
}

template<typename Func>
auto Client::request(const std::string& method, const std::string& path, const std::string& errmsg, Func&& f) -> std::invoke_result_t<Func, httplib::Result&> {
	httplib::Request req;
	req.method = method;
	req.path = path;
	if (!cli)
		throw std::runtime_error("No connection");
	auto res = cli -> send(req); 
	if (res) {
		return f(res);
	} else {
		throw std::runtime_error(errmsg + ": " + httplib::to_string(res.error()));
	}
}

void Client::set_port() {
	auto pp = portpath();
	std::ifstream file(pp);
	std::stringstream ti;
	ti << file.rdbuf();
	port = std::stoi(ti.str()); 
}

bool Client::isalive() {
	if (!cli)
		return false;
	int retries = 0;
	while (retries < 2){
		if(auto res = cli -> Get("/alive"))
			return true;
		retries++;
	}
	return false;
}

void Client::kill() {
	request("POST", "/kill", "Error killing server", 
		[this](auto &res) {
			while (isalive())
				utils::thread_ms_sleep(50);
			delete cli;
			cli = nullptr;
		}
	);
}

int Client::load_data(const std::string& name, const std::string& path) {
	// TODO make this ispath on the server
	if (!utils::ispath(path))
		throw std::invalid_argument("Not a path");
	return request("POST", "/data/load/" + name + "/" + path, "Error loading data",
		[](auto &res){
			return std::stoi(res -> body);
		}
	);
}

std::vector<std::string> Client::list_data() {
	return request("GET", "/data/list", "Error listing data", 
		[](auto &res) {
			std::cout << res->body << std::endl;
			nlohmann::json data = nlohmann::json::parse(res -> body);

			return data["names"].get<std::vector<std::string>>();
		}
	);
}

void Client::unload_data(const std::string& name) {
	request("POST", "/data/unload/" + name, "Error unloading data", 
		[](auto &res){
		    if (res -> status != 200)
				throw std::invalid_argument("No such data exists");
		}
	);
}

void Client::load_plugin(const std::string& name, const std::string& path) {
	// TODO make this ispath on the server
	if (!utils::ispath(path))
		throw std::invalid_argument("Not a path");

	request("POST", fmt::format("/plugin/load/{}/{}", name, path), "Error loading plugin", 
		[](auto &res){
		    if (res -> status != 200)
				throw std::invalid_argument("Server error loading plugin");
		}
	);
}

void Client::init_plugin(const std::string& plugin_name, const std::string& dataset_name) {
	// TODO make this ispath on the server
	request("POST", fmt::format("/plugin/init/{}/{}", plugin_name, dataset_name), "Error initializing plugin", 
		[](auto &res){
		    if (res -> status != 200)
				throw std::invalid_argument("Server Error initializing plugin");
		}
	);
}






