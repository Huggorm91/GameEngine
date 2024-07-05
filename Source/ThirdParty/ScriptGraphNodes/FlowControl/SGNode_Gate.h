#pragma once
#include "ScriptGraph/ScriptGraphNode.h"

DECLARE_GRAPH_NODE(SGNode_Gate, ScriptGraphNode)
{
public:
	SGNode_Gate();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Gate"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Flow Control"; }

	NodeResult Enter(size_t anEntryPinId, NodeEntryType aEntryType) override;
	void Reset() override;

private:
	bool myIsOpen = true;
};