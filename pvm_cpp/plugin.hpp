#ifndef plugin_hpp_INCLUDED
#define plugin_hpp_INCLUDED

#include <string>
#include "dataset.hpp"

class Plugin {
	public:
		Plugin();
		Plugin(const std::string &path);
		~Plugin();

		bool loaded;
		void load();
		int init(std::string dataset_path);
		int run();
		int finalize();

	private:
		std::string path;
		int (*init_)(void*);
		int (*run_)();
		int (*finalize_)();
		void *handle;

};


#endif // plugin_hpp_INCLUDED
