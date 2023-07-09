#pragma once
#include <Container/Blackboard.hpp>

class GameObject;

class Component
{
public:
	Component();
	Component(const Component& aComponent);
	virtual ~Component() = default;

	virtual Component& operator=(const Component& aComponent);

	virtual void Init(GameObject* aParent);
	virtual void Update();

	const GameObject& GetParent() const;
	GameObject& GetParent();

	virtual void SetActive(bool aIsActive);
	virtual void ToogleActive();
	bool IsActive() const;

	virtual void ComponentPointersInvalidated();

	virtual const Component* GetTypePointer() const = 0;

protected:
	GameObject* myParent;
	bool myIsActive;

	const CommonUtilities::Blackboard<unsigned int>& GetComponentContainer() const;
	CommonUtilities::Blackboard<unsigned int>& GetComponentContainer();
};