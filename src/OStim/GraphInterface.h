#pragma once

namespace OStim {
		

	enum FurnitureType {
		NONE = 0,
		BED = 1,
		BENCH = 2,
		CHAIR = 3,
		TABLE = 4,
		SHELF = 5,
		WALL = 6,
		COOKING_POT = 7,
	};

	enum ExpressionType {
		EXPRESSION = 1 << 0,
		PHONEME = 1 << 1,
		LID_MODIFIER = 1 << 2,
		BROW_MODIFIER = 1 << 3,
		BALL_MODIFIER = 1 << 4
	};

	struct FaceModifier {
	public:
		int type = 0;
		float baseValue = 0;
		float speedMultiplier = 0;
		float excitementMultiplier = 0;
		int delay = 0;
		int delayVariance = 0;
	};

	struct ActionActor {
	public:
		enum Requirement {
			NONE = 0,
			ANUS = 1 << 0,
			BREAST = 1 << 1,
			FOOT = 1 << 2,
			HAND = 1 << 3,
			MOUTH = 1 << 4,
			NIPPLE = 1 << 5,
			PENIS = 1 << 6,
			TESTICLES = 1 << 7,
			VAGINA = 1 << 8,
		};
	public:
		float stimulation = 0.0;
		float maxStimulation = 100.0;
		bool fullStrip = false;
		uint32_t requirements = 0;
		uint32_t strippingMask = 0;
		std::unordered_map<std::string, int> ints;
		std::unordered_map<std::string, std::vector<int>> intLists;
		std::unordered_map<std::string, float> floats;
		std::unordered_map<std::string, std::vector<float>> floatLists;
		std::unordered_map<std::string, std::string> strings;
		std::unordered_map<std::string, std::vector<std::string>> stringLists;
	};

	struct ActionAttributes {
	public:
		ActionActor actor;
		ActionActor target;
		ActionActor performer;
		std::vector<std::string> tags;

		bool hasTag(std::string tag);
	};

	struct Action {
	public:
		std::string type;
		ActionAttributes attributes;
		int actor;
		int target;
		int performer;

	};
	struct Speed {
	public:
		std::string animation;
		float playbackSpeed = 1.0;
	};

	struct ActorConditions {};

	struct Actor {
	public:
		uint32_t requirements;
		int penisAngle;
		float scale = 1.0;
		float scaleHeight = 120.748;
		bool feetOnGround;
		int expressionAction = -1;
		std::unordered_map<int, FaceModifier> eyeballModifierOverride;
		std::vector<std::string> tags;
		ActorConditions conditions;
		std::unordered_map<std::string, std::string> autotransitions;
	};

	struct Node {
	public:
		std::string scene_id;
		std::string lowercase_id;
		uint32_t numActors;
		std::string scene_name;
		std::vector<Speed> speeds;
		uint32_t defaultSpeed = 0;
		bool isTransition = false;
		bool isHub = false;
		bool isAggresive = false;
		bool hasIdleSpeed = false;
		bool noRandomSelection = false;
		FurnitureType furnitureType;
		std::vector<std::string> tags;
		std::vector<Actor> actors;
		std::vector<Action> actions;

		// maybe remove this in a later iteration?
		std::string sourceModule;
		std::string animClass;
	};
}