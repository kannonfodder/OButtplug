#pragma once

#include <BPInterface.h>

namespace PapyrusDevices {
    using VM = RE::BSScript::IVirtualMachine;

    std::vector<std::string> GetConnectedDevices(RE::StaticFunctionTag*) {
        auto intfc = OButtplug::BPInterface::GetSingleton();
        return intfc->FetchDevices();
    }

    bool SetDevice(RE::StaticFunctionTag*, int slot, std::string deviceName) {
        logger::info("Saving device {} in slot {}", deviceName, slot);
        auto intfc = OButtplug::BPInterface::GetSingleton();
        return intfc->SaveDeviceInSlot(static_cast<OButtplug::Device::Slot>(slot), deviceName);
    }

    void ScanForDevices(RE::StaticFunctionTag*) {
        OButtplug::BPInterface::GetSingleton()->DeviceLookup();
    }

    void TestSelectedDevices(RE::StaticFunctionTag*) {
        OButtplug::BPInterface::GetSingleton()->TestDevices();
    }

    std::string GetConnectionStatus(RE::StaticFunctionTag*){
        if (OButtplug::BPInterface::GetSingleton()->CheckConnection()) {
            return "Connected";
        }
        else {
            return "Disconnected";
        }
    }

    void ConnectToServer(RE::StaticFunctionTag*) {
        auto intfc = OButtplug::BPInterface::GetSingleton();
        if (intfc->CheckConnection()) {
            logger::warn("Attempted to reconnect, but already connected");
        }
        else {
            intfc->StartConnection();
        }
    }

    bool Bind(VM* a_vm) {
        const auto obj = "OButtplugNative"sv;

        BIND(GetConnectedDevices);
        BIND(SetDevice);
        BIND(ScanForDevices);
        BIND(TestSelectedDevices);
        BIND(GetConnectionStatus);
        BIND(ConnectToServer);

        return true;
    }
}  // namespace PapyrusDevices
