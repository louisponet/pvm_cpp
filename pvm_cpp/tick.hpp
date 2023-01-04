#ifndef tick_hpp_INCLUDED
#define tick_hpp_INCLUDED

#include <ctime>
class Tick {
	public:
		std::time_t timestamp;
		int volume;
		float open;
		float close;
		float high;
		float low;

		Tick(std::time_t timestamp, int volume, float open, float close, float high, float low) : timestamp(timestamp), volume(volume), open(open), close(close), high(high), low(low) {};
};

#endif // tick_hpp_INCLUDED
