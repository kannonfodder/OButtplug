#pragma once
struct Action {
public:
    std::string type;
    uint32_t actor;
    uint32_t target;
    uint32_t performer;
};

struct Actor {
public:
    std::string position;
    int32_t penisAngle;
    float scale = 1.0;
    float scaleHeight = 120.748;
    bool feetOnGround;
    std::vector<std::string> tags;
};

namespace Ostim {
    struct Node {
        std::string scene_id;
        std::vector<std::string> tags;
        std::vector<Actor*> actors;
        std::vector<Action*> actions;
    };
}  // namespace Ostim

namespace Messaging {
    enum messageId : uint32_t { kAnimationChanged = 0x4e9673 };

    struct AnimationChangedMessage {
        uint32_t messageId;
        Ostim::Node* newAnimation = nullptr;
    };
}  // namespace Messaging