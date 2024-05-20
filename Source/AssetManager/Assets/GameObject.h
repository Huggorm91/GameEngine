#pragma once
#include "Container/MemoryBlock.h"
#include "Components/Component.h"
#include "Math/Transform.h"

#include "Components/Light/DirectionallightComponent.h"

class Prefab;
void SetGameObjectIDCount(unsigned aValue);

class GameObject
{
public:
	GameObject();
	GameObject(const Prefab& aPrefab);
	GameObject(const GameObject& aGameObject);
	GameObject(GameObject&& aGameObject) noexcept;
	GameObject(const Json::Value& aJson);
	~GameObject();

	GameObject& operator=(const Prefab& aPrefab);
	GameObject& operator=(const GameObject& aGameObject);
	GameObject& operator=(GameObject&& aGameObject) noexcept;

	bool operator==(const GameObject& aGameObject);

	void Update();
	void Render();
	void DebugDraw();

	template<class T>
	T& AddComponent();
	template<class T>
	T& AddComponent(const T& aComponent);
	template<class T>
	T& AddComponent(T&& aComponent);

	template<class T>
	const T& GetComponent() const;
	template<class T>
	T& GetComponent();
	template<class T>
	const T* GetComponent(unsigned anID) const;
	template<class T>
	T* GetComponent(unsigned anID);

	const Component* GetComponentPointer(unsigned anID) const;
	Component* GetComponentPointer(unsigned anID);

	template<class T>
	std::vector<const T*> GetComponents() const;
	template<class T>
	std::vector<T*> GetComponents();

	template<class T>
	bool RemoveComponent(unsigned anID);
	template<class T>
	bool RemoveComponent(const T* aComponent);

	template<class T>
	bool HasComponent() const;

	void OnCollisionEnter(CollisionLayer::Layer aLayer, ColliderComponent* aCollider);
	void OnCollisionStay(CollisionLayer::Layer aLayer, ColliderComponent* aCollider);
	void OnCollisionExit(CollisionLayer::Layer aLayer, ColliderComponent* aCollider);

	void OnTriggerEnter(CollisionLayer::Layer aLayer, ColliderComponent* aTrigger);
	void OnTriggerStay(CollisionLayer::Layer aLayer, ColliderComponent* aTrigger);
	void OnTriggerExit(CollisionLayer::Layer aLayer, ColliderComponent* aTrigger);

	void SetPosition(const Crimson::Vector3f& aPosition);
	void SetRotation(const Crimson::Vector3f& aDegree);
	void SetScale(const Crimson::Vector3f& aScale);

	const Transform& GetTransform() const;
	const Crimson::Matrix4x4f& GetTransformMatrix() const;
	const Crimson::Vector4f& GetWorldPosition() const;

	void SetActive(bool aIsActive);
	void ToogleActive();
	bool IsActive() const;

	void SetActiveComponents(bool aIsActive);
	void ToogleActiveComponents();

	bool HasChild() const;
	bool HasParent() const;

	const GameObject* GetParent() const;
	GameObject* GetParent();
	void RemoveParent();

	void AddChild(GameObject* anObject);
	void RemoveChild(GameObject* anObject);

	const std::vector<GameObject*>& GetChildren() const;
	std::vector<GameObject*>& GetChildren();

	void SetName(const std::string& aName);
	const std::string& GetName() const;

	// returns "{Name}: {ID}"
	std::string ToString() const;

	unsigned GetComponentCount() const;
	unsigned GetID() const;

	void CreateImGuiWindowContent(const std::string& aWindowName);
	Json::Value ToJson() const;

	void Serialize(std::ostream& aStream) const;
	// Returns parent ID. Has no parent if 0.
	unsigned Deserialize(std::istream& aStream);

	// Only call before creating another GameObject!
	void MarkAsPrefab();
	// Only call before creating another GameObject!
	void MarkAsPrefab(unsigned anID);

	// Excpects GameObjects to already be copies of eachother.
	void CopyIDsOf(const GameObject& anObject, bool aDecrementIDCount = false);

	static unsigned GetParentID(const Json::Value& aJson);
	static unsigned GetIDCount() { return localIDCount; }

private:
#ifdef EDITOR
	friend class PrefabManager;
	GameObject(unsigned anID);
#endif // EDITOR
	friend void SetGameObjectIDCount(unsigned aValue);
	friend class Component;

	static unsigned localIDCount;

	bool myIsActive;
	const unsigned myID;

	GameObject* myParent;

	std::string myName;
#ifdef EDITOR
	std::string myImguiText;
#endif // EDITOR

	Transform myTransform;

