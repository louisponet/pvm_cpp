#include "pvm_cpp/brokers/alpaca.hpp"
#include <iostream>
#include <sstream>
#include "date/date.h"
#include "fmt/core.h"
#include "nlohmann/json.hpp"

httplib::Headers headers(const std::string& alpaca_key_id,
                         const std::string& alpaca_secret) {
    return {
        {"APCA-API-KEY-ID", alpaca_key_id},
        {"APCA-API-SECRET-KEY", alpaca_secret},
    };
}
AlpacaBroker::AlpacaBroker(const std::string alpaca_key_id_,
                           const std::string alpaca_secret_,
                           const int max_requests)
    : max_requests(max_requests) {

    httplib::SSLClient client("paper-api.alpaca.markets");

    auto resp =
        client.Get("/v2/account", ::headers(alpaca_key_id_, alpaca_secret_));
    if (!resp) {
        throw "Error couldn't connect to alpaca";
    }
    alpaca_key_id = alpaca_key_id_;
    alpaca_secret = alpaca_secret_;
}

// AlpacaBroker::AlpacaBroker(const std::string alpaca_key_id_,
//                            const std::string alpaca_secret_) :
// 	AlpacaBroker(alpaca_key_id_, alpaca_secret_, 200){}

httplib::Headers AlpacaBroker::headers() {
    return ::headers(alpaca_key_id, alpaca_secret);
};

std::vector<Bar> AlpacaBroker::get_bars(const std::string& ticker,
                                        const std::string& start,
                                        const std::string& stop,
                                        const std::string& timeframe) {

    httplib::SSLClient client("data.alpaca.markets");

    httplib::Params params{{"limit", "1000"},
                           {"timeframe", timeframe},
                           {"start", start},
                           {"end", stop}};

    std::string query_string = httplib::detail::params_to_query_str(params);

    auto resp = client.Get(
        fmt::format("/v2/stocks/{}/bars?{}", ticker, query_string).c_str(),
        headers());

    if (!resp) {
        throw "Couldn't retrieve bars";
    }

    auto data = nlohmann::json::parse(resp->body);

    auto datbars = data["bars"];
    uint16_t nbars = data["bars"].size();

    std::vector<Bar> bars(nbars);

    for (int i = 0; i < nbars; i++) {
        auto curbar = datbars[i];
        std::istringstream in(curbar["t"].get<std::string>());
        std::chrono::time_point<std::chrono::high_resolution_clock> tp;
        in >> date::parse("%4y-%2m-%2dT%2h:%2m:%2sZ", tp);
        bars[i] = Bar{tp,
                      curbar["o"].get<float>(),
                      curbar["h"].get<float>(),
                      curbar["l"].get<float>(),
                      curbar["c"].get<float>(),
                      curbar["v"].get<int>()};
    }
    return bars;
}
