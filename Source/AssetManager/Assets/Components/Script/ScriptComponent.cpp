#include "AssetManager.pch.h"
#include "ScriptComponent.h"

#include "AssetManager.h"
#include "Nodes/NodeInclude.h"
#include "../Collision/ColliderComponent.h"
#include "ScriptGraph/ScriptGraphPayload.h"

ScriptComponent::ScriptComponent() : Component(ComponentType::Script), myScriptGraph(nullptr)
{}

ScriptComponent::ScriptComponent(const ScriptComponent& aComponent) : Component(aComponent), myScriptGraph(nullptr), myScriptPath(aComponent.myScriptPath)
{
}

ScriptComponent::ScriptComponent(ScriptComponent&& aComponent) noexcept : Component(std::move(aComponent)), myScriptGraph(nullptr), myScriptPath(aComponent.myScriptPath)
{
}

ScriptComponent::ScriptComponent(const Json::Value& aJson) : Component(aJson), myScriptGraph(nullptr), myScriptPath(aJson["ScriptPath"].asString())
{}

ScriptComponent& ScriptComponent::operator=(const ScriptComponent & aComponent)
{
	Component::operator=(aComponent);
	Script::ScriptData data;
	myScriptPath = aComponent.myScriptPath;
	aComponent.myScriptGraph->Serialize(data);
	myScriptGraph->Deserialize(data);
	myScriptGraph->SetPath(myScriptPath);
	return *this;
}

ScriptComponent& ScriptComponent::operator=(ScriptComponent&& aComponent) noexcept
{
	Component::operator=(std::move(aComponent));
	Script::ScriptData data;
	myScriptPath = aComponent.myScriptPath;
	aComponent.myScriptGraph->Serialize(data);
	myScriptGraph->Deserialize(data);
	myScriptGraph->SetPath(myScriptPath);
	return *this;
}

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
	if (!myScriptGraph)
	{
		myScriptGraph = std::make_shared<ScriptGraph>(ScriptGraph(myParent));
	}	
	
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

void ScriptComponent::OnCollisionEnter(CollisionLayer::Layer, ColliderComponent* aCollider)
{
	ScriptGraphPayload payload;
	payload.SetPinValue("Collider ID", aCollider->GetParent().GetID());
	myScriptGraph->ExecuteWithPayload("On Collision Enter", payload);
}

void ScriptComponent::OnCollisionStay(CollisionLayer::Layer, ColliderComponent* aCollider)
{
	ScriptGraphPayload payload;
	payload.SetPinValue("Collider ID", aCollider->GetParent().GetID());
	myScriptGraph->ExecuteWithPayload("On Collision Stay", payload);
}

void ScriptComponent::OnCollisionExit(CollisionLayer::Layer, ColliderComponent* aCollider)
{
	ScriptGraphPayload payload;
	payload.SetPinValue("Collider ID", aCollider->GetParent().GetID());
	myScriptGraph->ExecuteWithPayload("On Collision Exit", payload);
}

void ScriptComponent::OnTriggerEnter(CollisionLayer::Layer, ColliderComponent* aCollider)
{
	ScriptGraphPayload payload;
	payload.SetPinValue("Trigger ID", aCollider->GetParent().GetID());
	myScriptGraph->ExecuteWithPayload("On Trigger Enter", payload);
}

void ScriptComponent::OnTriggerStay(CollisionLayer::Layer, ColliderComponent* aCollider)
{
	ScriptGraphPayload payload;
	payload.SetPinValue("Trigger ID", aCollider->GetParent().GetID());
	myScriptGraph->ExecuteWithPayload("On Trigger Stay", payload);
}

void ScriptComponent::OnTriggerExit(CollisionLayer::Layer, ColliderComponent* aCollider)
{
	ScriptGraphPayload payload;
	payload.SetPinValue("Trigger ID", aCollider->GetParent().GetID());
	myScriptGraph->ExecuteWithPayload("On Trigger Exit", payload);
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
