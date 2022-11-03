#pragma once
#include "Singleton.h"
#include "InterfaceSpec/IPluginInterface.h"
#include "WebSocketClient.h"

namespace OButtplug {
	class BPInterface final:
		public IBPInterface,
		public ISingleton<BPInterface>

	{		
		enum 
		{
			kCurrentPluginVersion = 1,
			kSerializationVersion = 0
		};
	public: // IPluginInterface
		virtual UINT32 GetVersion() override { return kCurrentPluginVersion; };
		virtual void Revert() override {};
	public: // IBPInterface
		void LogThing(int someData);
		void StartConnection();
		std::vector<std::string> FetchDevices();
	private:
		std::atomic<std::shared_ptr<session>> m_session;
	};
	
};