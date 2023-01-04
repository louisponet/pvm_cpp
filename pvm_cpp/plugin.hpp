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
		int init(int datasize);
		int run(const Dataset &data);
		int finalize();
		void *handle;

	private:
		std::string path;

};


#endif // plugin_hpp_INCLUDED
