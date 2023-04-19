#ifndef bar_hpp_INCLUDED
#define bar_hpp_INCLUDED

#include <chrono>
#include <ctime>
#include "nlohmann/json.hpp"

struct Bar {
    using hrc = std::chrono::high_resolution_clock;

    std::chrono::time_point<hrc> timestamp;
    float open;
    float high;
    float low;
    float close;
    int volume;
};

using json = nlohmann::json;
void to_json(json& j, const Bar& t);
void from_json(const json& j, Bar& t);
#endif  // bar_hpp_INCLUDED
