#pragma once
#include "ScriptGraph/ScriptGraphNode.h"

DECLARE_GRAPH_NODE(SGNode_ToStringBool, ScriptGraphNode)
{
public:
	SGNode_ToStringBool();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Bool"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "ToString"; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(SGNode_ToStringInt, ScriptGraphNode)
{
public:
	SGNode_ToStringInt();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Int"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "ToString"; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(SGNode_ToStringGameObjectID, ScriptGraphNode)
{
public:
	SGNode_ToStringGameObjectID();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "GameObject ID"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "ToString"; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(SGNode_ToStringFloat, ScriptGraphNode)
{
public:
	SGNode_ToStringFloat();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Float"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "ToString"; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(SGNode_ToStringVector3, ScriptGraphNode)
{
public:
	SGNode_ToStringVector3();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Vector3"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "ToString"; }
	NodeResult DoOperation() override;
};