	std::vector<GameObject*> myChildren;

#ifndef _RETAIL
	std::vector<Component*> myDebugPointers;
#endif // !_RETAIL
	std::unordered_multimap<std::type_index, size_t> myIndexList;
	Crimson::MemoryBlock myComponents;

	Component* AllocateComponent(const Component* aComponent);
	template<class T>
	T& AddComponentInternal(const size_t& anIndex);

	void SetParent(GameObject* anObject);

	void RemoveParentInternal();
	void RemoveFromParent();

	void TransformHasChanged();

	bool IsRelated(GameObject* anObject);
	bool IsChildRecursive(GameObject* anObject);
	bool IsChild(GameObject* anObject);
	bool IsParentRecursive(GameObject* anObject);
	bool IsParent(GameObject* anObject);
};

template<class T>
inline T& GameObject::AddComponent()
{
	static_assert(std::is_base_of_v<Component, T> && "Trying to add something that isnt a Component!");
	auto index = myComponents.AddValue(T());
	return AddComponentInternal<T>(index);
}

template<class T>
inline T& GameObject::AddComponent(const T& aComponent)
{
	static_assert(std::is_base_of_v<Component, T> && "Trying to add something that isnt a Component!");
	auto index = myComponents.AddValue(aComponent);
	return AddComponentInternal<T>(index);
}

template<class T>
inline T& GameObject::AddComponent(T&& aComponent)
{
	static_assert(std::is_base_of_v<Component, T> && "Trying to add something that isnt a Component!");
	auto index = myComponents.AddValue(std::move(aComponent));
	return AddComponentInternal<T>(index);
}

template<class T>
inline const T& GameObject::GetComponent() const
{
	auto iter = myIndexList.equal_range(typeid(T));
	return myComponents.GetValue<T>(iter.first->second);
}

template<class T>
inline T& GameObject::GetComponent()
{
	auto iter = myIndexList.equal_range(typeid(T));
	return myComponents.GetValue<T>(iter.first->second);
}

template<class T>
inline const T* GameObject::GetComponent(unsigned anID) const
{
	auto range = myIndexList.equal_range(typeid(T));
	for (auto iter = range.first; iter != range.second; iter++)
	{
		if (const T& component = myComponents.GetValue<T>(iter->second); component.GetComponentID() == anID)
		{
			return &component;
		}
	}
	return nullptr;
}

template<class T>
inline T* GameObject::GetComponent(unsigned anID)
{
	auto range = myIndexList.equal_range(typeid(T));
	for (auto iter = range.first; iter != range.second; iter++)
	{
		if (T& component = myComponents.GetValue<T>(iter->second); component.GetComponentID() == anID)
		{
			return &component;
		}
	}
	return nullptr;
}

template<class T>
inline std::vector<const T*> GameObject::GetComponents() const
{
	std::vector<const T*> result;
	auto range = myIndexList.equal_range(typeid(T));
	for (auto iter = range.first; iter != range.second; iter++)
	{
		result.emplace_back(&myComponents.GetValue<T>(iter->second));
	}
	return result;
}

template<class T>
inline std::vector<T*> GameObject::GetComponents()
{
	std::vector<T*> result;
	auto range = myIndexList.equal_range(typeid(T));
	for (auto iter = range.first; iter != range.second; iter++)
	{
		result.emplace_back(&myComponents.GetValue<T>(iter->second));
	}
	return result;
}

template<class T>
inline bool GameObject::RemoveComponent(unsigned anID)
{
	auto range = myIndexList.equal_range(typeid(T));
	for (auto iter = range.first; iter != range.second; iter++)
	{
		if (T& component = myComponents.GetValue<T>(iter->second); component.GetComponentID() == anID)
		{
#ifdef GAME
			component.~T();
#endif // GAME
			myIndexList.erase(iter);
			return true;
		}
	}
	return false;
}

template<class T>
inline bool GameObject::RemoveComponent(const T* aComponent)
{
	auto range = myIndexList.equal_range(typeid(T));
	for (auto iter = range.first; iter != range.second; iter++)
	{
		if (T* component = &myComponents.GetValue<T>(iter->second); component == aComponent)
		{
#ifdef GAME
			component->~T();
#endif // GAME
			myIndexList.erase(iter);
			return true;
		}
	}
	return false;
}

template<class T>
inline bool GameObject::HasComponent() const
{
	auto iter = myIndexList.find(typeid(T));
	return iter != myIndexList.end();
}

template<class T>
inline T& GameObject::AddComponentInternal(const size_t& anIndex)
{
	myIndexList.emplace(typeid(T), anIndex);

	T& component = myComponents.GetValue<T>(anIndex);
	component.Init(this);
#ifndef _RETAIL
	myDebugPointers.emplace_back(&component);
#endif // !_RETAIL
	return component;
}