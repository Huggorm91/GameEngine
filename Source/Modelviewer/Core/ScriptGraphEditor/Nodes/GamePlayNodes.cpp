#include "ModelViewer.pch.h"
#include "GamePlayNodes.h"
#include "Core/Modelviewer.h"
#include "AssetManager\Assets\Components\Gameplay\HealthComponent.h"

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

void MVNode_GamePlayTakeDamage::Init()
{
	CreateExecPin("In", PinDirection::Input, true);
	CreateExecPin("Out", PinDirection::Output, true);

	CreateDataPin<GameObjectID>("ID", PinDirection::Input);
	CreateDataPin<int>("Damage", PinDirection::Input);
}

size_t MVNode_GamePlayTakeDamage::DoOperation()
{
	GameObjectID id = GameObjectID::Invalid;
	int damage = 0;

	if (GetPinData("ID", id) && GetPinData("Damage", damage))
	{
		auto object = ModelViewer::Get().GetGameObject(id);
		if (object && object->HasComponent<HealthComponent>())
		{
			object->GetComponent<HealthComponent>().TakeDamage(damage);
		}
		return ExitViaPin("Out");
	}

	return ExitWithError("Invalid input!");
}