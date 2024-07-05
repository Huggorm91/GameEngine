#pragma once
#include "ScriptGraph/ScriptGraphNode.h"

DECLARE_GRAPH_NODE(SGNode_FloatGreater, ScriptGraphNode)
{
public:
	SGNode_FloatGreater();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Greater than"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Float"; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(SGNode_FloatLess, ScriptGraphNode)
{
public:
	SGNode_FloatLess();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Less than"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Float"; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(SGNode_FloatEqual, ScriptGraphNode)
{
public:
	SGNode_FloatEqual();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Equal"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Float"; }
	NodeResult DoOperation() override;
};