#include "pvm_cpp/dataset.hpp"
#include <fstream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;
Dataset::Dataset() : ticker(""){}

Dataset::Dataset(std::string filepath) {
	std::ifstream f(filepath);
	auto data = json::parse(f);
	ticker = data["ticker"];
	for (json tick : data["ticks"]) {
		ticks.push_back(tick.get<Tick>());
	}
}

int Dataset::size() const {
	return ticks.size();
}

void Dataset::add(const Tick& tick){
	q.write(tick);
	ticks.push_back(tick);
}
