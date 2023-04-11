#pragma once

#include <OStim/GraphInterface.h>

namespace OStim {

    class IPluginInterface {
    public:
        enum : uint32_t
        {
            kInterfaceVersion0 = 0,
        };
        IPluginInterface() { };
        virtual ~IPluginInterface() { };

        virtual UINT32 GetVersion() = 0;
        virtual void Revert() = 0;
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

    class IGraphInterface : public IPluginInterface {
    public:
        virtual OStim::Node* getNode(std::string sceneId) = 0;
    };
    
    class IThreadInterface : public IPluginInterface {
    public:
        virtual std::vector<RE::Actor*> getActors(int64_t threadId) = 0;
    };
}