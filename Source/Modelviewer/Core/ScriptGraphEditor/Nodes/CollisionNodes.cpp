#include "Modelviewer.pch.h"
#include "CollisionNodes.h"

void MVNode_EventOnTriggerEnter::Init()
{
	CreateExecPin("Out", PinDirection::Output, true);
	CreateDataPin<GameObjectID>("Trigger ID", PinDirection::Output);
	CreateDataPin<GameObjectID>("Collider ID", PinDirection::Output);
}

void MVNode_EventOnTriggerExit::Init()
{
	CreateExecPin("Out", PinDirection::Output, true);
	CreateDataPin<GameObjectID>("Trigger ID", PinDirection::Output);
	CreateDataPin<GameObjectID>("Collider ID", PinDirection::Output);
}

void MVNode_EventOnTriggerStay::Init()
{
	CreateExecPin("Out", PinDirection::Output, true);
	CreateDataPin<GameObjectID>("Trigger ID", PinDirection::Output);
	CreateDataPin<GameObjectID>("Collider ID", PinDirection::Output);
}

void MVNode_EventOnCollisionEnter::Init()
{
	CreateExecPin("Out", PinDirection::Output, true);
	CreateDataPin<GameObjectID>("Collider1 ID", PinDirection::Output);
	CreateDataPin<GameObjectID>("Collider2 ID", PinDirection::Output);
}

void MVNode_EventOnCollisionExit::Init()
{
	CreateExecPin("Out", PinDirection::Output, true);
	CreateDataPin<GameObjectID>("Collider1 ID", PinDirection::Output);
	CreateDataPin<GameObjectID>("Collider2 ID", PinDirection::Output);
}

void MVNode_EventOnCollisionStay::Init()
{
	CreateExecPin("Out", PinDirection::Output, true);
	CreateDataPin<GameObjectID>("Collider1 ID", PinDirection::Output);
	CreateDataPin<GameObjectID>("Collider2 ID", PinDirection::Output);
}