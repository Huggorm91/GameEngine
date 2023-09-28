#include "AssetManager.pch.h"
#include "GameObject.h"
#include "Components/Rendering/AnimatedMeshComponent.h"
#include "Prefab.h"

#ifndef _RETAIL
#include "ModelViewer/Core/ModelViewer.h"
#include "Modelviewer/Core/Commands/EditCmd_ChangeGameObjectName.h"
#endif // !_RETAIL


unsigned int GameObject::localIDCount = 0;

GameObject::GameObject() : myComponents(1000), myIndexList(), myCount(0), myTransform(), myIsActive(true), myID(++localIDCount), myName("GameObject"), myImguiText(myName), myParent(nullptr), myChildren()
#ifndef _RETAIL
, myDebugPointers()
#endif // !_RETAIL
{
}

#ifndef _RETAIL
GameObject::GameObject(unsigned anID) : myComponents(1000), myIndexList(), myCount(0), myTransform(), myIsActive(true), myID(anID), myName("GameObject"), myImguiText(myName), myDebugPointers(), myParent(nullptr), myChildren()
{
}
#endif // !_RETAIL

GameObject::GameObject(const Prefab& aPrefab) : GameObject()
{
	if (aPrefab.myTemplate)
	{
		const Component* pointer = nullptr;
		for (auto [type, index] : aPrefab.myTemplate->myIndexList)
		{
			pointer = aPrefab.myTemplate->myComponents.GetValueUnsafe<Component>(index)->GetTypePointer();
			::AddComponent(pointer, *this);
		}
	}
	else
	{
		AMLogger.Err("GameObject " + std::to_string(myID) + ": Trying to copy from invalid prefab");
	}
}

GameObject::GameObject(const GameObject& aGameObject) : myComponents(), myIndexList(), myTransform(aGameObject.myTransform), myCount(), myIsActive(aGameObject.myIsActive), myID(++localIDCount), myName(aGameObject.myName),
myImguiText(myName), myParent(aGameObject.myParent), myChildren(aGameObject.myChildren)
#ifndef _RETAIL
, myDebugPointers()
#endif // !_RETAIL
{
	const Component* pointer = nullptr;
	for (auto [type, index] : aGameObject.myIndexList)
	{
		pointer = aGameObject.myComponents.GetValueUnsafe<Component>(index)->GetTypePointer();
		::AddComponent(pointer, *this);
	}
}

GameObject::GameObject(GameObject&& aGameObject) noexcept : myComponents(aGameObject.myComponents), myIndexList(aGameObject.myIndexList), myTransform(aGameObject.myTransform), myCount(aGameObject.myCount), myIsActive(aGameObject.myIsActive),
myID(aGameObject.myID), myName(aGameObject.myName), myImguiText(myName), myParent(aGameObject.myParent), myChildren(aGameObject.myChildren)
#ifndef _RETAIL
, myDebugPointers()
#endif // !_RETAIL
{
	for (auto [type, index] : myIndexList)
	{
		myComponents.ChangeValueUnsafe<Component>(index)->Init(this);
#ifndef _RETAIL
		myDebugPointers.emplace_back(myComponents.GetValueUnsafe<Component>(index));
#endif // !_RETAIL
	}
}

GameObject::GameObject(const Json::Value& aJson) : myComponents(1000), myIndexList(), myCount(0), myTransform(aJson["Transform"]), myIsActive(aJson["IsActive"].asBool()), myID(aJson["ID"].asUInt()), myName(aJson["Name"].asString()), myImguiText(myName), myParent(nullptr), myChildren()
#ifndef _RETAIL
, myDebugPointers()
#endif // !_RETAIL
{
	for (auto& jsonComponent : aJson["Components"])
	{
		LoadComponent(jsonComponent, *this);
	}
}

GameObject::~GameObject()
{
	if (myParent)
	{
		myParent->RemoveChild(this);
	}
	for (auto& child : myChildren)
	{
		child->RemoveParent();
	}
}

