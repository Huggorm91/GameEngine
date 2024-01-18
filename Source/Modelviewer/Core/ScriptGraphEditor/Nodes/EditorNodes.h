#pragma once
#include "ScriptGraph/ScriptGraphNode.h"

BeginScriptGraphNode(MVNode_EditorGetCameraPos)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "Get Camera Position"; }
	std::string GetDescription() const override { return "Get the position of the editor camera."; }
	std::string GetNodeCategory() const override { return "Editor"; }
	size_t DoOperation() override;
	bool IsSimpleNode() const override { return false; }
};

BeginScriptGraphNode(MVNode_EditorGetCreatedObject)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "Get Created Object"; }
	std::string GetDescription() const override { return "Get a created object using an index."; }
	std::string GetNodeCategory() const override { return "Editor"; }
	size_t DoOperation() override;
	bool IsSimpleNode() const override { return false; }
};

BeginScriptGraphNode(MVNode_BuildString)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "Build String"; }
	std::string GetDescription() const override { return "Merges 2 strings into 1."; }
	std::string GetNodeCategory() const override { return "Casts"; }
	size_t DoOperation() override;
	bool IsSimpleNode() const override { return true; }
};