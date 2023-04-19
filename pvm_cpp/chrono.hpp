#ifndef chrono_hpp_INCLUDED
#define chrono_hpp_INCLUDED

#include <chrono>

using hrc = std::chrono::high_resolution_clock;
using namespace std::literals;
using ns_t = std::chrono::nanoseconds;
using time_point = std::chrono::time_point<hrc>;

static inline uint64_t rdtsc() {
    uint64_t rax, rdx;
    __asm__ __volatile__("rdtsc" : "=a"(rax), "=d"(rdx));
    return (rdx << 32) + rax;
}

#endif  // chrono_hpp_INCLUDED
