#include "ModelViewer.pch.h"
#include "GamePlayNodes.h"

void MVNode_EventDamageTaken::Init()
{
	CreateExecPin("Out", PinDirection::Output, true);
	CreateDataPin<GameObjectID>("Object ID", PinDirection::Output);
}

void MVNode_EventObjectDied::Init()
{
	CreateExecPin("Out", PinDirection::Output, true);
	CreateDataPin<GameObjectID>("Object ID", PinDirection::Output);
}