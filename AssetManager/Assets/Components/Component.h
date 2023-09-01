#pragma once
#include <Container/Blackboard.hpp>
#include <JsonVector.hpp>
#include "ComponentType.h"

// A template to build new components can be found in ComponentTemplate.h

class GameObject;
namespace CommonUtilities
{
	enum class eInputAction : int;
}

class Component
{
public:
	Component();
	Component(ComponentType aType);
	Component(const Component& aComponent);
	Component(const Json::Value& aJson, GameObject* aParent);
	virtual ~Component() = default;

	virtual Component& operator=(const Component& aComponent);

	virtual void Init(const Json::Value& aJson);
	virtual void Init(GameObject* aParent);
	virtual void Update();

	const GameObject& GetParent() const;
	GameObject& GetParent();

	ComponentType GetType() const;

	virtual void SetActive(bool aIsActive);
	virtual void ToogleActive();
	bool IsActive() const;

	virtual void ComponentPointersInvalidated();

	virtual Json::Value ToJson() const;
	virtual const Component* GetTypePointer() const;

protected:
	GameObject* myParent;
	ComponentType myType;
	bool myIsActive;

	const CommonUtilities::Blackboard<unsigned int>& GetComponentContainer() const;
	CommonUtilities::Blackboard<unsigned int>& GetComponentContainer();

	virtual void NotifyInput(CommonUtilities::eInputAction anEvent);
};