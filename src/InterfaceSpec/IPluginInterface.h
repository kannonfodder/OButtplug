#pragma once

class IPluginInterface
{
public:
	IPluginInterface() { };
	virtual ~IPluginInterface() { };

	virtual UINT32 GetVersion() = 0;
	virtual void Revert() = 0;
};

class IInterfaceMap
{
public:
	virtual IPluginInterface* QueryInterface(const char* name) = 0;
	virtual bool AddInterface(const char* name, IPluginInterface* pluginInterface) = 0;
	virtual IPluginInterface* RemoveInterface(const char* name) = 0;
};

struct InterfaceExchangeMessage
{
	enum: uint32_t
	{
		kMessage_ExchangeInterface = 'OBP'
	};

	IInterfaceMap* interfaceMap = NULL;
};

class IBPInterface : public IPluginInterface
{
public:
	virtual void LogThing(int someData) = 0;
	virtual void StartConnection() = 0;
	virtual std::vector<std::string> FetchDevices() = 0;
};