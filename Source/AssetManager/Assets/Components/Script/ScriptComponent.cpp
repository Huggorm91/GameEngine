#include "AssetManager.pch.h"
#include "ScriptComponent.h"
#include "AssetManager.h"

ScriptComponent::ScriptComponent() : Component(ComponentType::Script), myScriptGraph(nullptr)
{}

ScriptComponent::ScriptComponent(const Json::Value& aJson) : Component(aJson), myScriptGraph(nullptr), myScriptPath(aJson["ScriptPath"].asString())
{}

void ScriptComponent::Update()
{
	if (!myScriptGraph || !myIsActive)
	{
		return;
	}

	myScriptGraph->Tick(Crimson::Timer::GetDeltaTime());
}

void ScriptComponent::Init(GameObject* aParent)
{
	Component::Init(aParent);
	myScriptGraph = std::make_shared<ScriptGraph>(ScriptGraph(myParent));
	
	if (!myScriptPath.empty())
	{
		if (Script::ScriptData* data = AssetManager::GetAsset<Script::ScriptData*>(myScriptPath))
		{
			myScriptGraph->Deserialize(*data);
			myScriptGraph->SetPath(myScriptPath);
		}
	}
}

std::shared_ptr<ScriptGraph> ScriptComponent::GetScriptGraph()
{
	return myScriptGraph;
}

void ScriptComponent::CreateImGuiComponents()
{
	Component::CreateImGuiComponents();
	if (myScriptGraph)
	{
		ImGui::Text(myScriptPath.c_str());
	}
	else
	{
		ImGui::Text("No script selected!");
	}

	constexpr ImVec2 imageSize = { 75.f, 75.f };
	ImGui::Image(nullptr, imageSize);

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Dragged_Script"))
		{
			IM_ASSERT(payload->DataSize == sizeof(std::string*));
			myScriptPath = *static_cast<std::string*>(payload->Data);
			if (auto data = AssetManager::GetAsset<Script::ScriptData*>(myScriptPath))
			{
				myScriptGraph = std::make_shared<ScriptGraph>(ScriptGraph(myParent));
				myScriptGraph->Deserialize(*data);
				myScriptGraph->SetPath(myScriptPath);
			}
		}
		ImGui::EndDragDropTarget();
	}
}

Json::Value ScriptComponent::ToJson() const
{
	auto result = Component::ToJson();
	result["ScriptPath"] = myScriptGraph ? myScriptGraph->GetPath() : "";
	return result;
}
