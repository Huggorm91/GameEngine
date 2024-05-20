#pragma once
#include "ScriptGraph/ScriptGraphNode.h"

DECLARE_GRAPH_NODE(SGNode_MathAdd, ScriptGraphNode)
{
public:
	SGNode_MathAdd();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Add"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Math"; }
	NodeResult DoOperation() override;
};