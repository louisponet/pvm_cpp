#include <gtest/gtest.h>
#include "pvm_cpp/spmc_queue.hpp"
#include "pvm_cpp/bar.hpp"
#include "pvm_cpp/chrono.hpp"


TEST(Basic, Basic){
	SPMCQueue<Bar, 1024> q;
	auto t = Bar{hrc::now(), 10, 0.1, 0.1, 20.1, 20.1}; 
	auto r = q.get_reader();

	Bar readbar;
	auto success = r.read(readbar);
	EXPECT_EQ(success, false); 
	q.write(t);
	success = r.read(readbar);
	EXPECT_EQ(success, true);
	EXPECT_EQ(readbar.low, t.low);
}
