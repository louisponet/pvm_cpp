#include "pvm_cpp/spmc_queue.hpp"
#include <benchmark/benchmark.h>
#include <ctime>
#include <x86intrin.h>
#include <chrono>
#include <thread>

static inline uint64_t rdtsc()
{
    uint64_t rax, rdx;
    __asm__ __volatile__("rdtsc" : "=a"(rax), "=d"(rdx));
    return (rdx << 32) + rax;
}

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

    auto duration_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    uint64_t cycles = rdtsc_end - rdtsc_start;

    return (double)cycles / duration_ns.count();
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
