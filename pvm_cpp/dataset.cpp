#include "pvm_cpp/dataset.hpp"
#include <fstream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;
Dataset::Dataset() : ticker(""){}

Dataset::Dataset(std::string filepath) {
	std::ifstream f(filepath);
	auto data = json::parse(f);
	ticker = data["ticker"];
	for (auto tick : data["ticks"]) {
		ticks.push_back(Tick(tick["timestamp"].get<std::time_t>(), 
		              tick["volume"].get<int>(), 
		              tick["open"].get<float>(), 
		              tick["close"].get<float>(),
		              tick["high"].get<float>(),
		              tick["low"].get<float>()));
	}
}
int Dataset::size() {
	return ticks.size();
}
