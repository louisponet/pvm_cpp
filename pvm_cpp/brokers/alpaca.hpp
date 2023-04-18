#pragma once

#include <string>

class AlpacaBroker {
	public:
		AlpacaBroker(const std::string alpaca_key_id, const std::string alpaca_secret);
		AlpacaBroker() = delete;

	private:
		std::string alpaca_key_id;
		std::string alpaca_secret;

};
