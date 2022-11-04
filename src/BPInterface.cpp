#pragma once
#include "BPInterface.h"

#include "BpIo/Messages.h"
#include "BpIo/WebsocketClient.h"

void makeSession(std::atomic<std::shared_ptr<session>>* sess) {
    try {
        net::io_context ioc;
        sess->store(std::make_shared<session>(ioc));
        auto const host = "127.0.0.1";
        auto const port = "12345";
        sess->load()->startConnection(host, port);
        logger::info("starting buttplug handshake");
        logger::info("running ws");
        ioc.run();
    } catch (std::exception const& e) {
        logger::error("It broke inside {}", e.what());
    }
}

namespace OButtplug {

    void BPInterface::StartConnection() {
        logger::info("creating session");
        std::thread(makeSession, &m_session).detach();
    }

    std::vector<std::string> BPInterface::FetchDevices() {
        std::vector<std::string> a;
        a.push_back("-None-");
        for (auto device : deviceMap) {
            a.push_back(device.second);
        }
        return a;
    }

    void BPInterface::WSMessageRecieved(std::string bufferMessage) {
        auto msg = json::parse(bufferMessage);
        logger::info("{}", msg.dump());
        if (msg.is_array()) {
            auto& message = msg[0];
            // TODO: Change device map over to holding struct
            if (message["DeviceAdded"] != nullptr) {
                auto& deviceAdded = message["DeviceAdded"];
                logger::info("Device Added {} , {} , {}", deviceAdded["Id"], deviceAdded["DeviceName"],
                             deviceAdded["DeviceIndex"]);
                deviceMap.insert(std::make_pair(deviceAdded["DeviceIndex"].get<int>(),
                                                deviceAdded["DeviceName"].get<std::string>()));
            } else if (message["DeviceList"] != nullptr) {
                auto& deviceListJson = message["DeviceList"];
                auto deviceList = deviceListJson.get<DeviceList>();
                for (auto dev : deviceList.Devices) {
                    auto it = deviceMap.find(dev.DeviceIndex);
                    if (it == deviceMap.end()) {
                        deviceMap.insert(std::make_pair(dev.DeviceIndex, dev.DeviceName));
                    }
                }
            }
        }
    }

    void BPInterface::DeviceLookup() {
        RequestDevices rD{1};
        json j2 = rD;
        m_session.load()->send(boost::make_shared<std::string>(j2.dump()));

        StartScanning msg{2};
        json j = msg;
        m_session.load()->send(boost::make_shared<std::string>(j.dump()));
    }

}  // namespace OButtplug