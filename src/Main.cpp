#pragma once

#include <BPInterface.h>
#include <Messaging/IMessages.h>
#include <Papyrus/Papyrus.h>
#include <OSA.h>
#include <stddef.h>

using namespace RE::BSScript;
using namespace SKSE;
using namespace SKSE::log;
using namespace SKSE::stl;

PluginHandle g_pluginHandle = kInvalidPluginHandle;

namespace {
    void InitializeLogging() {
        auto path = log_directory();
        if (!path) {
            report_and_fail("Unable to lookup SKSE logs directory.");
        }
        *path /= PluginDeclaration::GetSingleton()->GetName();
        *path += L".log";

        std::shared_ptr<spdlog::logger> log;
        if (IsDebuggerPresent()) {
            log = std::make_shared<spdlog::logger>("Global", std::make_shared<spdlog::sinks::msvc_sink_mt>());
        } else {
            log = std::make_shared<spdlog::logger>(
                "Global", std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true));
        }
        log->set_level(spdlog::level::info);
        log->flush_on(spdlog::level::info);

        spdlog::set_default_logger(std::move(log));
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [%t] [%s:%#] %v");
    }

    void UnSpecifiedSenderMessageHandler(SKSE::MessagingInterface::Message* message) {
        log::info("{} Message received", message->type);
        switch (message->type) {
            case Messaging::kAnimationChanged: {
                if (message->dataLen != sizeof(Messaging::AnimationChangedMessage)) {
                    logger::error("Invalid message received from {} - type {}", message->sender, message->type);
                }
                auto animChangedMessage = (Messaging::AnimationChangedMessage*)message->data;
                logger::info("Scene changed {}", animChangedMessage->newAnimation->scene_id);
            }break;
        }
    }
    
    void MessageHandler(SKSE::MessagingInterface::Message* a_msg) {
            logger::info("Received {} from {}", a_msg->type, a_msg->sender);
        switch (a_msg->type) {
            case SKSE::MessagingInterface::kPostLoad: {
                auto message = SKSE::GetMessagingInterface();
                if (message) {
                    logger::info("Registering listener for OSA");
                    message->RegisterListener(nullptr, UnSpecifiedSenderMessageHandler);
                }
            } break;
            case SKSE::MessagingInterface::kPostPostLoad: {
                logger::info("Received {} from {}", a_msg->type, a_msg->sender);
                OSA::InterfaceExchangeMessage msg;
                auto message = SKSE::GetMessagingInterface();
                message->Dispatch(OSA::InterfaceExchangeMessage::kExchangeInterface, (void*)&msg, sizeof(OSA::InterfaceExchangeMessage*), "OSA");
                if (!msg.interfaceMap) {
                    logger::critical("Couldn't get interface map!");
                    return;
                }

                auto osaMessagingRegistry = static_cast<OSA::IMessagingRegistry*>(msg.interfaceMap->QueryInterface("Messaging"));
                if (!osaMessagingRegistry) {
                    logger::critical("Couldn't get messaging registry");
                    break;
                } 
                osaMessagingRegistry->RegisterForMessages("OButtplug");
                OButtplug::BPInterface::GetSingleton()->StartConnection();
            } break;
        }
    }
}  // namespace

SKSEPluginLoad(const LoadInterface* skse) {
    InitializeLogging();

    auto* plugin = PluginDeclaration::GetSingleton();
    auto version = plugin->GetVersion();
    log::info("{} {} is loading...", plugin->GetName(), version);
    g_pluginHandle = skse->GetPluginHandle();
    Init(skse);

    auto message = SKSE::GetMessagingInterface();

    // message->RegisterListener(nullptr, InterfaceExchangeMessageHandler);
    if (!message->RegisterListener(MessageHandler)) {
        return false;
    }

    Papyrus::Bind();

    log::info("{} has finished loading.", plugin->GetName());
    return true;
}
