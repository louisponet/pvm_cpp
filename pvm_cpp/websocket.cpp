#include "pvm_cpp/websocket.hpp"

void ConnectionMetadata::on_open(WebsocketClient* c, websocketpp::connection_hdl handle) {

    status = "Open";

    WebsocketClient::connection_ptr connection = c->get_con_from_hdl(handle);

    server = connection->get_response_header("Server");
}

void ConnectionMetadata::on_fail(WebsocketClient* c, websocketpp::connection_hdl handle) {
    status = "Failed";

    WebsocketClient::connection_ptr con = c->get_con_from_hdl(handle);

    server       = con->get_response_header("Server");
    error_reason = con->get_ec().message();
}

void ConnectionMetadata::on_message(websocketpp::connection_hdl handle,
                                    WebsocketClient::message_ptr msg) {
    if (msg->get_opcode() == websocketpp::frame::opcode::text) {

        messages.push_back(msg->get_payload());

    } else {

        messages.push_back(websocketpp::utility::to_hex(msg->get_payload()));
    }
}

void ConnectionMetadata::record_sent_message(std::string message) {
    messages.push_back(">> " + message);
}

std::ostream& operator<<(std::ostream& out, ConnectionMetadata const& data) {
    out << "> URI: " << data.uri << "\n"
        << "> Status: " << data.status << "\n"
        << "> Remote Server: " << (data.server.empty() ? "None Specified" : data.server) << "\n"
        << "> Error/close reason: " << (data.error_reason.empty() ? "N/A" : data.error_reason);
    out << "> Messages Processed: (" << data.messages.size() << ") \n";

    std::vector<std::string>::const_iterator it;
    for (it = data.messages.begin(); it != data.messages.end(); ++it) {
        out << *it << "\n";
    }

    return out;
}

//WebSocketEndpoint
WebsocketEndpoint::WebsocketEndpoint() : next_id(0) {

    endpoint.clear_access_channels(websocketpp::log::alevel::all);
    endpoint.clear_error_channels(websocketpp::log::elevel::all);

    endpoint.init_asio();
    endpoint.start_perpetual();

    thread.reset(new websocketpp::lib::thread(&WebsocketClient::run, &endpoint));
}

WebsocketEndpoint::~WebsocketEndpoint() {
    endpoint.stop_perpetual();

    for (ConnectionMap::const_iterator it = connections.begin(); it != connections.end(); ++it) {
        if (it->second->get_status() != "Open") {
            // Only close open connections
            continue;
        }

        std::cout << "> Closing connection " << it->second->get_id() << std::endl;

        websocketpp::lib::error_code ec;
        endpoint.close(it->second->get_handle(), websocketpp::close::status::going_away, "", ec);
        if (ec) {
            std::cout << "> Error closing connection " << it->second->get_id() << ": "
                      << ec.message() << std::endl;
        }
    }

    thread->join();
}

int WebsocketEndpoint::connect(std::string const& uri) {
    websocketpp::lib::error_code ec;

    WebsocketClient::connection_ptr con = endpoint.get_connection(uri, ec);

    if (ec) {
        std::cout << "> Connect initialization error: " << ec.message() << std::endl;
        return -1;
    }

    int new_id = next_id++;
    ConnectionMetadata::WebsocketMetadataPointer metadata_ptr(
        new ConnectionMetadata(new_id, con->get_handle(), uri));
    connections[new_id] = metadata_ptr;

    con->set_open_handler(websocketpp::lib::bind(&ConnectionMetadata::on_open, metadata_ptr,
                                                 &endpoint, websocketpp::lib::placeholders::_1));

    con->set_fail_handler(websocketpp::lib::bind(&ConnectionMetadata::on_fail, metadata_ptr,
                                                 &endpoint, websocketpp::lib::placeholders::_1));

    con->set_message_handler(websocketpp::lib::bind(&ConnectionMetadata::on_message, metadata_ptr,
                                                    websocketpp::lib::placeholders::_1,
                                                    websocketpp::lib::placeholders::_2));

    endpoint.connect(con);

    return new_id;
}

void WebsocketEndpoint::send(int id, std::string message) {
    websocketpp::lib::error_code err;

    ConnectionMap::iterator it = connections.find(id);
    if (it == connections.end()) {
        std::cout << "> No connection found with id " << id << std::endl;
        return;
    }

    endpoint.send(it->second->get_handle(), message, websocketpp::frame::opcode::text, err);
    if (err) {
        std::cout << "> Error sending message: " << err.message() << std::endl;
        return;
    }

    it->second->record_sent_message(message);
}

void WebsocketEndpoint::close(int id, websocketpp::close::status::value code) {
    websocketpp::lib::error_code err;

    ConnectionMap::iterator metadata_it = connections.find(id);
    if (metadata_it == connections.end()) {
        std::cout << "> No connection found with id " << id << std::endl;
        return;
    }

    endpoint.close(metadata_it->second->get_handle(), code, "", err);
    if (err) {
        std::cout << "> Error initiating close: " << err.message() << std::endl;
    }
}

ConnectionMetadata::WebsocketMetadataPointer WebsocketEndpoint::get_metadata(int id) const {

    ConnectionMap::const_iterator it = connections.find(id);

    if (it == connections.end()) {
        return ConnectionMetadata::WebsocketMetadataPointer();
    } else {
        return it->second;
    }
}
