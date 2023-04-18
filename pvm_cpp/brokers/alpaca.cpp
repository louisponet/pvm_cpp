#include "pvm_cpp/brokers/alpaca.hpp"

httplib::Headers headers(const std::string& alpaca_key_id, const std::string& alpaca_secret){
  return {
      {"APCA-API-KEY-ID", alpaca_key_id},
      {"APCA-API-SECRET-KEY", alpaca_secret},
  };

}
AlpacaBroker::AlpacaBroker(const std::string alpaca_key_id_, 
                           const std::string alpaca_secret_,
						   const int max_requests): cur_requests(0), max_requests(max_requests){

	httplib::Client client("paper-api.alpaca.markets");

	auto resp = client.Get("/v2/account", ::headers(alpaca_key_id_, alpaca_secret_));
	if (!resp) {
		throw "Error couldn't connect to alpaca";
	}

	alpaca_key_id = alpaca_key_id_;
	alpaca_secret = alpaca_secret_;

}

AlpacaBroker::AlpacaBroker(const std::string alpaca_key_id_, 
                           const std::string alpaca_secret_) : 
	AlpacaBroker(alpaca_key_id_, alpaca_secret_, 200){}

httplib::Headers AlpacaBroker::headers() {
	return ::headers(alpaca_key_id, alpaca_secret);
};

std::vector<Bar> AlpacaBroker::get_bars(const std::string& ticker,
                                        const std::string& start,
                                        const std::string& stop,
                                        const std::string& timeframe){



}
