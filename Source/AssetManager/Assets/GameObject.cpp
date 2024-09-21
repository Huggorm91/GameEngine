#include "AssetManager.pch.h"
#include "GameObject.h"
#include "Components/Rendering/AnimatedMeshComponent.h"
#include "GameplayEngine/Network/GameObjectMessage.h"
#include "Prefab.h"

#ifdef EDITOR
#include "Editor/ModelViewer.h"
#include "Editor/Commands/EditCmd_ChangeGameObjectName.h"
#include "ImguiTransform.h"
#endif // !_RETAIL

UUIDv4::UUID GameObject::nullUUID(uint64_t(0), uint64_t(0));

GameObject::GameObject() :
	myIsActive(true),
	mySyncTimer(-1.f),
	myParent(nullptr),
	myUUID(GenerateUUID()),
	myName("GameObject"),
#ifdef EDITOR
	myImguiText(myName),
#endif // EDITOR
	myTransform(),
	myChildren(),
#ifndef _RETAIL
	myDebugPointers(),
#endif // !_RETAIL
	myIndexList(),
	myComponents(1000u)
{
}

GameObject::GameObject(const Prefab& aPrefab) : GameObject()
{
	if (aPrefab.myTemplate)
	{
		for (auto& [type, index] : aPrefab.myTemplate->myIndexList)
		{
			const Component* oldComponent = &aPrefab.myTemplate->myComponents.GetValue<Component>(index);
			Component* newComponent = AllocateComponent(oldComponent);

			oldComponent->CopyTo(newComponent);
			newComponent->Init(this);
		}
	}
	else
	{
		AMLogger.Err("GameObject " + myUUID.str() + ": Trying to copy from invalid prefab");
	}
}

GameObject::GameObject(const UUIDv4::UUID& anUUID) :
	myIsActive(true),
	mySyncTimer(-1.f),
	myParent(nullptr),
	myUUID(anUUID),
	myName("GameObject"),
#ifdef EDITOR
	myImguiText(myName),
#endif // EDITOR
	myComponents(1000u)
{
}

GameObject::GameObject(const GameObject& aGameObject) :
	myIsActive(aGameObject.myIsActive),
	mySyncTimer(-1.f),
	myParent(nullptr),
	myUUID(GenerateUUID()),
	myName(aGameObject.myName),
#ifdef EDITOR
	myImguiText(myName),
#endif // EDITOR
	myTransform(aGameObject.myTransform),
	myComponents(aGameObject.myComponents.GetSize())
{
	for (auto& [type, index] : aGameObject.myIndexList)
	{
		const Component* oldComponent = &aGameObject.myComponents.GetValue<Component>(index);
		Component* newComponent = AllocateComponent(oldComponent);

		oldComponent->CopyTo(newComponent);
		newComponent->Init(this);
	}
}

GameObject::GameObject(GameObject&& aGameObject) noexcept :
	myIsActive(aGameObject.myIsActive),
	mySyncTimer(aGameObject.mySyncTimer),
	myParent(aGameObject.myParent),
	mySyncMovement(aGameObject.mySyncMovement),
	myUUID(aGameObject.myUUID),
	myName(aGameObject.myName),
#ifdef EDITOR
	myImguiText(myName),
#endif // EDITOR
	myTransform(aGameObject.myTransform),
	myChildren(aGameObject.myChildren),
	myComponents(aGameObject.myComponents.GetSize())
{
	for (auto& [type, index] : aGameObject.myIndexList)
	{
		Component* oldComponent = &aGameObject.myComponents.GetValue<Component>(index);
		Component* newComponent = AllocateComponent(oldComponent);

		oldComponent->MoveTo(newComponent);
		newComponent->Init(this);
	}
}

GameObject::GameObject(const Json::Value& aJson) :
	myIsActive(aJson["IsActive"].asBool()),
	mySyncTimer(-1.f),
	myParent(nullptr),
	myUUID(aJson["UUID"].isNull() ? GenerateUUID().bytes() : aJson["UUID"].asString()),
	myName(aJson["Name"].asString()),
