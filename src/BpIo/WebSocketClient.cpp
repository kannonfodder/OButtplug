#pragma once
#include "WebSocketClient.h"
#include "BPInterface.h"

void session::startConnection(char const* host, char const* port) {
    // Save these for later
    host_ = host;
    logger::info("start domain lookup");
    // Look up the domain name
    resolver_.async_resolve(host, port, beast::bind_front_handler(&session::on_resolve, shared_from_this()));
    ws_.text(true);
}

void session::performButtplugHandshake() {
    auto j_arry = json::array();
    auto msg = json::parse(R"({"RequestServerInfo":{"Id":1,"ClientName":"OButtplug","MessageVersion":1}})");
    j_arry.push_back(msg);
    auto const text = j_arry.dump();
    ws_.async_write(net::buffer(text), beast::bind_front_handler(&session::on_write_bpHandshake, shared_from_this()));
}

void session::on_resolve(beast::error_code ec, tcp::resolver::results_type results) {
    if (ec) return fail(ec, "resolve");

    logger::info("set timeout");
    // Set the timeout for the operation
    beast::get_lowest_layer(ws_).expires_after(std::chrono::seconds(30));
    logger::info("ws connect");
    // Make the connection on the IP address we get from a lookup
    beast::get_lowest_layer(ws_).async_connect(results,
                                               beast::bind_front_handler(&session::on_connect, shared_from_this()));
}

void session::on_connect(beast::error_code ec, tcp::resolver::results_type::endpoint_type ep) {
    if (ec) return fail(ec, "connect");

    // Turn off the timeout on the tcp_stream, because
    // the websocket stream has its own timeout system.
    beast::get_lowest_layer(ws_).expires_never();

    // Set suggested timeout settings for the websocket
    ws_.set_option(websocket::stream_base::timeout::suggested(beast::role_type::client));
    logger::info("ws change UA");
    // Set a decorator to change the User-Agent of the handshake
    ws_.set_option(websocket::stream_base::decorator([](websocket::request_type& req) {
        req.set(http::field::user_agent, std::string(BOOST_BEAST_VERSION_STRING) + " websocket-client-async");
    }));

    // Update the host_ string. This will provide the value of the
    // Host HTTP header during the WebSocket handshake.
    // See https://tools.ietf.org/html/rfc7230#section-5.4
    host_ += ':' + std::to_string(ep.port());
    logger::info("ws handshake");
    // Perform the websocket handshake
    ws_.async_handshake(host_, "/", beast::bind_front_handler(&session::on_handshake, shared_from_this()));
}

void session::on_handshake(beast::error_code ec) {
    if (ec) return fail(ec, "handshake");
    logger::info("finish initial websocket handshake");
    ready = true;
    // Send the message
    performButtplugHandshake();
}

void session::on_write_bpHandshake(beast::error_code ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);
    if (ec) return fail(ec, "hs write");
    logger::info("handshake write complete");
    ws_.async_read(buffer_hs, beast::bind_front_handler(&session::on_read_bpHandshake, shared_from_this()));
}
void session::on_read_bpHandshake(beast::error_code ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if (ec) return fail(ec, "hs read");
    logger::info("binary: {} text: {}", ws_.got_binary(), ws_.got_text());
    auto msg = json::parse(beast::buffers_to_string(buffer_hs.data()));
    logger::info("{}", msg.dump());
    auto ping = msg[0]["ServerInfo"]["MaxPingTime"].get<int>();
    logger::info("Ping Time : {}", ping);

    startReadLoop();
    // startPingLoop(ping);

    // auto getDevicesMsg = std::string(R"([{"StartScanning":{"Id":1}}])");
    // ws_.async_write(net::buffer(getDevicesMsg), beast::bind_front_handler(&session::on_write_devices,
    // shared_from_this()));
}

void session::on_write_devices(beast::error_code ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);
    if (ec) return fail(ec, "write fetch dev read");
    beast::flat_buffer buffer;
    ws_.read(buffer);
    auto msg = json::parse(beast::buffers_to_string(buffer.data()));
    logger::info("{}", msg.dump());
}

void session::startReadLoop() {
    ws_.async_read(buffer_, beast::bind_front_handler(&session::on_read, shared_from_this()));
}

void session::on_read(beast::error_code ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if (ec) return fail(ec, "read");
    logger::info("doing read");

    auto bufferData = beast::buffers_to_string(buffer_.data());
    OButtplug::BPInterface::GetSingleton()->WSMessageRecieved(bufferData);
    // Trigger next read
    if (ws_.is_open()) {
        buffer_.consume(buffer_.size());
        // std::this_thread::sleep_for(std::chrono::milliseconds(50));
        ws_.async_read(buffer_, beast::bind_front_handler(&session::on_read, shared_from_this()));
    }
}

void session::on_close(beast::error_code ec) {
    if (ec) return fail(ec, "close");

    // If we get here then the connection is closed gracefully

    // The make_printable() function helps print a ConstBufferSequence
    // std::cout << beast::make_printable(buffer_.data()) << std::endl;
}

void session::send(boost::shared_ptr<std::string const> const& ss) {
    // Post our work to the strand, this ensures
    // that the members of `this` will not be
    // accessed concurrently.
    logger::info("Sending {}", ss.get()->c_str());
    net::post(ws_.get_executor(), beast::bind_front_handler(&session::on_send, shared_from_this(), ss));
}

void session::on_send(boost::shared_ptr<std::string const> const& ss) {
    // Always add to queue
    queue_.push_back(ss);

    // Are we already writing?
    if (queue_.size() > 1) return;

    // We are not currently writing, so send this immediately
    ws_.async_write(net::buffer(*queue_.front()), beast::bind_front_handler(&session::on_write, shared_from_this()));
}

void session::on_write(beast::error_code ec, std::size_t) {
    // Handle the error, if any
    if (ec) return fail(ec, "write");

    // Remove the string from the queue
    queue_.erase(queue_.begin());

    // Send the next message if any
    if (!queue_.empty())
        ws_.async_write(net::buffer(*queue_.front()),
                        beast::bind_front_handler(&session::on_write, shared_from_this()));
}
