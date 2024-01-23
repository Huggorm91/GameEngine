#pragma once
#include "ScriptGraph/Nodes/Events/SGNode_EventBase.h"

BeginScriptGraphDerivedNode(MVNode_EventDamageTaken, SGNode_EventBase)
{
public:
	void Init() override;

	FORCEINLINE bool IsInternalOnly() const override { return false; }
	std::string GetNodeTitle() const override { return "Damage Taken"; }
	std::string GetDescription() const override { return "An event node that fires when the object takes damage."; }
};

BeginScriptGraphDerivedNode(MVNode_EventObjectDied, SGNode_EventBase)
{
public:
	void Init() override;

	FORCEINLINE bool IsInternalOnly() const override { return false; }
	std::string GetNodeTitle() const override { return "Object Died"; }
	std::string GetDescription() const override { return "An event node that fires when the object has taken lethal damage."; }
};