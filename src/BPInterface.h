#pragma once
#include "Singleton.h"
#include "BpIo/WebSocketClient.h"
#include "Device.h"

namespace OButtplug {
	class BPInterface final:
		public ISingleton<BPInterface>
	{
	public: // IBPInterface
		void StartConnection();
		std::vector<std::string> FetchDevices();
		void WSMessageRecieved(std::string msg);
		void DeviceLookup();
		bool SaveDeviceInSlot(Device::Slot slot, std::string deviceName);
		void TestDevices();

		bool CheckConnection();

		std::shared_ptr<Device> getDeviceBySlot(Device::Slot slot);

		template<class T>
		inline void sendMessage(T message)
		{
			json j = message;
			m_session.load()->send(boost::make_shared<std::string>(j.dump()));
		}
	private:

		Device* getDeviceByName(const std::string& name);
				
		std::unordered_map<int, std::shared_ptr<Device>> connectedDevices; // Device Index / Device
		std::unordered_map<int, std::shared_ptr<Device>> selectedDevices; // 0 - penis, 1 - vagina, 2 - anal
		std::atomic<std::shared_ptr<session>> m_session;
	};
	
	

};