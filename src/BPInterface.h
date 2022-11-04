#pragma once
#include "Singleton.h"
#include "BpIo/WebSocketClient.h"

namespace OButtplug {
	class BPInterface final:
		public ISingleton<BPInterface>
	{
	public: // IBPInterface
		void StartConnection();
		std::vector<std::string> FetchDevices();
		void WSMessageRecieved(std::string msg);
		void DeviceLookup();
	private:
		std::unordered_map<int, std::string> deviceMap;
		std::atomic<std::shared_ptr<session>> m_session;
	};
	
};