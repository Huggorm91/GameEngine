#pragma once
#include "AnimationStateBase.h"

class AnimationStateMachine
{
public:
	AnimationStateMachine() = default;
	~AnimationStateMachine() = default;

	void Update();

	void PushState(const std::string& aStateName);
	void PopState();

private:
	struct Command
	{
		enum Type
		{
			None,
			Push,
			Pop
		} myAction;
		std::string myState;
	};

	std::vector<AnimationState*> myStateStack;
	std::unordered_map<std::string, std::shared_ptr<AnimationState>> myCachedStates;
	std::vector<Command> myCommands;
};