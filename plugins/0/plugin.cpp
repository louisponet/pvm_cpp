#include "pvm_cpp/dataset.hpp"
#include "pvm_cpp/chrono.hpp"
#include <thread>
#include <immintrin.h>
#include <iomanip>

extern "C" {
	using hrc = std::chrono::high_resolution_clock;

	extern int init(const Dataset& dataset) {
		return 0;
	}

	bool should_stop = false;
	bool stopped = false;
	std::thread t;

	extern int run(Dataset& dataset) {
		// Why can't I capture the reader here ??
		t = std::thread([&dataset](){
			SPMCQueue<Bar, 1024>::Reader reader = dataset.q.get_reader(); 
			Bar curbar;
			while (!should_stop) {
				if(reader.read(curbar)){
					auto n = hrc::now();
					std::cout << "plugin 0: " << (n - curbar.timestamp)/1ns << std::endl;
				}
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

