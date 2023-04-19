#include "pvm_cpp/dataset.hpp"
#include <fstream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;
Dataset::Dataset() : ticker("") {}

Dataset::Dataset(std::string filepath) {
    std::ifstream f(filepath);
    auto data = json::parse(f);
    ticker = data["ticker"];
    for (json bar : data["bars"]) {
        bars.push_back(bar.get<Bar>());
    }
}

int Dataset::size() const {
    return bars.size();
}

void Dataset::add(const Bar& bar) {
    q.write(bar);
    bars.push_back(bar);
}
