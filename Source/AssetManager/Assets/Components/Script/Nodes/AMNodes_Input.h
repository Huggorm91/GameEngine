#pragma once
#include "ThirdParty/MuninGraph/ScriptGraph/ScriptGraphNode.h"

DECLARE_GRAPH_NODE(AMNode_GetKeyDown, ScriptGraphNode)
{
public:
	AMNode_GetKeyDown();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Key Down"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Input"; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(AMNode_GetKeyHeld, ScriptGraphNode)
{
public:
	AMNode_GetKeyHeld();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Key Held"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Input"; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(AMNode_GetKeyUp, ScriptGraphNode)
{
public:
	AMNode_GetKeyUp();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Key Up"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Input"; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(AMNode_GetKeyDownOrHeld, ScriptGraphNode)
{
public:
	AMNode_GetKeyDownOrHeld();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Key Down or Held"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Input"; }
	NodeResult DoOperation() override;
};