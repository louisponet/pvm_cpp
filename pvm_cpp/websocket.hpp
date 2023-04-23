#ifndef websocket_hpp_INCLUDED
#define websocket_hpp_INCLUDED

#include <websocketpp/client.hpp>
#include <websocketpp/common/memory.hpp>
#include <websocketpp/common/thread.hpp>
#include <websocketpp/config/asio_client.hpp>

#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

typedef websocketpp::client<websocketpp::config::asio_tls_client> WebsocketClient;

class ConnectionMetadata {
   public:
    typedef websocketpp::lib::shared_ptr<ConnectionMetadata> WebsocketMetadataPointer;

    ConnectionMetadata(int id, websocketpp::connection_hdl handle,
                       std::string uri)
        : id(id),
          handle(handle),
          status("Connecting"),
          uri(uri),
          server("N/A") {}

    void on_open(WebsocketClient* c, websocketpp::connection_hdl handle);

    void on_fail(WebsocketClient* c, websocketpp::connection_hdl handle);


    void on_message(websocketpp::connection_hdl handle, WebsocketClient::message_ptr msg);

    websocketpp::connection_hdl get_handle() { return handle; }
    std::string get_status() { return status; }
    int get_id() { return id; }

    const std::string& last_message() const;
    int n_messages() const;

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
    typedef std::shared_ptr<boost::asio::ssl::context> SSLContextPointer;
    WebsocketEndpoint();

    int connect(std::string const& uri);

    static SSLContextPointer on_tls_init();
    void send(int id, std::string message);

    void close(int id, websocketpp::close::status::value code);

    ~WebsocketEndpoint();

    ConnectionMetadata::WebsocketMetadataPointer get_metadata(int id) const;

   private:
    typedef std::map<int, ConnectionMetadata::WebsocketMetadataPointer> ConnectionMap;

    WebsocketClient endpoint;
    websocketpp::lib::shared_ptr<websocketpp::lib::thread> thread;

    ConnectionMap connections;
    int next_id;
};

#endif  // websocket_hpp_INCLUDED
