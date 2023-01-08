#include "pvm_cpp/dataset.hpp"
#include <string>
#include <fstream>
#include <unistd.h>
#include <thread>
#include <omp.h>
//Try wether we can't put dataset in a library that this one links against
extern "C" {
	extern int init(const Dataset& dataset) {
		return 0;
	}

	extern int run(const Dataset& dataset) {
		int test = 0;
		#pragma omp parallel for num_threads(4)
		for (auto const& t: dataset.ticks){
			test += t.volume;
		}
		std::ofstream ofile("out.txt");
		ofile << test << std::endl;
		
		return 0;
	}
	extern int finalize(const Dataset& dataset) {
		return 0;
	}
}

