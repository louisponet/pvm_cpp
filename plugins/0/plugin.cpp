#include "pvm_cpp/dataset.hpp"
#include <string>
#include <fstream>
#include <unistd.h>
#include <thread>
#include <omp.h>
#include "pvm_cpp/spmc_queue.hpp"

//Try wether we can't put dataset in a library that this one links against
extern "C" {

	extern int init(const Dataset& dataset) {
		return 0;
	}

	static bool should_stop = false;

	extern int run(Dataset& dataset) {
		std::cout << "test" << std::endl;
		SPMCQueue<Tick, 1024>::Reader reader = dataset.q.get_reader(); 
		std::cout << "test2" << std::endl;
		std::thread t([&reader](){
			// while (!should_stop) {
				std::cout << "test3" << std::endl;
				auto tick = reader.read();
				std::cout << "test4" << std::endl;
				std::cout << tick.high << std::endl;
				std::cout << "test5" << std::endl;
			// }
		});
		t.join();
		return 0;
	}
	extern int finalize(const Dataset& dataset) {
		should_stop = true;
		return 0;
	}
}

