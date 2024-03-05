#pragma once

class AnimationState
{
public:
	AnimationState() = default;
	virtual ~AnimationState() = default;

	virtual void Init() = 0;
	virtual void Update() = 0;

	virtual void Enter() = 0;
	virtual void Exit() = 0;

	virtual bool CanExit() = 0;
private:
	// ScriptGraph
	// BlackBoard for variables
	std::string myName;
};