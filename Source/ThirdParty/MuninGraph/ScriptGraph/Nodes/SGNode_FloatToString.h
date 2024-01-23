#pragma once
#include "ScriptGraph/ScriptGraphNode.h"

BeginScriptGraphNode(SGNode_FloatToString)
{
public:

	void Init() override;
	std::string GetNodeTitle() const override { return "Float to String"; }
	std::string GetDescription() const override { return "Converts a float value to a text string."; }
	std::string GetNodeCategory() const override { return "Casts"; }
	size_t DoOperation() override;
	FORCEINLINE bool IsSimpleNode() const override { return true; }
};

BeginScriptGraphNode(SGNode_IntToString)
{
public:

	void Init() override;
	std::string GetNodeTitle() const override { return "Int to String"; }
	std::string GetDescription() const override { return "Converts an integer value to a text string."; }
	std::string GetNodeCategory() const override { return "Casts"; }
	size_t DoOperation() override;
	FORCEINLINE bool IsSimpleNode() const override { return true; }
};