#pragma once

#include <string>
#include "httplib.h"
#include "pvm_cpp/bar.hpp"

class AlpacaBroker {
	public:
		AlpacaBroker(const std::string alpaca_key_id, const std::string alpaca_secret);
		AlpacaBroker() = delete;


		std::vector<Bar> get_bars();
	private:

		httplib::Headers headers();

		std::string alpaca_key_id;
		std::string alpaca_secret;


};
