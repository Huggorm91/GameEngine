#pragma once
#include "ScriptGraph/Nodes/Events/SGNode_EventBase.h"

BeginScriptGraphDerivedNode(MVNode_EventOnTriggerEnter, SGNode_EventBase)
{
public:
	void Init() override;

	FORCEINLINE bool IsInternalOnly() const override { return false; }
	std::string GetNodeTitle() const override { return "On Trigger Enter"; }
	std::string GetDescription() const override { return "An event node that fires when the object enters a trigger."; }
};

BeginScriptGraphDerivedNode(MVNode_EventOnTriggerExit, SGNode_EventBase)
{
public:
	void Init() override;

	FORCEINLINE bool IsInternalOnly() const override { return false; }
	std::string GetNodeTitle() const override { return "On Trigger Exit"; }
	std::string GetDescription() const override { return "An event node that fires when the object exits a trigger."; }
};

BeginScriptGraphDerivedNode(MVNode_EventOnTriggerStay, SGNode_EventBase)
{
public:
	void Init() override;

	FORCEINLINE bool IsInternalOnly() const override { return false; }
	std::string GetNodeTitle() const override { return "On Trigger Stay"; }
	std::string GetDescription() const override { return "An event node that fires every frame the object is inside a trigger."; }
};

BeginScriptGraphDerivedNode(MVNode_EventOnCollisionEnter, SGNode_EventBase)
{
public:
	void Init() override;

	FORCEINLINE bool IsInternalOnly() const override { return false; }
	std::string GetNodeTitle() const override { return "On Collision Enter"; }
	std::string GetDescription() const override { return "An event node that fires when the object starts colliding."; }
};

BeginScriptGraphDerivedNode(MVNode_EventOnCollisionExit, SGNode_EventBase)
{
public:
	void Init() override;

	FORCEINLINE bool IsInternalOnly() const override { return false; }
	std::string GetNodeTitle() const override { return "On Collision Exit"; }
	std::string GetDescription() const override { return "An event node that fires when the object stops colliding."; }
};

BeginScriptGraphDerivedNode(MVNode_EventOnCollisionStay, SGNode_EventBase)
{
public:
	void Init() override;

	FORCEINLINE bool IsInternalOnly() const override { return false; }
	std::string GetNodeTitle() const override { return "On Collision Stay"; }
	std::string GetDescription() const override { return "An event node that fires every frame the object is colliding."; }
};