#ifdef EDITOR
	myImguiText(myName),
#endif // EDITOR
	myTransform(aJson["Transform"]),
	myChildren(),
#ifndef _RETAIL
	myDebugPointers(),
#endif // !_RETAIL
	myIndexList(),
	myComponents(aJson["MemorySize"].asLargestUInt())
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
	for (auto& [type, index] : myIndexList)
	{
		myComponents.GetValue<Component>(index).~Component();
	}
}

GameObject& GameObject::operator=(const Prefab& aPrefab)
{
	if (aPrefab.myTemplate)
	{
		myComponents.Clear();
		myIndexList.clear();
#ifndef _RETAIL
		myDebugPointers.clear();
#endif // !_RETAIL
		for (auto& [type, index] : aPrefab.myTemplate->myIndexList)
		{
			const Component* oldComponent = &aPrefab.myTemplate->myComponents.GetValue<Component>(index);
			Component* newComponent = AllocateComponent(oldComponent);

			oldComponent->CopyTo(newComponent);
			newComponent->Init(this);
		}
	}
	else
	{
		AMLogger.Err("GameObject " + myUUID.str() + ": Trying to copy from invalid prefab");
	}
	return *this;
}

GameObject& GameObject::operator=(const GameObject& aGameObject)
{
	myComponents.Clear();
	myIndexList.clear();
	myComponents.Resize(aGameObject.myComponents.GetSize());
#ifndef _RETAIL
	myDebugPointers.clear();
#endif // !_RETAIL

#ifdef EDITOR
	myImguiText = aGameObject.myName;
#endif // EDITOR
	myTransform = aGameObject.myTransform;
	myIsActive = aGameObject.myIsActive;
	myName = aGameObject.myName;
	myParent = aGameObject.myParent;
	myChildren = aGameObject.myChildren;

	for (auto& [type, index] : aGameObject.myIndexList)
	{
		const Component* oldComponent = &aGameObject.myComponents.GetValue<Component>(index);
		Component* newComponent = AllocateComponent(oldComponent);

		oldComponent->CopyTo(newComponent);
		newComponent->Init(this);
	}

	return *this;
}

GameObject& GameObject::operator=(GameObject&& aGameObject) noexcept
{
	myComponents.Clear();
	myIndexList.clear();
	myComponents.Resize(aGameObject.myComponents.GetSize());
#ifndef _RETAIL
	myDebugPointers.clear();
#endif // !_RETAIL

#ifdef EDITOR
	myImguiText = aGameObject.myName;
#endif // EDITOR
	myTransform = aGameObject.myTransform;
	myIsActive = aGameObject.myIsActive;
	myName = aGameObject.myName;
	const_cast<UUIDv4::UUID&>(myUUID) = aGameObject.myUUID;
	myParent = aGameObject.myParent;
	myChildren = aGameObject.myChildren;
	mySyncTimer = aGameObject.mySyncTimer;
	mySyncMovement = aGameObject.mySyncMovement;

	for (auto& [type, index] : aGameObject.myIndexList)
	{
		Component* oldComponent = &aGameObject.myComponents.GetValue<Component>(index);
		Component* newComponent = AllocateComponent(oldComponent);

		oldComponent->MoveTo(newComponent);
		newComponent->Init(this);
	}

	return *this;
}

bool GameObject::operator==(const GameObject& aGameObject) const
{
	return myUUID == aGameObject.myUUID;
}

bool GameObject::operator==(const UUIDv4::UUID& anUUID) const
{
	return myUUID == anUUID;
}

