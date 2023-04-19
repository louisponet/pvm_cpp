#ifndef plugin_hpp_INCLUDED
#define plugin_hpp_INCLUDED

#include <string>
#include "dataset.hpp"

class Plugin {
   public:
    Plugin();
    Plugin(const std::string& path);
    ~Plugin();

    bool loaded;
    void load();
    int init(const Dataset&);
    int run(const Dataset& dataset);
    int finalize(const Dataset&);

   private:
    std::string path;
    int (*init_)(const Dataset&);
    int (*run_)(const Dataset&);
    int (*finalize_)(const Dataset&);
    void* handle;
};

#endif  // plugin_hpp_INCLUDED
