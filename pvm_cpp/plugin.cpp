#include "plugin.hpp"
#include "crow.h"
#include <dlfcn.h>
#include <iostream>

Plugin::Plugin() {};

Plugin::Plugin(const std::string &path) : loaded(false), path(path){
}

Plugin::~Plugin(){
	if (loaded)
		dlclose(handle);
}

void Plugin::load(){
	handle = dlopen(path.c_str(), RTLD_LAZY);
	const char *dlsym_error = dlerror();
	if (dlsym_error) {
		CROW_LOG_WARNING << "Cannot open: "<< dlsym_error;
	} else {
		loaded = true;
	}
	//TODO handle nopath or other error exceptions
}
int Plugin::init(int datasize) {
	//todo put the handles in the plugin
	void *fhandle = dlsym(handle, "init");
	if (!fhandle){
		CROW_LOG_WARNING << "Cannot load symbol 'init'";
		return 1;
	}

	int (*i)(int) = reinterpret_cast<int (*)(int)>(fhandle);
	const char *dlsym_error = dlerror();
	if (dlsym_error) {
		CROW_LOG_WARNING << "Cannot load symbol 'init': " << dlsym_error;
		return 1;
	}
	return (*i)(datasize);
}


