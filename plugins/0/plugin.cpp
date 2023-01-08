#include "pvm_cpp/dataset.hpp"
#include <string>

//Try wether we can't put dataset in a library that this one links against
extern "C" {
	extern int init(const Dataset& dataset) {
		return 0;
	}

	extern int run(const Dataset& dataset) {
		return 0;
	}
	extern int finalize(const Dataset& dataset) {
		return 0;
	}
}

