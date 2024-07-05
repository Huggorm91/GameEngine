#include "pch.h"
#include "SGNode_CollisionEvents.h"

IMPLEMENT_GRAPH_NODE(SGNode_OnCollisionEnter, ScriptGraphEventNode);

SGNode_OnCollisionEnter::SGNode_OnCollisionEnter()
{
	CreateDataPin<unsigned>("Collider ID", PinDirection::Output);
}

IMPLEMENT_GRAPH_NODE(SGNode_OnCollisionStay, ScriptGraphEventNode);

SGNode_OnCollisionStay::SGNode_OnCollisionStay()
{
	CreateDataPin<unsigned>("Collider ID", PinDirection::Output);
}

IMPLEMENT_GRAPH_NODE(SGNode_OnCollisionExit, ScriptGraphEventNode);

SGNode_OnCollisionExit::SGNode_OnCollisionExit()
{
	CreateDataPin<unsigned>("Collider ID", PinDirection::Output);
}

IMPLEMENT_GRAPH_NODE(SGNode_OnTriggerEnter, ScriptGraphEventNode);

SGNode_OnTriggerEnter::SGNode_OnTriggerEnter()
{
	CreateDataPin<unsigned>("Trigger ID", PinDirection::Output);
}

IMPLEMENT_GRAPH_NODE(SGNode_OnTriggerStay, ScriptGraphEventNode);

SGNode_OnTriggerStay::SGNode_OnTriggerStay()
{
	CreateDataPin<unsigned>("Trigger ID", PinDirection::Output);
}

IMPLEMENT_GRAPH_NODE(SGNode_OnTriggerExit, ScriptGraphEventNode);

SGNode_OnTriggerExit::SGNode_OnTriggerExit()
{
	CreateDataPin<unsigned>("Trigger ID", PinDirection::Output);
}