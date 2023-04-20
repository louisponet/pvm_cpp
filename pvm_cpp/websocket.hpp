#ifndef websocket_hpp_INCLUDED
#define websocket_hpp_INCLUDED

#include "websocketpp/client.hpp"
#include "websocketpp/common/memory.hpp"
#include "websocketpp/common/thread.hpp"
#include "websocketpp/config/asio_no_tls_client.hpp"

#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

typedef websocketpp::client<websocketpp::config::asio_client> client;

class ConnectionMetadata {
   public:
    typedef websocketpp::lib::shared_ptr<ConnectionMetadata> ptr;

    ConnectionMetadata(int id, websocketpp::connection_hdl handle,
                       std::string uri)
        : id(id),
          handle(handle),
          status("Connecting"),
          uri(uri),
          server("N/A") {}

    void on_open(client* c, websocketpp::connection_hdl handle);

    void on_fail(client* c, websocketpp::connection_hdl handle);

    websocketpp::connection_hdl get_handle() { return handle; }
    std::string get_status() { return status; }
    int get_id() { return id; }

    void record_sent_message(std::string message);

    friend std::ostream& operator<<(std::ostream& out,
                                    ConnectionMetadata const& data);

   private:
    int id;
    websocketpp::connection_hdl handle;
    std::string status;
    std::string uri;
    std::string server;
    std::string error_reason;
    std::vector<std::string> messages;
};

class WebsocketEndpoint {
   public:
    WebsocketEndpoint();

    int connect(std::string const& uri);

    void send(int id, std::string message);

    void close(int id, websocketpp::close::status::value code);

    ~WebsocketEndpoint();

    ConnectionMetadata::ptr get_metadata(int id) const;

   private:
    typedef std::map<int, ConnectionMetadata::ptr> connection_map;

    client endpoint;
    websocketpp::lib::shared_ptr<websocketpp::lib::thread> thread;

    connection_map connections;
    int next_id;
};

#endif  // websocket_hpp_INCLUDED
