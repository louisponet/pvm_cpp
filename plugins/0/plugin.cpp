#include "pvm_cpp/dataset.hpp"
#include <string>

//Try wether we can't put dataset in a library that this one links against
Dataset *DATA;

extern "C" {
	extern int init(void *path) {
		DATA = new Dataset(*(std::string*) path);
		return 0;
	}

	extern int run() {
		return DATA->size();
	}
	extern int finalize() {
		delete DATA;
		return 0;
	}
}

