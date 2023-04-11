#pragma once

#include "Singleton.h"
#include "Messaging/IMessages.h"

namespace OButtplug {
	class AnimationChangedHandler final : public ISingleton<AnimationChangedHandler> {


	public:
		void handle(Messaging::AnimationChangedMessage*);
	};
}