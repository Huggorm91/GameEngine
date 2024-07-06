#pragma once
#include "ScriptGraph/ScriptGraphNode.h"

DECLARE_GRAPH_NODE(SGNode_Vector3Add, ScriptGraphNode)
{
public:
	SGNode_Vector3Add();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Add"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Vector3"; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(SGNode_Vector3Subtract, ScriptGraphNode)
{
public:
	SGNode_Vector3Subtract();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Subtract"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Vector3"; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(SGNode_Vector3Multiplication, ScriptGraphNode)
{
public:
	SGNode_Vector3Multiplication();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Multiplication"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Vector3"; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(SGNode_Vector3Division, ScriptGraphNode)
{
public:
	SGNode_Vector3Division();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Division"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Vector3"; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(SGNode_Vector3Abs, ScriptGraphNode)
{
public:
	SGNode_Vector3Abs();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Abs"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Vector3"; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(SGNode_Vector3Clamp, ScriptGraphNode)
{
public:
	SGNode_Vector3Clamp();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Clamp"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Vector3"; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(SGNode_Vector3ClampMagnitude, ScriptGraphNode)
{
public:
	SGNode_Vector3ClampMagnitude();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Clamp Magnitude"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Vector3"; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(SGNode_Vector3Lerp, ScriptGraphNode)
{
public:
	SGNode_Vector3Lerp();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Lerp"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Vector3"; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(SGNode_Vector3Distance, ScriptGraphNode)
{
public:
	SGNode_Vector3Distance();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Distance"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Vector3"; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(SGNode_Vector3Dot, ScriptGraphNode)
{
public:
	SGNode_Vector3Dot();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Dot"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Vector3"; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(SGNode_Vector3Cross, ScriptGraphNode)
{
public:
	SGNode_Vector3Cross();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Cross"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Vector3"; }
	NodeResult DoOperation() override;
};