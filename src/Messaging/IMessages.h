#pragma once

namespace OStim {
    using ThreadId = int64_t;
    
}  // namespace Ostim

namespace Messaging {
    enum messageId : uint32_t { kAnimationChanged = 0x4e9673 };

    struct AnimationChangedMessage {
        uint32_t messageId;
        OStim::ThreadId threadId;
        std::string sceneId;
    };
}  // namespace Messaging