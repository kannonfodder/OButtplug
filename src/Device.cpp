#pragma once

#include "Device.h"

#include "BPInterface.h"
#include "BpIo/Messages.h"

namespace OButtplug {
    Device::Device(DeviceListDevice dev) {
        this->deviceIndex = dev.DeviceIndex;
        this->deviceName = dev.DeviceName;
        this->deviceMessages = dev.DeviceMessages;
    }

    Device::Device(DeviceAdded dev) {
        this->deviceIndex = dev.DeviceIndex;
        this->deviceMessages = dev.DeviceMessages;
        this->deviceName = dev.DeviceName;
    }

    void Device::vibrateAndStop(float power, int timeMs) {
        std::thread vibr_t(
            [this,timeMs,power]() {
                this->vibrate(power);
                std::this_thread::sleep_for(std::chrono::milliseconds(timeMs));
                this->stop();
            });
        vibr_t.detach();
    }

    void Device::vibrate(float power) {
        if (deviceMessages.VibrateCmd.available) {
            if (power < 0) {
                power = 0;
            }
            if (power > 1.0) {
                power = 1.0;
            }
            VibrateSpeed vs{power};
            VibrateCmd cmd{99, this->deviceIndex, {vs}};
            BPInterface::GetSingleton()->sendMessage(cmd);
        }
    }

    void Device::stop() {
        StopDeviceCmd stop{100, deviceIndex};
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        BPInterface::GetSingleton()->sendMessage(stop);
    }
}  // namespace OButtplug