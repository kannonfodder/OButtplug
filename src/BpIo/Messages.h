#pragma once

struct Ping {
    int Id;
};
struct PingMessage {
    Ping Ping;
};

void to_json(json& j, const PingMessage& pm) {
    json arry;
    arry.push_back(json{{"Ping", json{{"Id", pm.Ping.Id}}}});
    j = arry;
}

struct StartScanning {
    int Id;
};

struct StartScanningMessage {
    StartScanning StartScanning;
};

void to_json(json& j, const StartScanning& ss) {
    json arry;
    arry.push_back(json{{"StartScanning", json{{"Id", ss.Id}}}});
    j = arry;
}


struct RequestDevices {
    int Id;
};

void to_json(json& j, const RequestDevices& rd) {
    json arry;
    arry.push_back(json{ {"RequestDeviceList", json{{"Id", rd.Id}}} });
    j = arry;
}


struct DeviceMessageScalarCmd {
    int StepCount;
    std::string FeatureDescriptor;
    std::string ActuatorType;
};

struct DeviceMessageSingleMotorVibrateCmd {
    bool available;
};

struct DeviceMessageVibrateCmd {
    bool available;
    int FeatureCount;
};

struct DeviceMessageStopCmd {
    bool available;
};

struct DeviceMessages{
    DeviceMessageSingleMotorVibrateCmd SingleMotorVibrateCmd;
    DeviceMessageVibrateCmd VibrateCmd;
    std::vector<DeviceMessageScalarCmd> ScalarCmd;
    DeviceMessageStopCmd StopDeviceCmd;
};

struct DeviceListDevice {
    std::string DeviceName;
    int DeviceIndex;
    DeviceMessages DeviceMessages;
};

struct DeviceList {
    int Id;
    std::vector<DeviceListDevice> Devices;    
};

void from_json(const json& j, DeviceList& dlm) {
    dlm.Id = j.at("Id");
    for (json device : j.at("Devices")) {
        DeviceListDevice dev;
        dev.DeviceName = device.at("DeviceName");
        dev.DeviceIndex = device.at("DeviceIndex");
        DeviceMessages devMsgs;
        DeviceMessageStopCmd stopCmd;
        devMsgs.StopDeviceCmd = stopCmd;
        if (device.at("DeviceMessages")["SingleMotorVibrateCmd"] != nullptr) {
            DeviceMessageSingleMotorVibrateCmd cmd{ true };
            devMsgs.SingleMotorVibrateCmd = cmd;
        }
        if (device.at("DeviceMessages")["VibrateCmd"] != nullptr) {
            DeviceMessageVibrateCmd vibrateCmd{ true };
            devMsgs.VibrateCmd = vibrateCmd;
        }
        if (device.at("DeviceMessages")["ScalarCmd"] != nullptr) {
            for (json deviceScalarCommand : device.at("DeviceMessages").at("ScalarCmd")) {
                DeviceMessageScalarCmd scalarCmd;
                scalarCmd.ActuatorType = deviceScalarCommand.at("ActuatorType");
                scalarCmd.FeatureDescriptor = deviceScalarCommand.at("FeatureDescriptor");
                scalarCmd.StepCount = deviceScalarCommand.at("StepCount");
                devMsgs.ScalarCmd.push_back(scalarCmd);
            }
        }
        
        dev.DeviceMessages = devMsgs;
        dlm.Devices.push_back(dev);
    }
}