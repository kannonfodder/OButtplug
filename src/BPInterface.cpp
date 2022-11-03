#pragma once
#include "BPInterface.h"


void makeSession(std::atomic<std::shared_ptr<session>>* sess){
    try {
        net::io_context ioc;
        sess->store(std::make_shared<session>(ioc));
        auto const host = "127.0.0.1";
        auto const port = "12345";
        sess->load()->startConnection(host, port);
        logger::info("starting buttplug handshake");
        logger::info("running ws");
        ioc.run();
    }
    catch (std::exception const& e) {
        logger::error("It broke inside {}", e.what());
    }
}

namespace OButtplug {
    void BPInterface::StartConnection() {
        logger::info("creating session");
        std::thread(makeSession, &m_session)
            .detach();

    }
    std::vector<std::string> BPInterface::FetchDevices() {
        std::vector<std::string> a;
        a.push_back("Device 1");
        return a;
    }
    void BPInterface::LogThing(int someData) {
    }
}  // namespace OButtplug