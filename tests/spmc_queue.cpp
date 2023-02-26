#include <gtest/gtest.h>
#include "pvm_cpp/spmc_queue.hpp"
#include "pvm_cpp/tick.hpp"
#include "pvm_cpp/chrono.hpp"


TEST(Basic, Basic){
	SPMCQueue<Tick, 1024> q;
	auto t = Tick{hrc::now(), 10, 0.1, 0.1, 20.1, 20.1}; 
	auto r = q.get_reader();

	Tick readtick;
	auto success = r.read(readtick);
	EXPECT_EQ(success, false); 
	q.write(t);
	success = r.read(readtick);
	EXPECT_EQ(success, true);
	EXPECT_EQ(readtick.low, t.low);
}
