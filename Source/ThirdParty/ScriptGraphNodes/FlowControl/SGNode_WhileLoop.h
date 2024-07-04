#pragma once
#include "ScriptGraph/ScriptGraphNode.h"

DECLARE_GRAPH_NODE(SGNode_WhileLoop, ScriptGraphNode)
{
public:
	SGNode_WhileLoop();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "While"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Flow Control"; }

	NodeResult Enter(size_t anEntryPinId, NodeEntryType aEntryType) override;

private:

	int myIndex = 0;
};