#include "AssetManager.pch.h"
#include "ScriptComponent.h"
#include "Time/Timer.h"
#include "ThirdParty/MuninGraph/MuninScriptGraph.h"
#include "ThirdParty/MuninGraph/ScriptGraph/Blueprint.h"

ScriptComponent::ScriptComponent() : Component(ComponentType::Script)
{}

ScriptComponent::ScriptComponent(const Json::Value & aJson) : Component(aJson)//, myScript(std::make_shared<Blueprint>(AssetManager::GetAsset<Blueprint>(aJson["ScriptPath"])))
{}

void ScriptComponent::Update()
{
	if (!myIsActive)
	{
		return;
	}

	myScript->Tick(Crimson::Timer::GetDeltaTime());
}

void ScriptComponent::CreateImGuiComponents(const std::string& aWindowName)
{
	Component::CreateImGuiComponents(aWindowName);
}

Json::Value ScriptComponent::ToJson() const
{
	auto result = Component::ToJson();
	result["ScriptPath"] = myScript->GetPath();
	return result;
}

void ScriptComponent::Serialize(std::ostream& aStream) const
{
	Component::Serialize(aStream);
}

void ScriptComponent::Deserialize(std::istream& aStream)
{
	Component::Deserialize(aStream);
}

inline std::string ScriptComponent::ToString() const
{
	return "ScriptComponent";
}

inline const ScriptComponent* ScriptComponent::GetTypePointer() const
{
	return this;
}
