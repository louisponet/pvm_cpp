#pragma once
#include <boost/filesystem.hpp>
#include <string>
#include <vector>
#include "httplib.h"
#include "pvm_cpp/bar.hpp"

class Client {
   private:
    const int id;
    int port;
    httplib::Client* cli;

    boost::filesystem::path portpath();
    void set_port();

    template <typename Func>
    auto request(const std::string& method, const std::string& path,
                 const std::string& errmsg, Func&& f)
        -> std::invoke_result_t<Func, httplib::Result&>;
    template <typename Data, typename Func>
    auto request(const std::string& method, const std::string& path,
                 const Data& data, const std::string& errmsg, Func&& f)
        -> std::invoke_result_t<Func, httplib::Result&>;

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
    void add_bar(const std::string& name, const Bar& bar);
    Bar get_bar(const std::string& name, int id);
    std::vector<std::string> list_data();
    void load_plugin(const std::string& name, const std::string& path);
    void init_plugin(const std::string& plugin_name,
                     const std::string& dataset_path);
    void run_plugin(const std::string& plugin_name,
                    const std::string& dataset_name);
    void finalize_plugin(const std::string& plugin_name,
                         const std::string& dataset_name);
    void unload_plugin(const std::string& name);
};
