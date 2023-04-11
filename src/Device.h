#pragma once

#include "BpIo/Messages.h"

namespace OButtplug {
	class Device {	
	public:
	enum Slot {
			Penis = 0,
			Vaginal = 1,
			Anal = 2
		};
	public:
		Device(DeviceListDevice dev);
		Device(DeviceAdded dev);
		int deviceIndex;
		std::string deviceName;

		
		void vibrateAndStop(float power, int timeMs);
		void vibrate(float power);
		void stop();
	private:

		DeviceMessages deviceMessages;
	};
};