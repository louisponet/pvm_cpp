#pragma once
#include <string>
#include "httplib.h"
#include <filesystem>
#include <vector>
#include "pvm_cpp/tick.hpp"

class Client {
	private:
		const int id;
		int port;
		httplib::Client* cli;

		std::filesystem::path portpath();
		void set_port();

		template<typename Func>
		auto request(const std::string& method, const std::string& path, const std::string& errmsg, Func&& f) -> std::invoke_result_t<Func, httplib::Result&> ;
		template<typename Data, typename Func>
		auto request(const std::string& method, const std::string& path, const Data& data, const std::string& errmsg, Func&& f) -> std::invoke_result_t<Func, httplib::Result&>;

	public:
		Client();
		Client(int i);
		~Client();
		void connect();
		bool isalive();
		void kill();

		void load_data(const std::string& name, const std::string& path);
		void load_data(const std::string& name);
		void unload_data(const std::string& name);
		void add_tick(const std::string& name, const Tick& tick);
		Tick get_tick(const std::string& name, int id);
		std::vector<std::string>  list_data();
		void load_plugin(const std::string& name, const std::string& path);
		void init_plugin(const std::string& plugin_name, const std::string& dataset_path);
		void run_plugin(const std::string& plugin_name, const std::string& dataset_name);
		void finalize_plugin(const std::string& plugin_name, const std::string& dataset_name);
		void unload_plugin(const std::string& name);

};

