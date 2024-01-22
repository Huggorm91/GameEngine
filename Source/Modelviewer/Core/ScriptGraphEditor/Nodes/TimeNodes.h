#pragma once
#include "ScriptGraph/ScriptGraphNode.h"
#include "PostMaster/Observer.h"

BeginScriptGraphNode(MVNode_Timer), public Crimson::Observer
{
public:
	MVNode_Timer() : myID(++ourIDCount), myTriggerCount(0), myStopCount(0) {}
	void Init() override;
	std::string GetNodeTitle() const override { return "Timer"; }
	std::string GetDescription() const override { return "Starts a path after a set duration."; }
	std::string GetNodeCategory() const override { return "Time"; }
	size_t DoOperation() override;

	void RecieveMessage(const Crimson::Message& aMessage) override;

private:
	static int ourIDCount;

	const int myID;
	int myTriggerCount;
	int myStopCount;

};

BeginScriptGraphNode(MVNode_TimerStop)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "Stop Timer"; }
	std::string GetDescription() const override { return "Stops execution of a timer."; }
	std::string GetNodeCategory() const override { return "Time"; }
	size_t DoOperation() override;
};