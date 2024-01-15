#pragma once
#include "ScriptGraph/ScriptGraphNode.h"

BeginScriptGraphNode(SGNode_GameObjectCreate)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "Create Gameobject"; }
	std::string GetDescription() const override { return "Creates a new Gameobject."; }
	std::string GetNodeCategory() const override { return "GameObject"; }
	size_t DoOperation() override;
	bool IsSimpleNode() const override { return false; }
};

BeginScriptGraphNode(SGNode_GameObjectGet)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "Get Gameobject"; }
	std::string GetDescription() const override { return "Manually gets a Gameobject."; }
	std::string GetNodeCategory() const override { return "GameObject"; }
	size_t DoOperation() override;
	bool IsSimpleNode() const override { return false; }
};

BeginScriptGraphNode(SGNode_GameObjectGetPos)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "Get Position"; }
	std::string GetDescription() const override { return "Gets an existing Gameobjects position."; }
	std::string GetNodeCategory() const override { return "GameObject"; }
	size_t DoOperation() override;
	bool IsSimpleNode() const override { return false; }
};

BeginScriptGraphNode(SGNode_GameObjectSetPos)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "Set Position"; }
	std::string GetDescription() const override { return "Sets an existing Gameobjects position."; }
	std::string GetNodeCategory() const override { return "GameObject"; }
	size_t DoOperation() override;
	bool IsSimpleNode() const override { return false; }
};