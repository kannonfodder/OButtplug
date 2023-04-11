#pragma once

struct Ping {
    int Id;
};
struct PingMessage {
    Ping Ping;
};

inline void to_json(json& j, const PingMessage& pm) {
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

inline void to_json(json& j, const StartScanning& ss) {
    json arry;
    arry.push_back(json{{"StartScanning", json{{"Id", ss.Id}}}});
    j = arry;
}

struct StopScanning {
    int Id;
};

inline void to_json(json& j, const StopScanning& ss) {
    json arry;
    arry.push_back(json{{"StopScanning", json{{"Id", ss.Id}}}});
    j = arry;
}

struct RequestDevices {
    int Id;
};

inline void to_json(json& j, const RequestDevices& rd) {
    json arry;
    arry.push_back(json{{"RequestDeviceList", json{{"Id", rd.Id}}}});
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

struct DeviceMessages {
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

inline void devMessages_from_json(const json& j, DeviceMessages& devMsgs) {
    if (j.find("DeviceMessages") == j.end()) {
        logger::warn("Attempted to find DeviceMessages in {}", j.dump());
        return;
    }
    auto& messages = j.at("DeviceMessages");
    if (j.at("DeviceMessages").find("SingleMotorVibrateCmd") != j.at("DeviceMessages").end()) {
        DeviceMessageSingleMotorVibrateCmd cmd{true};
        devMsgs.SingleMotorVibrateCmd = cmd;
    }

    if (j.at("DeviceMessages").find("VibrateCmd") != j.at("DeviceMessages").end()) {
        DeviceMessageVibrateCmd vibrateCmd{true};
        devMsgs.VibrateCmd = vibrateCmd;
    }

    if (j.at("DeviceMessages").find("ScalarCmd") != j.at("DeviceMessages").end()) {
        for (json deviceScalarCommand : j.at("DeviceMessages").at("ScalarCmd")) {
            DeviceMessageScalarCmd scalarCmd;
            scalarCmd.ActuatorType = deviceScalarCommand.at("ActuatorType");
            scalarCmd.FeatureDescriptor = deviceScalarCommand.at("FeatureDescriptor");
            scalarCmd.StepCount = deviceScalarCommand.at("StepCount");
            devMsgs.ScalarCmd.push_back(scalarCmd);
        }
    }
}

inline void from_json(const json& j, DeviceList& dlm) {
    dlm.Id = j.at("Id");
    for (json device : j.at("Devices")) {
        DeviceListDevice dev;
        dev.DeviceName = device.at("DeviceName");
        dev.DeviceIndex = device.at("DeviceIndex");
        DeviceMessages devMsgs;
        devMessages_from_json(device, devMsgs);
        dev.DeviceMessages = devMsgs;
        dlm.Devices.push_back(dev);
    }
}

struct DeviceAdded {
    int Id;
    std::string DeviceName;
    int DeviceIndex;
    DeviceMessages DeviceMessages;
};

inline void from_json(const json& j, DeviceAdded& dlm) {
    logger::info("{}", j.dump());
    dlm.Id = j.at("Id");
    dlm.DeviceName = j.at("DeviceName");
    dlm.DeviceIndex = j.at("DeviceIndex");
    DeviceMessages devMessages;
    devMessages_from_json(j, devMessages);
    dlm.DeviceMessages = devMessages;
}

struct DeviceRemoved {
    int Id;
    int DeviceIndex;
};

inline void from_json(const json& j, DeviceRemoved& dr) {
    dr.Id = j.at("Id");
    dr.DeviceIndex = j.at("DeviceIndex");
}
/*Not yet implemented*/
struct Scalar {
    float ScalarVal;
    std::string ActuatorType;
};

struct ScalarCmd {
    int Id;
    int DeviceIndex;
    std::vector<Scalar> Scalars;
};

inline void to_json(json& j, const ScalarCmd& cmd) {
    json arry;
    json scalars;
    for (int i = 0; i < cmd.Scalars.size(); i++) {
        auto& scalar = cmd.Scalars[i];
        scalars.push_back(json{{"Index", i}, {"Scalar", scalar.ScalarVal}, {"ActuatorType", scalar.ActuatorType}});
    }
    arry.push_back(json{{"ScalarCmd", json{{"Id", cmd.Id}, {"DeviceIndex", cmd.DeviceIndex}, {"Scalars", scalars}}}});
    j = arry;
}

struct VibrateSpeed {
    float speed;    
};

struct VibrateCmd {
    int Id;
    int DeviceIndex;
    std::vector<VibrateSpeed> Speeds;
};

inline void to_json(json& j, const VibrateCmd& cmd) {
    
    json speeds;
    for (int i = 0; i < cmd.Speeds.size(); i++) {
        auto& speed = cmd.Speeds[i];
        speeds.push_back(json{ {"Index", i}, {"Speed",speed.speed} });
    }
    j.push_back(json{ {"VibrateCmd", json{{"Id", cmd.Id},{"DeviceIndex",cmd.DeviceIndex},{"Speeds",speeds}}} });    
}

struct StopDeviceCmd {
    int Id;
    int DeviceIndex;
};

inline void to_json(json& j, const StopDeviceCmd cmd) {
    j.push_back(json{ {"StopDeviceCmd",json{{"Id",cmd.Id},{"DeviceIndex",cmd.DeviceIndex}}} });
}