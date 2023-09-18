#pragma once
#include <Container/Blackboard.hpp>
#include <JsonVector.hpp>
#include "ComponentType.h"
#include "ThirdParty/DearImGui/ImGui/imgui.h"
#include "ThirdParty/DearImGui/ImGui/imgui_stdlib.h"

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
	Component(Component&& aComponent) noexcept;
	Component(const Json::Value& aJson);
	virtual ~Component() = default;

	virtual Component& operator=(const Component& aComponent);
	virtual Component& operator=(Component&& aComponent) noexcept;

	virtual void Init(const Json::Value& aJson);
	virtual void Init(GameObject* aParent);
	virtual void Update();

	const GameObject& GetParent() const;
	GameObject& GetParent();

	ComponentType GetType() const;
	unsigned GetParentID() const;
	unsigned GetComponentID() const;

	virtual void SetActive(bool aIsActive);
	virtual void ToogleActive();
	bool IsActive() const;

	//virtual void SetValue(unsigned aSlot)

	virtual void ComponentPointersInvalidated();
	virtual void CreateImGuiComponents(const std::string& aWindowName);
	virtual Json::Value ToJson() const;
	virtual const Component* GetTypePointer() const;

	// Only call before creating another Component!
	void MarkAsPrefabComponent(unsigned anID = 0);
	static void SetIDCount(unsigned aValue) { localIDCount = aValue; }
	static unsigned GetIDCount() { return localIDCount; }

protected:
	GameObject* myParent;
	ComponentType myType;
	const unsigned myID;
	bool myIsActive;

	static unsigned localIDCount;

	const CommonUtilities::Blackboard<unsigned int>& GetComponentContainer() const;
	CommonUtilities::Blackboard<unsigned int>& GetComponentContainer();

	virtual void NotifyInput(CommonUtilities::eInputAction anEvent);
};