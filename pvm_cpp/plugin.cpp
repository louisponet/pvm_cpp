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

	init_ = reinterpret_cast<int (*)(void*)>(init_handle);

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

	run_ = reinterpret_cast<int (*)()>(run_handle);

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

	finalize_ = reinterpret_cast<int (*)()>(finalize_handle);
	loaded = true;
	//TODO handle nopath or other error exceptions
}
int Plugin::init(std::string path) {
	return init_(&path);
	//todo put the handles in the plugin
}

int Plugin::run() {
	return run_();
	//todo put the handles in the plugin
}

int Plugin::finalize() {
	return finalize_();
	//todo put the handles in the plugin
}
