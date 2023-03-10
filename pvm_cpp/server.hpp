#pragma once
#include "crow.h"
#include "pvm_cpp/dataset.hpp"
#include "pvm_cpp/plugin.hpp"
#include <filesystem>
#include <map>

class Server {
	private:
		int id;
		crow::SimpleApp server;
		bool should_stop;
		std::unordered_map<std::string, Dataset> datasets;

		int port();
		std::filesystem::path portpath();
		std::filesystem::path storage_dir();

	public:
		std::unordered_map<std::string, Plugin> plugins;
		Server();
		Server(int i); 
		~Server();

		void loop();
};