void GameObject::Update()
{
	if (myIsActive)
	{
		if (mySyncTimer >= 0.f)
		{
			const float timeDelta = Crimson::Time::GetDeltaTime() / Network::globalSyncFrequency;
			const float syncDelta = mySyncTimer / Network::globalSyncFrequency;
			const float lerpValue = Crimson::Clamp(timeDelta, 0.f, syncDelta);
			myTransform.AddToPosition(Crimson::Lerp(Crimson::Vector3f::Null, mySyncMovement, lerpValue));
			myTransform.AddToRotationRadian(Crimson::Lerp(Crimson::Vector3f::Null, mySyncRotation, lerpValue));

			mySyncTimer += Crimson::Time::GetDeltaTime();
			if (mySyncTimer >= Network::globalSyncFrequency)
			{
				mySyncTimer = -1.f;
			}
		}
		if (myTransform.HasChanged())
		{
			TransformHasChanged();
		}

		for (auto& [type, index] : myIndexList)
		{
			myComponents.GetValue<Component>(index).Update();
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

		for (auto& [type, index] : myIndexList)
		{
			myComponents.GetValue<Component>(index).Render();
		}
	}
}

void GameObject::DebugDraw()
{
	if (myIsActive)
	{
		if (myTransform.HasChanged())
		{
			TransformHasChanged();
		}

		for (auto& [type, index] : myIndexList)
		{
			myComponents.GetValue<Component>(index).DebugDraw();
		}
	}
}

void GameObject::OnCollisionEnter(CollisionLayer::Layer aLayer, ColliderComponent* aCollider)
{
	for (auto& [type, index] : myIndexList)
	{
		myComponents.GetValue<Component>(index).OnCollisionEnter(aLayer, aCollider);
	}
}

void GameObject::OnCollisionStay(CollisionLayer::Layer aLayer, ColliderComponent* aCollider)
{
	for (auto& [type, index] : myIndexList)
	{
		myComponents.GetValue<Component>(index).OnCollisionStay(aLayer, aCollider);
	}
}

void GameObject::OnCollisionExit(CollisionLayer::Layer aLayer, ColliderComponent* aCollider)
{
	for (auto& [type, index] : myIndexList)
	{
		myComponents.GetValue<Component>(index).OnCollisionExit(aLayer, aCollider);
	}
}

void GameObject::OnTriggerEnter(CollisionLayer::Layer aLayer, ColliderComponent* aTrigger)
{
	for (auto& [type, index] : myIndexList)
	{
		myComponents.GetValue<Component>(index).OnTriggerEnter(aLayer, aTrigger);
	}
}

void GameObject::OnTriggerStay(CollisionLayer::Layer aLayer, ColliderComponent* aTrigger)
{
	for (auto& [type, index] : myIndexList)
	{
		myComponents.GetValue<Component>(index).OnTriggerStay(aLayer, aTrigger);
	}
}

void GameObject::OnTriggerExit(CollisionLayer::Layer aLayer, ColliderComponent* aTrigger)
{
	for (auto& [type, index] : myIndexList)
	{
		myComponents.GetValue<Component>(index).OnTriggerExit(aLayer, aTrigger);
	}
}

void GameObject::RecieveNetmessage(const Network::GameObjectMessage& aMessage)
{
	if (aMessage.action == Network::ObjectAction::Move)
	{
#ifndef NETWORK_SERVER
		const auto& position = reinterpret_cast<const Crimson::Vector3f&>(aMessage.data);
		mySyncMovement = position - myTransform.GetPosition();
		const auto& rotation = reinterpret_cast<const Crimson::Vector3f&>(aMessage.data[sizeof(Crimson::Vector3f)]);
		mySyncRotation = rotation - myTransform.GetRotationRadian();
		mySyncTimer = 0.f;
#else
		myTransform.SetPosition(reinterpret_cast<const Crimson::Vector3f&>(aMessage.data));
		myTransform.SetRotationRadian(reinterpret_cast<const Crimson::Vector3f&>(aMessage.data[sizeof(Crimson::Vector3f)]));
#endif // !NETWORK_SERVER
	}

	for (auto& [type, index] : myIndexList)
	{
		myComponents.GetValue<Component>(index).RecieveNetmessage(aMessage);
	}
}

void GameObject::SetPosition(const Crimson::Vector3f& aPosition)
{
	myTransform.SetPosition(aPosition);
}

void GameObject::AddToPosition(const Crimson::Vector3f& aChange)
{
	myTransform.AddToPosition(aChange);
}

void GameObject::SetRotation(const Crimson::Vector3f& aRotation, bool anIsRadians)
{
	if (anIsRadians)
	{
		myTransform.SetRotationRadian(aRotation);
	}
	else
	{
		myTransform.SetRotationDegree(aRotation);
	}
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
		for (auto& [type, index] : myIndexList)
		{
			myComponents.GetValue<Component>(index).TransformHasChanged();
		}
	}
	return myTransform.GetTransformMatrix();
}

