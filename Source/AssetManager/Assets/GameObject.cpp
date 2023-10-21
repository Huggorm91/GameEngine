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
myImguiText(myName), myParent(), myChildren()
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
	for (auto& child : myChildren)
	{
		child->myParent = nullptr;
		child->myTransform.RemoveParent();
	}
	RemoveFromParent();
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
			TransformHasChanged();
		}

		for (auto [type, index] : myIndexList)
		{
			myComponents.ChangeValueUnsafe<Component>(index)->Update();
		}
	}
}

void GameObject::Render()
{
	if (myIsActive)
	{
		if (myTransform.HasChanged())
		{
			TransformHasChanged();
		}

		for (auto [type, index] : myIndexList)
		{
			myComponents.ChangeValueUnsafe<Component>(index)->Render();
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

void GameObject::SetPosition(const Crimson::Vector3f& aPosition)
{
	myTransform.SetPosition(aPosition);
}

void GameObject::SetRotation(const Crimson::Vector3f& aRotation)
{
	myTransform.SetRotation(aRotation);
}

void GameObject::SetScale(const Crimson::Vector3f& aScale)
{
	myTransform.SetScale(aScale);
}

const Transform& GameObject::GetTransform() const
{
	return myTransform;
}

const Crimson::Matrix4x4f& GameObject::GetTransformMatrix() const
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

const Crimson::Vector4f& GameObject::GetWorldPosition() const
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
	if (anObject == this)
	{
		return;
	}
	RemoveFromParent();
	myParent = anObject;
	myTransform.SetParent(&anObject->myTransform);
	TransformHasChanged();
}

void GameObject::RemoveParentInternal()
{
	myParent = nullptr;
	myTransform.RemoveParent();
	TransformHasChanged();
}

void GameObject::RemoveFromParent()
{
	if (myParent)
	{
		for (auto iter = myParent->myChildren.begin(); iter != myParent->myChildren.end(); iter++)
		{
			if ((*iter) == this)
			{
				myParent->myChildren.erase(iter);
				break;
			}
		}
	}
}

void GameObject::TransformHasChanged()
{
	myTransform.Update();

	for (auto [type, index] : myIndexList)
	{
		myComponents.GetValueUnsafe<Component>(index)->TransformHasChanged();
	}

	for (auto& child : myChildren)
	{
		child->TransformHasChanged();
	}
}

bool GameObject::IsRelated(GameObject* anObject)
{
	if (IsChildRecursive(anObject))
	{
		return true;
	}
	return IsParentRecursive(anObject);
}

bool GameObject::IsChildRecursive(GameObject* anObject)
{
	if (IsChild(anObject))
	{
		return true;
	}

	for (auto& child : myChildren)
	{
		if (child->IsChildRecursive(anObject))
		{
			return true;
		}
	}
	return false;
}

bool GameObject::IsChild(GameObject* anObject)
{
	for (auto& child : myChildren)
	{
		if (child == anObject)
		{
			return true;
		}
	}
	return false;
}

bool GameObject::IsParentRecursive(GameObject* anObject)
{
	if (myParent)
	{
		if (IsParent(anObject))
		{
			return true;
		}
		return myParent->IsParentRecursive(anObject);
	}
	return false;
}

bool GameObject::IsParent(GameObject* anObject)
{
	return myParent == anObject;
}

void GameObject::AddChild(GameObject* anObject)
{
	if (anObject == this || IsRelated(anObject))
	{
		return;
	}
	anObject->SetParent(this);
	myChildren.emplace_back(anObject);
}

void GameObject::RemoveChild(GameObject* anObject)
{
	for (auto iter = myChildren.begin(); iter != myChildren.end(); iter++)
	{
		if (*iter == anObject)
		{
			(*iter)->RemoveParentInternal();
			myChildren.erase(iter);
			return;
		}
	}
}

const std::vector<GameObject*>& GameObject::GetChildren() const
{
	return myChildren;
}

std::vector<GameObject*>& GameObject::GetChildren()
{
	return myChildren;
}

const GameObject* GameObject::GetParent() const
{
	return myParent;
}

GameObject* GameObject::GetParent()
{
	return myParent;
}

void GameObject::RemoveParent()
{
	RemoveFromParent();
	RemoveParentInternal();
}

bool GameObject::HasChild() const
{
	return myChildren.size() > 0;
}

bool GameObject::HasParent() const
{
	return myParent != nullptr;
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
				text = component->ToString() + " " + std::to_string(component->GetComponentID());
				ImGui::SetNextItemOpen(true, ImGuiCond_::ImGuiCond_Appearing);
				if (ImGui::TreeNode(text.c_str(), component->ToString().c_str()))
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

	if (myParent)
	{
		result["ParentID"] = myParent->myID;
	}
	else
	{
		result["ParentID"] = 0u;
	}

	result["Components"] = Json::arrayValue;
	const Component* component = nullptr;
	unsigned i = 0;
	for (auto [type, index] : myIndexList)
	{
		component = myComponents.GetValueUnsafe<Component>(index);
		result["Components"][i] = component->ToJson();
		result["Components"][i].setComment("// " + component->ToString(), Json::commentBefore);
		++i;
	}

	return result;
}

struct GameObjectData
{
	unsigned ID;
	unsigned ParentID;
	unsigned ComponentCount;
	bool IsActive;
};

void GameObject::Serialize(std::ostream& aStream) const
{
	Binary::eType type = Binary::GameObject;
	GameObjectData data;
	data.ID = myID;
	data.ParentID = myParent ? myParent->myID : 0u;
	data.ComponentCount = myCount;
	data.IsActive = myIsActive;
	aStream.write(reinterpret_cast<char*>(&type), sizeof(type));
	aStream.write(reinterpret_cast<char*>(&data), sizeof(data));
	aStream.write(myName.c_str(), myName.size() + 1);
	myTransform.Serialize(aStream);

	for (auto [compType, index] : myIndexList)
	{
		myComponents.GetValueUnsafe<Component>(index)->Serialize(aStream);
	}
}

unsigned GameObject::Deserialize(std::istream& aStream)
{
	GameObjectData data;
	aStream.read(reinterpret_cast<char*>(&data), sizeof(data));
	const_cast<unsigned&>(myID) = data.ID;
	myIsActive = data.IsActive;
	std::getline(aStream, myName, '\0');
	myTransform.Deserialize(aStream);

	for (unsigned i = 0; i < data.ComponentCount; i++)
	{
		Binary::eType type;
		aStream.read(reinterpret_cast<char*>(&type), sizeof(type));
		if (type != Binary::Component)
		{
			throw std::runtime_error("GameObject::Deserialize: Invalid Binary::Type when loading components. ID: " + std::to_string(myID));
		}
		LoadComponent(aStream, *this);
	}
	return data.ParentID;
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

void GameObject::CopyIDsOf(const GameObject& anObject, bool aDecrementIDCount)
{
	if (aDecrementIDCount)
	{
		localIDCount--;
	}
	const_cast<unsigned&>(myID) = anObject.myID;

	for (auto [type, index] : myIndexList)
	{
		myComponents.ChangeValueUnsafe<Component>(index)->CopyID(anObject.myComponents.GetValueUnsafe<Component>(index), aDecrementIDCount);
	}
}

unsigned GameObject::GetParentID(const Json::Value& aJson)
{
	return aJson["ParentID"].asUInt();
}

void SetGameObjectIDCount(unsigned aValue)
{
	GameObject::localIDCount = aValue;
}
