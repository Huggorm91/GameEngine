#pragma once
#include "ScriptGraph/ScriptGraphNode.h"

DECLARE_GRAPH_NODE(SGNode_FloatAdd, ScriptGraphNode)
{
public:
	SGNode_FloatAdd();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Add"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Float"; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(SGNode_FloatSubtract, ScriptGraphNode)
{
public:
	SGNode_FloatSubtract();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Subtract"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Float"; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(SGNode_FloatMultiplication, ScriptGraphNode)
{
public:
	SGNode_FloatMultiplication();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Multiplication"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Float"; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(SGNode_FloatDivision, ScriptGraphNode)
{
public:
	SGNode_FloatDivision();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Division"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Float"; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(SGNode_FloatCos, ScriptGraphNode)
{
public:
	SGNode_FloatCos();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Cos"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Float"; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(SGNode_FloatSin, ScriptGraphNode)
{
public:
	SGNode_FloatSin();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Sin"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Float"; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(SGNode_FloatAbs, ScriptGraphNode)
{
public:
	SGNode_FloatAbs();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Abs"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Float"; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(SGNode_FloatATan, ScriptGraphNode)
{
public:
	SGNode_FloatATan();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "ATan"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Float"; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(SGNode_FloatATan2, ScriptGraphNode)
{
public:
	SGNode_FloatATan2();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "ATan2"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Float"; }
	NodeResult DoOperation() override;
};