#ifndef tick_hpp_INCLUDED
#define tick_hpp_INCLUDED

#include <ctime>
#include "nlohmann/json.hpp"
class Tick {
	public:
		std::time_t timestamp;
		int volume;
		float open;
		float close;
		float high;
		float low;

		Tick(std::time_t timestamp, int volume, float open, float close, float high, float low) : timestamp(timestamp), volume(volume), open(open), close(close), high(high), low(low) {};
		Tick() : timestamp(std::time_t{0}), 
	             volume(0),
	             open(0.0),
	             close(0.0),
	             high(0.0),
	             low(0.0){};
};

using json = nlohmann::json;
void to_json(json& j, const Tick& t);
void from_json(const json& j, Tick& t);
#endif // tick_hpp_INCLUDED
