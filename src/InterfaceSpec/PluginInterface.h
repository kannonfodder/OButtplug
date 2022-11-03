#pragma once

#include "IPluginInterface.h"


struct SKSESerializationInterface;

class PluginInterface : public IPluginInterface
{
public:
	PluginInterface() { };
	virtual ~PluginInterface() { };

	virtual UINT32 GetVersion() override { return 0; };
	virtual void Revert() override { };
};

class InterfaceMap
	: public IInterfaceMap
	, public std::unordered_map<const char*, IPluginInterface*>
{
public:
	virtual IPluginInterface* QueryInterface(const char* name) override;
	virtual bool AddInterface(const char* name, IPluginInterface* pluginInterface) override;
	virtual IPluginInterface* RemoveInterface(const char* name) override;
};