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
	if (!handle) {
		CROW_LOG_WARNING << "Cannot open " << path;
		return;
	}
	const char *dlsym_error = dlerror();
	if (dlsym_error) {
		CROW_LOG_WARNING << "Cannot open "<< path << ": " << dlsym_error;
		return;
	}

	void *init_handle = dlsym(handle, "init");
	if (!init_handle){
		CROW_LOG_WARNING << "Cannot load function 'init' in " << path;
		return;
	}
	dlsym_error = dlerror();
	if (dlsym_error) {
		CROW_LOG_WARNING << "Cannot load function 'init' in " << path << ": " << dlsym_error;
		return;
	}

	init_ = reinterpret_cast<int (*)(const Dataset&)>(init_handle);

	void *run_handle = dlsym(handle, "run");
	if (!run_handle){
		CROW_LOG_WARNING << "Cannot load function 'run' in " << path;
		return;
	}
	dlsym_error = dlerror();
	if (dlsym_error) {
		CROW_LOG_WARNING << "Cannot load function 'run' in " << path << ": " << dlsym_error;
		return;
	}

	run_ = reinterpret_cast<int (*)(const Dataset&)>(run_handle);

	void *finalize_handle = dlsym(handle, "finalize");
	if (!finalize_handle){
		CROW_LOG_WARNING << "Cannot load function 'finalize' in " << path;
		return;
	}
	dlsym_error = dlerror();
	if (dlsym_error) {
		CROW_LOG_WARNING << "Cannot load function 'finalize' in " << path << ": " << dlsym_error;
		return;
	}

	finalize_ = reinterpret_cast<int (*)(const Dataset&)>(finalize_handle);
	loaded = true;
	//TODO handle nopath or other error exceptions
}
int Plugin::init(const Dataset& dataset) {
	return init_(dataset);
}

int Plugin::run(const Dataset& dataset) {
	return run_(dataset);
}

int Plugin::finalize(const Dataset& dataset) {
	return finalize_(dataset);
}
