#pragma once

namespace OSA {

    class IPluginInterface {


    public:
        enum : uint32_t
        {
            kInterfaceVersion0 = 0,
        };
        IPluginInterface() {};
        virtual ~IPluginInterface() {};

        virtual const char* GetInterfaceName() = 0;
        virtual uint32_t GetInterfaceVersion() = 0;
    };

    class IInterfaceMap {
    public:
        virtual IPluginInterface* QueryInterface(const char* name) = 0;
        virtual bool AddInterface(const char* name, IPluginInterface* pluginInterface) = 0;
        virtual IPluginInterface* RemoveInterface(const char* name) = 0;
    };

    struct InterfaceExchangeMessage {
        enum : uint32_t { kExchangeInterface = 'OSA' };

        IInterfaceMap* interfaceMap = nullptr;
    };

    class IMessagingRegistry : public IPluginInterface
    {
    public:
        virtual void RegisterForMessages(std::string newListener) = 0;
    };
}