const Crimson::Vector4f& GameObject::GetWorldPosition() const
{
	if (myTransform.HasChanged())
	{
		const_cast<Transform&>(myTransform).Update();
		for (auto& [type, index] : myIndexList)
		{
			myComponents.GetValue<Component>(index).TransformHasChanged();
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

void GameObject::SetActiveComponents(bool aIsActive)
{
	for (auto& [type, index] : myIndexList)
	{
		myComponents.GetValue<Component>(index).SetActive(aIsActive);
	}
}

void GameObject::ToogleActiveComponents()
{
	for (auto& [type, index] : myIndexList)
	{
		myComponents.GetValue<Component>(index).ToogleActive();
	}
}

Component* GameObject::AllocateComponent(const Component* aComponent)
{
	// Allocate memory for the component
	auto newIndex = myComponents.Allocate(aComponent->SizeOf());

	if (newIndex.second)
	{
		ComponentContainerResized();
	}

	// Save references to new component
	myIndexList.emplace(aComponent->TypeId(), newIndex.first);
#ifndef _RETAIL
	myDebugPointers.emplace_back(&myComponents.GetValue<Component>(newIndex.first));
#endif // !_RETAIL

	return &myComponents.GetValue<Component>(newIndex.first);
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

void GameObject::ComponentContainerResized()
{
#ifndef _RETAIL
	myDebugPointers.clear();
#endif // !_RETAIL

	for (auto& [type, index] : myIndexList)
	{
		Component& component = myComponents.GetValue<Component>(index);
		component.ComponentPointersInvalidated();
#ifndef _RETAIL
		myDebugPointers.emplace_back(&component);
#endif // !_RETAIL
	}
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

	for (auto& [type, index] : myIndexList)
	{
		myComponents.GetValue<Component>(index).TransformHasChanged();
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

UUIDv4::UUID GameObject::GenerateUUID()
{
	static UUIDv4::UUIDGenerator<std::mt19937_64> localUUIDGenerator;
	UUIDv4::UUID result = nullUUID;
	while (result == nullUUID)
	{
		result = localUUIDGenerator.getUUID();
	}
	return result;
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

#ifdef EDITOR
	myImguiText = aName;
	ModelViewer::GetImguiManager().ChangeIndexName(this, aName);
#endif // EDITOR
}

const std::string& GameObject::GetName() const
{
	return myName;
}

std::string GameObject::ToString() const
{
	return myName + ": " + myUUID.str();
}

unsigned int GameObject::GetComponentCount() const
{
	return static_cast<unsigned>(myIndexList.size());
}


const UUIDv4::UUID& GameObject::GetUUID() const
{
	return myUUID;
}

#ifdef EDITOR
void GameObject::CreateImGuiWindowContent(const std::string& aWindowName)
{
	if (ImGui::CollapsingHeader(myName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		std::string id = "ID: " + myUUID.str();
		ImGui::Text(id.c_str());
		ImGui::Checkbox("Active", &myIsActive);
		if (ImGui::InputText("Name", &myImguiText, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
		{
			ModelViewer::Get().AddCommand(std::make_shared<EditCmd_ChangeGameObjectName>(this, myImguiText));
		}
		::CreateImGuiComponents(myTransform);
		if (ImGui::CollapsingHeader("Components", ImGuiTreeNodeFlags_DefaultOpen))
		{
			Component* component = nullptr;
			std::string text;
			for (auto& [type, index] : myIndexList)
			{
				component = &myComponents.GetValue<Component>(index);
				ImGui::PushID(static_cast<int>(index));
				ImGui::SetNextItemOpen(true, ImGuiCond_::ImGuiCond_Appearing);
				if (ImGui::TreeNode(component->ToString().c_str(), component->ToString().c_str()))
				{
					component->CreateImGuiComponents(aWindowName);
					ImGui::TreePop();
				}
				ImGui::PopID();
				ImGui::Separator();
			}
		}
	}
}
#endif // EDITOR

Json::Value GameObject::ToJson() const
{
	Json::Value result;
	result["IsActive"] = myIsActive;
	result["UUID"] = myUUID.bytes();
	result["Name"] = myName;
	result["Transform"] = myTransform.ToJson();

	if (myParent)
	{
		result["ParentUUID"] = myParent->myUUID.bytes();
	}
	else
	{
		result["ParentUUID"] = "";
	}

	result["Components"] = Json::arrayValue;
	const Component* component = nullptr;
	unsigned i = 0;
	for (auto& [type, index] : myIndexList)
	{
		component = &myComponents.GetValue<Component>(index);
		result["Components"][i] = component->ToJson();
		result["Components"][i].setComment("// " + component->ToString(), Json::commentBefore);
		++i;
	}

	return result;
}

struct GameObjectData
{
	UUIDv4::UUID ID;
	UUIDv4::UUID ParentID;
	unsigned ComponentCount;
	bool IsActive;
};

void GameObject::Serialize(std::ostream& aStream, bool aIsForNetwork) const
{
	GameObjectData data{};
	data.ID = myUUID;
	data.ParentID = myParent ? myParent->myUUID : nullUUID;
	data.ComponentCount = static_cast<unsigned>(myIndexList.size());
	data.IsActive = myIsActive;
	if (!aIsForNetwork)
	{
		Binary::eType type = Binary::GameObject;
		aStream.write(reinterpret_cast<char*>(&type), sizeof(type));
	}
	aStream.write(reinterpret_cast<char*>(&data), sizeof(data));
	aStream.write(myName.c_str(), myName.size() + 1);
	myTransform.Serialize(aStream);

	for (auto& [compType, index] : myIndexList)
	{
		myComponents.GetValue<Component>(index).Serialize(aStream);
	}
}

UUIDv4::UUID GameObject::Deserialize(std::istream& aStream)
{
	GameObjectData data{};
	aStream.read(reinterpret_cast<char*>(&data), sizeof(data));
	const_cast<UUIDv4::UUID&>(myUUID) = data.ID;
	myIsActive = data.IsActive;
	std::getline(aStream, myName, '\0');
	myTransform.Deserialize(aStream);

	for (unsigned i = 0; i < data.ComponentCount; i++)
	{
		Binary::eType type = Binary::Unknown;
		aStream.read(reinterpret_cast<char*>(&type), sizeof(type));
		if (type != Binary::Component)
		{
			throw std::runtime_error("GameObject::Deserialize: Invalid Binary::Type when loading components. ID: " + myUUID.str());
		}
		LoadComponent(aStream, *this);
	}
	return data.ParentID;
}

void GameObject::CopyUuidOf(const GameObject& anObject)
{
	const_cast<UUIDv4::UUID&>(myUUID) = anObject.myUUID;
}

std::string GameObject::GetParentID(const Json::Value& aJson)
{
	return aJson["ParentUUID"].isNull() ? "" : aJson["ParentUUID"].asString();
}