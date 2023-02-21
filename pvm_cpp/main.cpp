#include <boost/program_options.hpp>
#include "pvm_cpp/client.hpp"
#include "pvm_cpp/server.hpp"
#include "pvm_cpp/co_resource.hpp"
#include <fmt/format.h>

co_resource<std::string> greeting() {
	fmt::print("Enter");
	co_yield "Hello!";
	fmt::print("Exit");
}

// int main(int argc, char** argv)
// {
// 	namespace po = boost::program_options;
// 	po::options_description cmd_desc("Allowed options");
// 	cmd_desc.add_options()("help", "produce help message")("child,c", po::value<int>(),"Child instance")("path,p", po::value<std::string>(), "path");

//     po::variables_map vm;
//     po::store(po::parse_command_line(argc, argv, cmd_desc),vm);
//     po::notify(vm);

//     // co_resource<std::string> r = greeting();

//     // std::cout << *r;

// 	if (vm.count("child")) {
// 		Server server(vm["child"].as<int>());
// 		server.loop();
// 		return 0;
// 	} else {
// 		Client c(1);
// 		c.connect();
// 		c.load_data("test_data", "/home/ponet/.julia/environments/TradingDEV/data.json");

// 		std::cout << c.list_data()[0] << std::endl;
// 		auto tick = c.get_tick("test_data", 0);
// 		fmt::print("tick:\nvolume {}\nopen {}", tick.volume, tick.open);
// 		// c.load_plugin("test_plugin", "/home/ponet/Software/pvm_cpp/bazel-bin/plugins/0/libplugin_0.so");
// 		// c.init_plugin("test_plugin", "test_data");
// 		// c.run_plugin("test_plugin", "test_data");
// 		// c.finalize_plugin("test_plugin", "test_data");
// 		c.kill();
// 		return 0;
// 	}
// }

#include <bits/stdc++.h>
#include "pvm_cpp/spmc_queue.hpp"
#include "pvm_cpp/Statistic.hpp"
using namespace std;

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

using Q = SPMCQueue<Msg, 1024>;
Q q;

void read_thread(int tid, int cpu) {
  if (cpu >= 0) {
    cpupin(cpu);
  }
  auto reader = q.get_reader();
  uint64_t cnt = 0;
  Statistic<uint64_t> stats;
  stats.reserve(MaxI);
  while (true) {
    auto msg = reader.read();
    auto now = rdtsc();
    auto latency = now - msg.tsc;
    stats.add(latency);
    cnt++;
    assert(msg.i[0] >= cnt);
    for (auto cur : msg.i) assert(cur == msg.i[0]);
    if (msg.i[0] == MaxI) break;
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(tid * 100));
  cout << "tid: " << tid << ", drop cnt: " << (MaxI - cnt) << ", latency stats: " << endl;
  stats.print(cout);
  cout << endl;
}

// usage: ./multhread [reader cnt] [first cpu to bind]
int main(int argc, char** argv) {
  int reader_cnt = 3;
  int cpu_start = -1;
  if (argc >= 2) {
    reader_cnt = stoi(argv[1]);
  }
  if (argc >= 3) {
    cpu_start = stoi(argv[2]);
  }
  vector<thread> reader_thrs;
  for (int i = 0; i < reader_cnt; i++) {
    reader_thrs.emplace_back(read_thread, i, cpu_start < 0 ? -1 : cpu_start + i);
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(1));

  if (cpu_start >= 0) {
    cpupin(cpu_start + reader_cnt);
  }

  for (uint64_t i = 1; i <= MaxI; i++) {
    q.write(Msg{rdtsc(), {i}});
    // try set smaller waiting time to increase miss cnt, and set ZERO_COPY_READ = true to see if assert could fail
    auto expire = rdtsc() + 1000;
    while (rdtsc() < expire) continue;
  }

  for (auto& thr : reader_thrs) {
    thr.join();
  }
  return 0;
}
