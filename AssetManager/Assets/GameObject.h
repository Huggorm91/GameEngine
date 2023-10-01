#pragma once
#include "Components/Component.h"
#include "Components/Transform.h"
#include <fstream>

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

	void Update();

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

	void SetPosition(const CommonUtilities::Vector3f& aPosition);
	void SetRotation(const CommonUtilities::Vector3f& aRotation);
	void SetScale(const CommonUtilities::Vector3f& aScale);

	const Transform& GetTransform() const;
	const CommonUtilities::Matrix4x4f& GetTransformMatrix() const;
	const CommonUtilities::Vector4f& GetWorldPosition() const;

	void SetActive(bool aIsActive);
	void ToogleActive();
	bool IsActive() const;

	bool HasChild() const;
	bool HasParent() const;

	const GameObject* GetParent() const;
	GameObject* GetParent();

	void RemoveParent();
#ifndef _RETAIL
	void AddChild(std::shared_ptr<GameObject> anObject);
	void RemoveChild(std::shared_ptr<GameObject> anObject);

	const std::vector<std::shared_ptr<GameObject>>& GetChildren() const;
	std::vector<std::shared_ptr<GameObject>>& GetChildren();
#else
	void AddChild(GameObject* anObject);
	void RemoveChild(GameObject* anObject);

	const std::vector<GameObject*>& GetChildren() const;
	std::vector<GameObject*>& GetChildren();
#endif // !_RETAIL

	void SetName(const std::string& aName);
	const std::string& GetName() const;

	unsigned GetComponentCount() const;
	unsigned GetID() const;
	const unsigned& GetIDRef() const;

	void CreateImGuiWindowContent(const std::string& aWindowName);
	Json::Value ToJson() const;
	//void ToBinary(std::ofstream& aStream) const;

	// Only call before creating another GameObject!
	void MarkAsPrefab();
	// Only call before creating another GameObject!
	void MarkAsPrefab(unsigned anID);

	static unsigned GetIDCount() { return localIDCount; }

private:
#ifndef _RETAIL
	friend class PrefabManager;
	GameObject(unsigned anID);
#endif // !_RETAIL
	friend void SetGameObjectIDCount(unsigned aValue);
	friend class Component;
	static unsigned localIDCount;

	bool myIsActive;
	const unsigned myID;
	unsigned myCount;

	GameObject* myParent;

	std::string myName;
	std::string myImguiText;
	Transform myTransform;

#ifndef _RETAIL
	std::vector<std::shared_ptr<GameObject>> myChildren;
	std::vector<const Component*> myDebugPointers;
#else
	std::vector<GameObject*> myChildren;
#endif // !_RETAIL
	std::unordered_multimap<const std::type_info*, unsigned> myIndexList;
	CommonUtilities::Blackboard<unsigned> myComponents;

	void SetParent(GameObject*);

	void RemoveParentInternal();
	void RemoveFromParent();

	void TransformHasChanged();
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
#ifndef _RETAIL
	myDebugPointers.emplace_back(&myComponents.GetValue<T>(myCount));
#endif // !_RETAIL
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
#ifndef _RETAIL
	myDebugPointers.emplace_back(&myComponents.GetValue<T>(myCount));
#endif // !_RETAIL
	return myComponents.ChangeValue<T>(myCount++);
}

template<class T>
inline T& GameObject::AddComponent(T&& aComponent)
{
	if (myComponents.SetValue(myCount, std::move(aComponent)))
	{
		for (auto [type, index] : myIndexList)
		{
			myComponents.ChangeValueUnsafe<Component>(index)->ComponentPointersInvalidated();
		}
	}
	myIndexList.emplace(&typeid(aComponent), myCount);
	myComponents.ChangeValueUnsafe<Component>(myCount)->Init(this);
#ifndef _RETAIL
	myDebugPointers.emplace_back(&myComponents.GetValue<T>(myCount));
#endif // !_RETAIL
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