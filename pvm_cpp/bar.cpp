#include "pvm_cpp/bar.hpp"
#include <iostream>
#include "pvm_cpp/chrono.hpp"

using json = nlohmann::json;

void to_json(json& j, const Bar& t) {
    j = json{{"timestamp",
              std::chrono::duration_cast<ns_t>(t.timestamp.time_since_epoch())
                  .count()},
             {"volume", t.volume},
             {"open", t.open},
             {"close", t.close},
             {"high", t.high},
             {"low", t.low}};
}

void from_json(const json& j, Bar& t) {
    t.timestamp = time_point(ns_t(j.at("timestamp").get<uint64_t>()));
    j.at("volume").get_to(t.volume);
    j.at("open").get_to(t.open);
    j.at("close").get_to(t.close);
    j.at("high").get_to(t.high);
    j.at("low").get_to(t.low);
}
