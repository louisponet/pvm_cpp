#include "pvm_cpp/bar.hpp"
#include "benchmark/benchmark.h"
#include <vector>
#include "pvm_cpp/chrono.hpp"


static void BM_vector_filling_by_id(benchmark::State& state) {
	time_point t = hrc::now();
	for (auto _: state){
		std::vector<Bar> bars(100);
		for (int i = 0; i < 100; i++){
			bars[i] = Bar{t, (float)i, (float)i, (float)i, (float)i, i};
		}
	}
}

static void BM_vector_filling_by_emplace_back(benchmark::State& state) {
	time_point t = hrc::now();
	for (auto _: state){
		std::vector<Bar> bars;
		bars.reserve(100);
		for (int i = 0; i < 100; i++){
			bars.emplace_back(t, (float)i, (float)i, (float)i, (float)i, i);
		}
	}
}
static void BM_vector_filling_by_emplace(benchmark::State& state) {
	time_point t = hrc::now();
	for (auto _: state){
		std::vector<Bar> bars(100);
		auto start = bars.begin();
		for (int i = 0; i < 100; i++){
			bars.emplace(start+i, t, (float)i, (float)i, (float)i, (float)i, i);
		}
	}
}

BENCHMARK(BM_vector_filling_by_id);
BENCHMARK(BM_vector_filling_by_emplace_back);
BENCHMARK(BM_vector_filling_by_emplace);
BENCHMARK_MAIN();
