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

BeginScriptGraphNode(MVNode_GamePlayTakeDamage)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "Give Damage"; }
	std::string GetDescription() const override { return "Applies a set amount of damage to an object."; }
	std::string GetNodeCategory() const override { return "GamePlay"; }
	size_t DoOperation() override;
	bool IsSimpleNode() const override { return true; }
};