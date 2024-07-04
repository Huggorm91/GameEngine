#pragma once
#include "ScriptGraph/ScriptGraphNode.h"

DECLARE_GRAPH_NODE(SGNode_Not, ScriptGraphNode)
{
public:
	SGNode_Not();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Not"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Logic"; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(SGNode_And, ScriptGraphNode)
{
public:
	SGNode_And();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "And"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Logic"; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(SGNode_Or, ScriptGraphNode)
{
public:
	SGNode_Or();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Or"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Logic"; }
	NodeResult DoOperation() override;
};