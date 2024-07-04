#pragma once
#include "ScriptGraph/ScriptGraphNode.h"

DECLARE_GRAPH_NODE(SGNode_ForLoop, ScriptGraphNode)
{
public:
	SGNode_ForLoop();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "For"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Flow Control"; }

	NodeResult Enter(size_t anEntryPinId, NodeEntryType aEntryType) override;

private:
	int myIndex = 0;
	int myEnd = 0;
};