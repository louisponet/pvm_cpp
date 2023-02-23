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
// Dataset& Dataset::operator=(Dataset other){
// 	std::swap(ticker, other.ticker);
// 	std::swap(ticks, other.ticks);
	// std::swap(q, other.q);
// };

int Dataset::size() const {
	return ticks.size();
}

void Dataset::add(const Tick& tick){
	std::cout << "writing tick" << std::endl;
	q.write(tick);
	ticks.push_back(tick);
}
