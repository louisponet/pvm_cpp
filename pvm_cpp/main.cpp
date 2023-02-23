#include <boost/program_options.hpp>
#include "pvm_cpp/client.hpp"
#include "pvm_cpp/server.hpp"
#include "pvm_cpp/co_resource.hpp"
#include "pvm_cpp/spmc_queue.hpp"
#include "pvm_cpp/Statistic.hpp"
#include <benchmark/benchmark.h>
#include <fmt/format.h>
#include <ctime>
#include <x86intrin.h>

#include <chrono>
static inline uint64_t rdtsc()
{
    uint64_t rax, rdx;
    __asm__ __volatile__("rdtsc" : "=a"(rax), "=d"(rdx));
    return (rdx << 32) + rax;
}
// inline uint64_t rdtsc() {
//   return __rdtsc();
// }

static inline uint64_t rdtscp()
{
    uint64_t rax, rcx, rdx;
    __asm__ __volatile__("rdtscp" : "=a"(rax), "=d"(rdx), "=c"(rcx));
    return (rdx << 32) + rax;
}

static inline void cpuid()
{
    uint64_t rax, rbx, rcx, rdx;
    __asm__ __volatile__("cpuid" : "=a"(rax), "=b"(rbx), "=d"(rdx), "=c"(rcx));
}

static inline uint64_t rdtscp(int& chip, int& core)
{
    uint64_t rax, rcx, rdx;
    __asm__ __volatile__("rdtscp" : "=a"(rax), "=d"(rdx), "=c"(rcx));
    chip = (rcx & 0xFFF000) >> 12;
    core = rcx & 0xFFF;
    return (rdx << 32) + rax;
}

double getghz(){
  using Clock = std::conditional_t<std::chrono::high_resolution_clock::is_steady,
                                     std::chrono::high_resolution_clock,
                                     std::chrono::steady_clock>;

    int chip, core, chip2, core2;

    auto start = Clock::now();
	cpuid();
    uint64_t rdtsc_start = rdtscp(chip, core);

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    uint64_t rdtsc_end = rdtscp(chip2, core2);
    cpuid();

    auto end = Clock::now();

    // if (core != core2 || chip != chip2)
    //     throw std::runtime_error("please set this executable to a specific CPU core (e.g. taskset -c X ./exec)");

    auto duration_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    uint64_t cycles = rdtsc_end - rdtsc_start;

    return (double)cycles / duration_ns.count();
}

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


// const uint64_t MaxI = 1000000;

class QueueFixture: public benchmark::Fixture {
	public:
		QueueFixture() = default;
		SPMCQueue<Msg, 1024> q;
		std::thread t;
		bool done=false;

		void SetUp(benchmark::State& st){
			if (st.thread_index() == 0) {
				done = false;
				t = std::thread([&](){
					int cnt = 0;
					cpupin(4);
					while (!this->done){
						cnt++;
						q.write(Msg{rdtsc(), {0}});
					    auto expire = rdtsc() + st.range(0);
					    while (rdtsc() < expire) continue;
					}
					st.counters["Dropped"] = cnt - st.iterations();
				});
			}
		}
		void TearDown(benchmark::State& st){
			if (st.thread_index() == 0) {
				done = true;
				t.join();
			}
		}
};

BENCHMARK_DEFINE_F(QueueFixture, read)(benchmark::State& state){
	uint64_t cycles = 0;
	auto r1 = q.get_reader();
	for (auto _ : state){
		auto tick = r1.read();
	    auto now = rdtsc();
		cycles += now - tick.tsc;
	}
	state.counters["Latency (ns)"] = benchmark::Counter((float)cycles/state.iterations() * 1/getghz(), benchmark::Counter::kAvgThreads);
}

BENCHMARK_REGISTER_F(QueueFixture,read) -> Range(5, 5<<10) -> Threads(1) -> Threads(2) -> Threads(3);
BENCHMARK_MAIN();


// int main(int argc, char** argv)
// {
// 	namespace po = boost::program_options;
// 	po::options_description cmd_desc("Allowed options");
// 	cmd_desc.add_options()("help", "produce help message")("child,c", po::value<int>(),"Child instance")("path,p", po::value<std::string>(), "path");

//     po::variables_map vm;
//     po::store(po::parse_command_line(argc, argv, cmd_desc),vm);
//     po::notify(vm);
// 	SPMCQueue<Msg, 1024> q1;
// 	// q1.write(Tick{std::time(nullptr), 0, 23.0, 20.0, 124.0, 312.0});
// 	// q1.write(Tick{std::time(nullptr), 1, 13.0, 10.0, 14.0, 112.0});
// 	std::thread t1([&q1](){
// 		cpupin(1);
// 		while (true){
// 			if (tick.i[0] == MaxI) break;
// 			// sleep(2);
// 		}
// 	});

// 	t1.join();
// 	t2.join();

// 	return 0;
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
// }
