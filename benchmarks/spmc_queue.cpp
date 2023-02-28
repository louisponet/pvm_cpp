#include "pvm_cpp/spmc_queue.hpp"
#include "benchmark/benchmark.h"
#include "pvm_cpp/chrono.hpp"
#include <ctime>
#include <x86intrin.h>
#include <thread>

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

using Clock = std::conditional_t<hrc::is_steady,
                                 hrc,
                                 std::chrono::steady_clock>;
double getghz(){

    int chip, core, chip2, core2;

    auto start = Clock::now();
	cpuid();
    uint64_t rdtsc_start = rdtscp(chip, core);

    std::this_thread::sleep_for(500ms);

    uint64_t rdtsc_end = rdtscp(chip2, core2);
    cpuid();

    auto end = Clock::now();

    auto duration_ns = std::chrono::duration_cast<ns_t>(end - start);
    uint64_t cycles = rdtsc_end - rdtsc_start;

    return (double)cycles / duration_ns.count();
}

struct Msg
{
  hrc::time_point tsc;
  uint64_t rdtsc;
};

bool cpupin(int cpuid) {
  cpu_set_t my_set;
  CPU_ZERO(&my_set);
  CPU_SET(cpuid, &my_set);
  // if (sched_setaffinity(0, sizeof(cpu_set_t), &my_set)) {
  //   std::cout << "sched_setaffinity error: " << strerror(errno) << std::endl;
  //   return false;
  // }

  return true;
}

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
					cpupin(4);
					while (!this->done){
						q.write(Msg{hrc::now(), rdtsc()});
					    auto expire = rdtsc() + st.range(0);
					    while (rdtsc() < expire) continue;
					}
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
	uint64_t totread = 0; 
	std::chrono::nanoseconds dur(0); 
	auto r1 = q.get_reader();
	Msg curmsg;
	for (auto _ : state){
		if (r1.read(curmsg)){
		    auto now_cycles = rdtsc();
		    auto now_clock = hrc::now();
			cycles += now_cycles - curmsg.rdtsc;
			dur += now_clock - curmsg.tsc; 
			totread++;
		}
	}
	state.counters["avg cycles"] = benchmark::Counter((double)cycles/totread, benchmark::Counter::kAvgThreads);
	state.counters["Latency from cycles (ns)"] = benchmark::Counter((double)cycles/totread * 1/getghz(), benchmark::Counter::kAvgThreads);
	state.counters["Latency from timer  (ns)"] = benchmark::Counter((double)dur.count()/totread, benchmark::Counter::kAvgThreads);
}

BENCHMARK_REGISTER_F(QueueFixture,read) -> Range(5, 5<<10) -> Threads(1) -> Threads(2) -> Threads(3);
BENCHMARK_MAIN();
