#pragma once
#include "ThirdParty/MuninGraph/ScriptGraph/ScriptGraphNode.h"

DECLARE_GRAPH_NODE(AMNode_GameObjectGetThis, ScriptGraphNode)
{
public:
	AMNode_GameObjectGetThis();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Get from Owner"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "GameObject"; }
	FORCEINLINE bool ShouldRenderNodeHeader() const override { return true; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(AMNode_GameObjectCreate, ScriptGraphNode)
{
public:
	AMNode_GameObjectCreate();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Create"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "GameObject"; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(AMNode_GameObjectMove, ScriptGraphNode)
{
public:
	AMNode_GameObjectMove();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Move"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "GameObject"; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(AMNode_GameObjectGetPosition, ScriptGraphNode)
{
public:
	AMNode_GameObjectGetPosition();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Get Position"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "GameObject"; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(AMNode_GameObjectSetPosition, ScriptGraphNode)
{
public:
	AMNode_GameObjectSetPosition();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Set Position"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "GameObject"; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(AMNode_GameObjectGetRotation, ScriptGraphNode)
{
public:
	AMNode_GameObjectGetRotation();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Get Rotation"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "GameObject"; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(AMNode_GameObjectSetRotation, ScriptGraphNode)
{
public:
	AMNode_GameObjectSetRotation();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Set Rotation"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "GameObject"; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(AMNode_GameObjectGetScale, ScriptGraphNode)
{
public:
	AMNode_GameObjectGetScale();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Get Scale"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "GameObject"; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(AMNode_GameObjectSetScale, ScriptGraphNode)
{
public:
	AMNode_GameObjectSetScale();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "Set Scale"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "GameObject"; }
	NodeResult DoOperation() override;
};

DECLARE_GRAPH_NODE(AMNode_GameObjectIDToString, ScriptGraphNode)
{
public:
	AMNode_GameObjectIDToString();

	FORCEINLINE std::string_view GetNodeTitle() const override { return "GameObject ID To String"; }
	FORCEINLINE std::string_view GetNodeCategory() const override { return "Casts"; }
	NodeResult DoOperation() override;
};