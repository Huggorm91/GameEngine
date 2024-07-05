#pragma once
#include "ScriptGraph/ScriptGraphNode.h"

DECLARE_GRAPH_NODE(SGNode_Vector3Create, ScriptGraphNode)
{
public:
	SGNode_Vector3Create();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Create"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Vector3"; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(SGNode_Vector3Break, ScriptGraphNode)
{
public:
	SGNode_Vector3Break();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Break"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Vector3"; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(SGNode_Vector3Equal, ScriptGraphNode)
{
public:
	SGNode_Vector3Equal();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Equal"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Vector3"; }
	NodeResult DoOperation() override;
};