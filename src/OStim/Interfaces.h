#pragma once

#include "OStim.h"

namespace OStim {
	class ThreadInterface {
	public:
		inline static IThreadInterface* getOStimThreadInterface() { return ostimThreadInterface; }
		inline static void setOStimThreadInterface(IThreadInterface* ptr) { ostimThreadInterface = ptr; }
	private:
		inline static IThreadInterface* ostimThreadInterface;
	};
	class GraphInterface {
	public:
		inline static IGraphInterface* getOStimGraphInterface() { return ostimThreadInterface; }
		inline static void setOStimGraphInterface(IGraphInterface* ptr) { ostimThreadInterface = ptr; }
	private:
		inline static IGraphInterface* ostimThreadInterface;

	};
}
