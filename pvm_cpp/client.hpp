#pragma once
#include <string>
#include "httplib.h"
#include <filesystem>
#include <vector>


class Client {
	private:
		const int id;
		int port;
		httplib::Client* cli;

		std::filesystem::path portpath();
		void set_port();

		template<typename Func>
		auto request(const std::string& method, const std::string& path, const std::string& errmsg, Func&& f) -> std::invoke_result_t<Func, httplib::Result&> ;

	public:
		Client();
		Client(int i);
		~Client();
		void connect();
		bool isalive();
		void kill();

		void load_plugin(const std::string& name, const std::string& path);
		void init_plugin(const std::string& plugin_name, const std::string& dataset_path);
		void run_plugin(const std::string& plugin_name);
		void finalize_plugin(const std::string& plugin_name);
		void unload_plugin(const std::string& name);

};

