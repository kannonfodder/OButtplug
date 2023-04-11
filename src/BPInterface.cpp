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

    Device* BPInterface::getDeviceByName(const std::string& name) {
        for (auto& connectedDevice : connectedDevices) {
            if (connectedDevice.second.get()->deviceName == name) {
                return connectedDevice.second.get();
            }
        }
        return nullptr;
    }

    void BPInterface::StartConnection() {
        logger::info("creating session");
        std::thread(makeSession, &m_session).detach();
    }

    std::vector<std::string> BPInterface::FetchDevices() {
        std::vector<std::string> a;
        a.push_back("-None-");
        for (auto device : connectedDevices) {
            a.push_back(device.second.get()->deviceName);
        }
        return a;
    }

    void BPInterface::WSMessageRecieved(std::string bufferMessage) {
        auto msg = json::parse(bufferMessage);
        logger::info("{}", msg.dump());
        if (msg.is_array()) {
            auto& message = msg[0];
            if (message["DeviceAdded"] != nullptr) {
                auto deviceAdded = message["DeviceAdded"].get<DeviceAdded>();
                logger::info("Device Added {} , {} , {}", deviceAdded.Id, deviceAdded.DeviceName,
                             deviceAdded.DeviceIndex);

                Device newDevice = Device(deviceAdded);
                connectedDevices.insert_or_assign(newDevice.deviceIndex, std::make_shared<Device>(newDevice));
            } else if (message["DeviceList"] != nullptr) {
                auto& deviceListJson = message["DeviceList"];
                auto deviceList = deviceListJson.get<DeviceList>();
                for (auto dev : deviceList.Devices) {
                    Device newDevice = Device(dev);                    
                    connectedDevices.insert_or_assign(newDevice.deviceIndex, std::make_shared<Device>(newDevice));
                }
            } else if (message["DeviceRemoved"] != nullptr) {
                auto deviceRemoved = message["DeviceRemoved"].get<DeviceRemoved>();

                auto device = &connectedDevices.at(deviceRemoved.DeviceIndex);
                int toRemove = -1;
                for (auto& iter : selectedDevices) {
                    if (iter.second.get() == device->get()) {
                        toRemove = iter.first;
                        break;
                    }
                }
                if (toRemove != -1) {
                    selectedDevices.erase(toRemove);
                }

                connectedDevices.erase(device->get()->deviceIndex);
            }
        }
    }

    void BPInterface::DeviceLookup() {
        std::thread([&]() {
            RequestDevices rD{ 1 };
            json j2 = rD;
            m_session.load()->send(boost::make_shared<std::string>(j2.dump()));

            StartScanning msg{ 2 };
            json j = msg;
            m_session.load()->send(boost::make_shared<std::string>(j.dump()));

            std::this_thread::sleep_for(std::chrono::seconds(10));

            StopScanning ss{ 3 };
            json j3 = ss;
            m_session.load()->send(boost::make_shared<std::string>(j3.dump()));

        }).detach();
    }

    bool BPInterface::SaveDeviceInSlot(Device::Slot slot, std::string deviceName) {
        // Get pointer to connected device
        if (deviceName == "-None-") {
            logger::info("Not saving in slot {}", slot);
            if (selectedDevices.find(slot) != selectedDevices.end()) {
                logger::info("Erasing device in slot {}", slot);
                selectedDevices.erase(slot);
            }
            return true;
        }
        auto devicePtr = getDeviceByName(deviceName);        
        if (devicePtr == nullptr) {
            logger::error("Device {} not found", deviceName);
            return false;
        }
        std::shared_ptr<Device> device (devicePtr);
        // save into slot
        selectedDevices.insert_or_assign(slot, device);
        return true;
    }

    void BPInterface::TestDevices() {
        std::thread vibrateAndWait([&]() {
            for (auto& device : selectedDevices) {                
                device.second.get()->vibrate(1.0);
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                device.second.get()->stop();
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
        });
        vibrateAndWait.detach();
    }

    std::shared_ptr<Device> BPInterface::getDeviceBySlot(Device::Slot slot)
    {        
        auto device = selectedDevices.find(slot);
        if (device != selectedDevices.end()) {
            return std::shared_ptr<Device>(device->second);
        } else {
            return nullptr;
        }
        
    }

    bool BPInterface::CheckConnection() {
        return m_session.load()->isOpen();
    }
    

}  // namespace OButtplug