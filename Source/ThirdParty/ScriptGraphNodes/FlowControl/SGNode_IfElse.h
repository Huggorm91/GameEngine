#pragma once
#include "ScriptGraph/ScriptGraphNode.h"

DECLARE_GRAPH_NODE(SGNode_IfElse, ScriptGraphNode)
{
public:
	SGNode_IfElse();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "If"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Flow Control"; }

	NodeResult DoOperation() override;
};