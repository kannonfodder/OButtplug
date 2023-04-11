#pragma once

#include "AnimationChangedHandler.h"
#include "BPInterface.h"
#include "Ostim/Interfaces.h"

namespace OButtplug {

    void AnimationChangedHandler::handle(Messaging::AnimationChangedMessage* msg) {
        auto threadInterface = OStim::ThreadInterface::getOStimThreadInterface();
        auto graphInterface = OStim::GraphInterface::getOStimGraphInterface();
        if (threadInterface != nullptr)
        {
            
            auto actors = threadInterface->getActors(msg->threadId);
            int playerIndex = -1;
            for (int i = 0; i < actors.size(); i++) {
                if (actors[i]->IsPlayer() || actors[i]->IsPlayerRef())
                    playerIndex = i;
            }
            if (playerIndex == -1) {
                return;
            }
            auto node = graphInterface->getNode(msg->sceneId);
            for (auto& action : node->actions) {
                if (action.actor == playerIndex) {
                    if ((action.attributes.actor.requirements & OStim::ActionActor::Requirement::PENIS) == OStim::ActionActor::Requirement::PENIS) {
                        logger::info("animation changed to player penis");
                    }
                    if ((action.attributes.actor.requirements & OStim::ActionActor::Requirement::VAGINA) == OStim::ActionActor::Requirement::VAGINA) {
                        logger::info("animation changed to player vagina");
                    }
                    if ((action.attributes.actor.requirements & OStim::ActionActor::Requirement::ANUS) == OStim::ActionActor::Requirement::ANUS) {
                        logger::info("animation changed to player anal");
                    }
                }
                if (action.target == playerIndex) {
                    if((action.attributes.target.requirements & OStim::ActionActor::Requirement::PENIS) == OStim::ActionActor::Requirement::PENIS){
                        logger::info("animation changed to player penis");
                    }
                    if((action.attributes.target.requirements & OStim::ActionActor::Requirement::VAGINA) == OStim::ActionActor::Requirement::VAGINA){
                        logger::info("animation changed to player vagina");
                    }
                    if ((action.attributes.target.requirements & OStim::ActionActor::Requirement::ANUS) == OStim::ActionActor::Requirement::ANUS) {
                        logger::info("animation changed to player anal");
                    }
                }
            }
        }
        if (auto device = OButtplug::BPInterface::GetSingleton()->getDeviceBySlot(Device::Slot::Penis); device != nullptr) {
            device->vibrateAndStop(1.0, 1000);            
        }
    }
}  // namespace OButtplug