GameObject& GameObject::operator=(const Prefab& aPrefab)
{
	if (aPrefab.myTemplate)
	{
		myCount = 0;
		myComponents.Clear();
		myIndexList.clear();
#ifndef _RETAIL
		myDebugPointers.clear();
#endif // !_RETAIL
		const Component* pointer = nullptr;
		for (auto [type, index] : aPrefab.myTemplate->myIndexList)
		{
			pointer = aPrefab.myTemplate->myComponents.GetValueUnsafe<Component>(index)->GetTypePointer();
			::AddComponent(pointer, *this);
		}
	}
	else
	{
		AMLogger.Err("GameObject " + std::to_string(myID) + ": Trying to copy from invalid prefab");
	}
	return *this;
}

GameObject& GameObject::operator=(const GameObject& aGameObject)
{
	myCount = 0;
	myComponents.Clear();
	myIndexList.clear();
#ifndef _RETAIL
	myDebugPointers.clear();
#endif // !_RETAIL
	myTransform = aGameObject.myTransform;
	myIsActive = aGameObject.myIsActive;
	myName = aGameObject.myName;
	myImguiText = myName;
	myParent = aGameObject.myParent;
	myChildren = aGameObject.myChildren;

	const Component* pointer = nullptr;
	for (auto [type, index] : aGameObject.myIndexList)
	{
		pointer = aGameObject.myComponents.GetValueUnsafe<Component>(index)->GetTypePointer();
		::AddComponent(pointer, *this);
	}
	return *this;
}

GameObject& GameObject::operator=(GameObject&& aGameObject) noexcept
{
#ifndef _RETAIL
	myDebugPointers.clear();
#endif // !_RETAIL
	myCount = aGameObject.myCount;
	myComponents = aGameObject.myComponents;
	myIndexList = aGameObject.myIndexList;
	myTransform = aGameObject.myTransform;
	myIsActive = aGameObject.myIsActive;
	myName = aGameObject.myName;
	myImguiText = myName;
	const_cast<unsigned&>(myID) = aGameObject.myID;
	myParent = aGameObject.myParent;
	myChildren = aGameObject.myChildren;

	for (auto [type, index] : myIndexList)
	{
		myComponents.ChangeValueUnsafe<Component>(index)->Init(this);
#ifndef _RETAIL
		myDebugPointers.emplace_back(myComponents.GetValueUnsafe<Component>(index));
#endif // !_RETAIL
	}
	return *this;
}

void GameObject::Update()
{
	if (myIsActive)
	{
		if (myTransform.HasChanged())
		{
			myTransform.Update();

			for (auto [type, index] : myIndexList)
			{
				myComponents.GetValueUnsafe<Component>(index)->TransformHasChanged();
			}
		}

		for (auto [type, index] : myIndexList)
		{
			myComponents.ChangeValueUnsafe<Component>(index)->Update();
		}
	}
}

const Component* GameObject::GetComponentPointer(unsigned anID) const
{
	for (auto [type, index] : myIndexList)
	{
		if (const Component* component = myComponents.GetValueUnsafe<Component>(index); component->GetComponentID() == anID)
		{
			return component;
		}
	}
	return nullptr;
}

Component* GameObject::GetComponentPointer(unsigned anID)
{
	for (auto [type, index] : myIndexList)
	{
		if (Component* component = myComponents.ChangeValueUnsafe<Component>(index); component->GetComponentID() == anID)
		{
			return component;
		}
	}
	return nullptr;
}

void GameObject::SetPosition(const CommonUtilities::Vector3f& aPosition)
{
	myTransform.SetPosition(aPosition);
}

void GameObject::SetRotation(const CommonUtilities::Vector3f& aRotation)
{
	myTransform.SetRotation(aRotation);
}

void GameObject::SetScale(const CommonUtilities::Vector3f& aScale)
{
	myTransform.SetScale(aScale);
}

const Transform& GameObject::GetTransform() const
{
	return myTransform;
}

const CommonUtilities::Matrix4x4f& GameObject::GetTransformMatrix() const
{
	if (myTransform.HasChanged())
	{
		for (auto [type, index] : myIndexList)
		{
			myComponents.GetValueUnsafe<Component>(index)->TransformHasChanged();
		}
	}
	return myTransform.GetTransformMatrix();
}

