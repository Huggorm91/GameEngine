#pragma once
#include "ScriptGraph/Nodes/EventNodes.h"

DECLARE_GRAPH_NODE(SGNode_OnCollisionEnter, ScriptGraphEventNode)
{
public:
	SGNode_OnCollisionEnter();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "On Collision Enter"; }
};

DECLARE_GRAPH_NODE(SGNode_OnCollisionStay, ScriptGraphEventNode)
{
public:
	SGNode_OnCollisionStay();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "On Collision Stay"; }
};

DECLARE_GRAPH_NODE(SGNode_OnCollisionExit, ScriptGraphEventNode)
{
public:
	SGNode_OnCollisionExit();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "On Collision Exit"; }
};

DECLARE_GRAPH_NODE(SGNode_OnTriggerEnter, ScriptGraphEventNode)
{
public:
	SGNode_OnTriggerEnter();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "On Trigger Enter"; }
};

DECLARE_GRAPH_NODE(SGNode_OnTriggerStay, ScriptGraphEventNode)
{
public:
	SGNode_OnTriggerStay();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "On Trigger Stay"; }
};

DECLARE_GRAPH_NODE(SGNode_OnTriggerExit, ScriptGraphEventNode)
{
public:
	SGNode_OnTriggerExit();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "On Trigger Exit"; }
};