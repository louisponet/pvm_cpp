#pragma once

#include <atomic>
#include <string>
#include "pvm_cpp/bar.hpp"
#include "httplib.h"
#include "nlohmann/json.hpp"

class AlpacaBroker {

   public:

    AlpacaBroker(const std::string alpaca_key_id,
                 const std::string alpaca_secret, 
                 const int max_requests = 200);

    AlpacaBroker() = delete;

	std::vector<Bar> get_bars(const std::string& ticker,
                              const std::string& start, 
	                          const std::string& stop,
                              const std::string& timeframe);

   private:
	std::vector<Bar> parse_bars(httplib::Result& response);
    httplib::Headers headers();

   private:
    std::string alpaca_key_id;
    std::string alpaca_secret;

    std::atomic<int> cur_requests = 0;
    const int max_requests = 200;  // per minute
};
