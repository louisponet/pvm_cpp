#include "pvm_cpp/dataset.hpp"
#include <string>
#include <fstream>
#include <unistd.h>
#include <thread>
#include <omp.h>
#include "pvm_cpp/spmc_queue.hpp"
#include <immintrin.h>

extern "C" {

	extern int init(const Dataset& dataset) {
		return 0;
	}

	bool should_stop = false;
	bool stopped = false;
	std::thread t;

	extern int run(Dataset& dataset) {
		// Why can't I capture the reader here ??
		t = std::thread([&dataset](){
			SPMCQueue<Tick, 1024>::Reader reader = dataset.q.get_reader(); 
			while (!should_stop) {
				auto tick = reader.read();
				std::cout << tick.high << std::endl;
			}
		});
		return 0;
	}
	extern int finalize(const Dataset& dataset) {
		should_stop = true;
		t.join();
		return 0;
	}
}

