#pragma once
#include "Components/Component.h"
#include "Components/Transform.h"
#include <fstream>

class Prefab;

class GameObject
{
public:
	GameObject();
	GameObject(const Prefab& aPrefab);
	GameObject(const GameObject& aGameObject);
	GameObject(GameObject&& aGameObject) noexcept;
	GameObject(const Json::Value& aJson);
	~GameObject() = default;

	GameObject& operator=(const Prefab& aPrefab);
	GameObject& operator=(const GameObject& aGameObject);
	GameObject& operator=(GameObject&& aGameObject) noexcept;

	void Update();

	template<class T>
	T& AddComponent();
	template<class T>
	T& AddComponent(const T& aComponent);

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

	void SetPosition(const CommonUtilities::Vector3f& aPosition);
	void SetRotation(const CommonUtilities::Vector3f& aRotation);
	void SetScale(const CommonUtilities::Vector3f& aScale);

	const Transform& GetTransform() const;
	const CommonUtilities::Matrix4x4f& GetTransformMatrix() const;
	const CommonUtilities::Vector3f& GetWorldPosition() const;

	void SetActive(bool aIsActive);
	void ToogleActive();
	bool IsActive() const;

	void SetName(const std::string& aName);
	const std::string& GetName() const;

	unsigned GetComponentCount() const;
	unsigned GetID() const;

	void CreateImGuiWindowContent(const std::string& aWindowName);
	Json::Value ToJson() const;
	//void ToBinary(std::ofstream& aStream) const;

	// Only call before creating another GameObject!
	void MarkAsPrefab();
	// Only call before creating another GameObject!
	void MarkAsPrefab(unsigned anID);

	static void SetIDCount(unsigned aValue) { localIDCount = aValue; }
	static unsigned GetIDCount() { return localIDCount; }

private:
#ifndef _RETAIL
	friend class PrefabManager;
	GameObject(unsigned anID);
#endif // !_RETAIL
	friend class Component;
	static unsigned localIDCount;

	bool myIsActive;
	const unsigned myID;
	unsigned myCount;
	std::string myName;
	std::string myImguiText;
	Transform myTransform;

#ifdef _DEBUG
	std::vector<const Component*> myDebugPointers;
#endif // _DEBUG
	std::unordered_multimap<const std::type_info*, unsigned> myIndexList;
	CommonUtilities::Blackboard<unsigned> myComponents;
};

template<class T>
inline T& GameObject::AddComponent()
{
	if (myComponents.SetValue(myCount, T()))
	{
		for (auto [type, index] : myIndexList)
		{
			myComponents.ChangeValueUnsafe<Component>(index)->ComponentPointersInvalidated();
		}
	}
	myIndexList.emplace(&typeid(T), myCount);
	myComponents.ChangeValueUnsafe<Component>(myCount)->Init(this);
#ifdef _DEBUG
	myDebugPointers.emplace_back(&myComponents.GetValue<T>(myCount));
#endif // _DEBUG
	return myComponents.ChangeValue<T>(myCount++);
}

template<class T>
inline T& GameObject::AddComponent(const T& aComponent)
{
	if (myComponents.SetValue(myCount, aComponent))
	{
		for (auto [type, index] : myIndexList)
		{
			myComponents.ChangeValueUnsafe<Component>(index)->ComponentPointersInvalidated();
		}
	}
	myIndexList.emplace(&typeid(aComponent), myCount);
	myComponents.ChangeValueUnsafe<Component>(myCount)->Init(this);
#ifdef _DEBUG
	myDebugPointers.emplace_back(&myComponents.GetValue<T>(myCount));
#endif // _DEBUG
	return myComponents.ChangeValue<T>(myCount++);
}

template<class T>
inline const T& GameObject::GetComponent() const
{
	auto iter = myIndexList.equal_range(&typeid(T));
	return myComponents.GetValue<T>(iter.first->second);
}

template<class T>
inline T& GameObject::GetComponent()
{
	auto iter = myIndexList.equal_range(&typeid(T));
	return myComponents.ChangeValue<T>(iter.first->second);
}

template<class T>
inline const T* GameObject::GetComponent(unsigned anID) const
{
	auto range = myIndexList.equal_range(&typeid(T));
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
	auto range = myIndexList.equal_range(&typeid(T));
	for (auto iter = range.first; iter != range.second; iter++)
	{
		if (T& component = myComponents.ChangeValue<T>(iter->second); component.GetComponentID() == anID)
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
	auto range = myIndexList.equal_range(&typeid(T));
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
	auto range = myIndexList.equal_range(&typeid(T));
	for (auto iter = range.first; iter != range.second; iter++)
	{
		result.emplace_back(&myComponents.ChangeValue<T>(iter->second));
	}
	return result;
}

template<class T>
inline bool GameObject::RemoveComponent(unsigned anID)
{
	auto range = myIndexList.equal_range(&typeid(T));
	for (auto iter = range.first; iter != range.second; iter++)
	{
		if (const T& component = myComponents.GetValue<T>(iter->second); component.GetComponentID() == anID)
		{
			myIndexList.erase(iter);
			return true;
		}
	}
	return false;
}

template<class T>
inline bool GameObject::RemoveComponent(const T* aComponent)
{
	auto range = myIndexList.equal_range(&typeid(T));
	for (auto iter = range.first; iter != range.second; iter++)
	{
		if (const T* component = &myComponents.GetValue<T>(iter->second); component == aComponent)
		{
			myIndexList.erase(iter);
			return true;
		}
	}
	return false;
}

template<class T>
inline bool GameObject::HasComponent() const
{
	auto iter = myIndexList.find(&typeid(T));
	return iter != myIndexList.end();
}