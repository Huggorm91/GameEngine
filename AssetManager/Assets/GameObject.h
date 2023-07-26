#pragma once
#include "Components/Component.h"
#include "Components/Transform.h"

class GameObject
{
public:
	GameObject();
	GameObject(const GameObject& aGameObject);
	~GameObject() = default;

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
	std::vector<const T*> GetComponents() const;
	template<class T>
	std::vector<T*> GetComponents();

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

	unsigned int GetComponentCount() const;
	unsigned int GetID() const;

private:
	friend class Component;

	CommonUtilities::Blackboard<unsigned int> myComponents;
	std::unordered_multimap<const std::type_info*, unsigned int> myIndexList;
	Transform myTransform;
	unsigned int myCount;
	const unsigned int myID;
	bool myIsActive;

	static unsigned int localIDCount;
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
inline bool GameObject::HasComponent() const
{
	auto iter = myIndexList.find(&typeid(T));
	return iter != myIndexList.end();
}
