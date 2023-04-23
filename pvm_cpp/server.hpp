#pragma once
#include <boost/filesystem.hpp>
#include <map>
#include "crow.h"
#include "pvm_cpp/dataset.hpp"
#include "pvm_cpp/plugin.hpp"

class Server {
   private:
    int id;
    crow::SimpleApp server;
    bool should_stop;
    std::unordered_map<std::string, Dataset> datasets;

    int port();
    boost::filesystem::path portpath();
    boost::filesystem::path storage_dir();

   public:
    std::unordered_map<std::string, Plugin> plugins;
    Server();
    Server(int i);
    ~Server();

    void loop();
};
