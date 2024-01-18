#pragma once
#include "ScriptGraph/ScriptGraphNode.h"

BeginScriptGraphNode(MVNode_IfElse)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "If"; }
	std::string GetDescription() const override { return "Runs one of 2 paths depending on condition."; }
	std::string GetNodeCategory() const override { return "Flow"; }
	size_t DoOperation() override;
};

BeginScriptGraphNode(MVNode_ForLoop)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "For"; }
	std::string GetDescription() const override { return "Runs a path a set number of times then executes the 'Completed' path."; }
	std::string GetNodeCategory() const override { return "Flow"; }
	size_t DoOperation() override;
};

BeginScriptGraphNode(MVNode_Sequence)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "Sequence"; }
	std::string GetDescription() const override { return "Runs the paths in order."; }
	std::string GetNodeCategory() const override { return "Flow"; }
	size_t DoOperation() override;
};

BeginScriptGraphNode(MVNode_Gate)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "Gate"; }
	std::string GetDescription() const override { return "Will only run the True path once."; }
	std::string GetNodeCategory() const override { return "Flow"; }
	size_t DoOperation() override;

private:
	bool myHasActivated = false;
};