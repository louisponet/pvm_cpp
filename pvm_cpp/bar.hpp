#ifndef bar_hpp_INCLUDED
#define bar_hpp_INCLUDED

#include <ctime>
#include <chrono>
#include "nlohmann/json.hpp"

class Bar {
	using hrc = std::chrono::high_resolution_clock;

	public:
		std::chrono::time_point<hrc> timestamp;
		float open;
		float high;
		float low;
		float close;
		int volume;

		//Bar(std::time_t timestamp, int volume, float open, float close, float high, float low) : timestamp(timestamp), volume(volume), open(open), close(close), high(high), low(low) {};
		//Bar() : timestamp(std::time_t{0}), 
	 //             volume(0),
	 //             open(0.0),
	 //             close(0.0),
	 //             high(0.0),
	 //             low(0.0){};
};

using json = nlohmann::json;
void to_json(json& j, const Bar& t);
void from_json(const json& j, Bar& t);
#endif // bar_hpp_INCLUDED