const CommonUtilities::Vector4f& GameObject::GetWorldPosition() const
{
	if (myTransform.HasChanged())
	{
		for (auto [type, index] : myIndexList)
		{
			myComponents.GetValueUnsafe<Component>(index)->TransformHasChanged();
		}
	}
	return myTransform.GetWorldPosition();
}

void GameObject::SetActive(bool aIsActive)
{
	myIsActive = aIsActive;
}

void GameObject::ToogleActive()
{
	myIsActive = !myIsActive;
}

bool GameObject::IsActive() const
{
	return myIsActive;
}

void GameObject::SetParent(GameObject* anObject)
{
	myParent = anObject;
	myTransform.SetParent(&anObject->myTransform);
}

void GameObject::RemoveParent()
{
	myParent = nullptr;
	myTransform.RemoveParent();
}

void GameObject::AddChild(GameObject* anObject)
{
	anObject->SetParent(this);
	myChildren.emplace_back(anObject);
}

void GameObject::RemoveChild(GameObject* anObject)
{
	for (auto iter = myChildren.begin(); iter != myChildren.end(); iter++)
	{
		if (*iter == anObject)
		{
			(*iter)->RemoveParent();
			myChildren.erase(iter);
			return;
		}
	}
}

void GameObject::SetName(const std::string& aName)
{
	myName = aName;
	myImguiText = aName;
}

const std::string& GameObject::GetName() const
{
	return myName;
}

unsigned int GameObject::GetComponentCount() const
{
	return myCount;
}

unsigned int GameObject::GetID() const
{
	return myID;
}

void GameObject::CreateImGuiWindowContent(const std::string& aWindowName)
{
	if (ImGui::CollapsingHeader(myName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		std::string id = "ID: " + std::to_string(myID);
		ImGui::Text(id.c_str());
		ImGui::Checkbox("Active", &myIsActive);
		if (ImGui::InputText("Name", &myImguiText, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
		{
#ifndef _RETAIL
			ModelViewer::Get().AddCommand(std::make_shared<EditCmd_ChangeGameObjectName>(this, myImguiText));
#else
			myName = myImguiText;
#endif // !_RETAIL
		}
		myTransform.CreateImGuiComponents(aWindowName);
		if (ImGui::CollapsingHeader("Components", ImGuiTreeNodeFlags_DefaultOpen))
		{
			Component* component = nullptr;
			std::string text;
			for (auto [type, index] : myIndexList)
			{
				component = myComponents.ChangeValueUnsafe<Component>(index);
				text = ComponentTypeToString(component->GetType()) + " " + std::to_string(component->GetComponentID());
				ImGui::SetNextItemOpen(true, ImGuiCond_::ImGuiCond_Appearing);
				if (ImGui::TreeNode(text.c_str(), ComponentTypeToString(component->GetType()).c_str()))
				{
					component->CreateImGuiComponents(aWindowName);
					ImGui::TreePop();
				}
			}
		}
	}
}

Json::Value GameObject::ToJson() const
{
	Json::Value result;
	result["IsActive"] = myIsActive;
	result["ID"] = myID;
	result["Name"] = myName;
	result["Transform"] = myTransform.ToJson();

	result["Components"] = Json::arrayValue;
	const Component* component = nullptr;
	unsigned i = 0;
	for (auto [type, index] : myIndexList)
	{
		component = myComponents.GetValueUnsafe<Component>(index);
		result["Components"][i] = component->ToJson();
		result["Components"][i].setComment("// " + ComponentTypeToString(component->GetType()), Json::commentBefore);
		++i;
	}

	return result;
}

void GameObject::MarkAsPrefab()
{
	if (myID != 0)
	{
		const_cast<unsigned&>(myID) = 0;
		localIDCount--;
	}
	for (auto [type, index] : myIndexList)
	{
		myComponents.ChangeValueUnsafe<Component>(index)->MarkAsPrefabComponent();
	}
}

void GameObject::MarkAsPrefab(unsigned anID)
{
	if (myID != anID)
	{
		const_cast<unsigned&>(myID) = anID;
		localIDCount--;
	}
}