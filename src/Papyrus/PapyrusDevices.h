#pragma once

#include <BPInterface.h>

namespace PapyrusDevices {
    using VM = RE::BSScript::IVirtualMachine;

    std::vector<std::string> GetConnectedDevices(RE::StaticFunctionTag*) {
        auto intfc = OButtplug::BPInterface::GetSingleton();
        return intfc->FetchDevices();
    }

    void SetDevice(RE::StaticFunctionTag*, int slot, std::string deviceName) {
        logger::info("Saving device {} in slot {}", deviceName, slot);
    }

    void ScanForDevices(RE::StaticFunctionTag*) {
        OButtplug::BPInterface::GetSingleton()->DeviceLookup();
    }

    bool Bind(VM* a_vm) {
        const auto obj = "OButtplugNative"sv;

        BIND(GetConnectedDevices);
        BIND(SetDevice);
        BIND(ScanForDevices);

        return true;
    }
}  // namespace PapyrusDevices
