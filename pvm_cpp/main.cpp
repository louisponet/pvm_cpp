#include <boost/program_options.hpp>
#include "pvm_cpp/client.hpp"
#include "pvm_cpp/server.hpp"
#include "pvm_cpp/co_resource.hpp"
#include "pvm_cpp/spmc_queue.hpp"
#include "pvm_cpp/Statistic.hpp"
#include <benchmark/benchmark.h>
#include <fmt/format.h>
#include <ctime>

co_resource<std::string> greeting() {
	fmt::print("Enter");
	co_yield "Hello!";
	fmt::print("Exit");
}

struct Msg
{
  uint64_t tsc;
  uint64_t i[1];
};

inline uint64_t rdtsc() {
  return __builtin_ia32_rdtsc();
}

bool cpupin(int cpuid) {
  cpu_set_t my_set;
  CPU_ZERO(&my_set);
  CPU_SET(cpuid, &my_set);
  if (sched_setaffinity(0, sizeof(cpu_set_t), &my_set)) {
    std::cout << "sched_setaffinity error: " << strerror(errno) << std::endl;
    return false;
  }

  return true;
}


const uint64_t MaxI = 1000000;

static SPMCQueue<Msg, 1024> q;

static void BM_read_write(benchmark::State& state){
	for (auto _ : state){
		
}




int main(int argc, char** argv)
{
	namespace po = boost::program_options;
	po::options_description cmd_desc("Allowed options");
	cmd_desc.add_options()("help", "produce help message")("child,c", po::value<int>(),"Child instance")("path,p", po::value<std::string>(), "path");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, cmd_desc),vm);
    po::notify(vm);
	SPMCQueue<Msg, 1024> q1;
	// q1.write(Tick{std::time(nullptr), 0, 23.0, 20.0, 124.0, 312.0});
	// q1.write(Tick{std::time(nullptr), 1, 13.0, 10.0, 14.0, 112.0});
	std::thread t1([&q1](){
		Statistic<uint64_t> stats;
		cpupin(1);
		auto r1 = q1.get_reader();
		while (true){
			auto tick = r1.read();
		    auto now = rdtsc();
			stats.add(now - tick.tsc);
			if (tick.i[0] == MaxI) break;
			// sleep(2);
		}
		stats.print(std::cout);
	});

	std::thread t2([&q1](){
		cpupin(2);
		for(uint64_t i = 1; i<= MaxI; i++){
			q1.write(Msg{rdtsc(), {i}});
		    auto expire = rdtsc() + 100;
		    while (rdtsc() < expire) continue;
		}
	});
	t1.join();
	t2.join();

	return 0;
//     // co_resource<std::string> r = greeting();

//     // std::cout << *r;

	// if (vm.count("child")) {
	// 	Server server(vm["child"].as<int>());
	// 	server.loop();
	// 	return 0;
	// } else {
	// 	Client c(1);
	// 	c.connect();
	// 	c.load_data("test_data", "/home/ponet/.julia/environments/TradingDEV/data.json");

	// 	c.load_plugin("test_plugin", "/home/ponet/Software/pvm_cpp/bazel-bin/plugins/0/libplugin_0.so");
	// 	c.init_plugin("test_plugin", "test_data");
	// 	c.run_plugin("test_plugin", "test_data");
	// 	c.add_tick("test_data", Tick{std::time(nullptr), 10, 203.0, 112.0, 123.9, 12.30}); 
	// 	sleep(2);
	// 	c.finalize_plugin("test_plugin", "test_data");
	// 	c.kill();
	// 	return 0;
	// }
}
