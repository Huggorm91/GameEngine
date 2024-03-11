#pragma once
#include "Container/Blackboard.hpp"

class AnimationStateMachine;

class AnimationState
{
public:
	AnimationState() = default;
	virtual ~AnimationState() = default;

	virtual void Init(AnimationStateMachine* aStateMachine) = 0;
	virtual void Update() = 0;

	virtual void Enter() = 0;
	virtual void Exit() = 0;

	virtual bool CanExit() = 0;
private:
	// ScriptGraph
	Crimson::Blackboard<std::string> myVariables;
	std::string myName;
	AnimationStateMachine* myStateMachine;
};