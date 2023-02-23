#include "pvm_cpp/tick.hpp"

using json = nlohmann::json;

void to_json(json& j, const Tick& t) {
	j = json{{"timestamp", t.timestamp}, 
	          {"volume", t.volume},
	          {"open", t.open},
	          {"close", t.close},
	          {"high", t.high},
	          {"low", t.low}};
}

void from_json(const json& j, Tick& t) {
	j.at("timestamp").get_to(t.timestamp);
	j.at("volume").get_to(t.volume);
	j.at("open").get_to(t.open);
	j.at("close").get_to(t.close);
	j.at("high").get_to(t.high);
	j.at("low").get_to(t.low);
}

