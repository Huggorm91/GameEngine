#pragma once
#include "Container/Blackboard.hpp"
#include "Json/JsonVector.hpp"
#include "ComponentType.h"
#include "ThirdParty/DearImGui/ImGui/imgui.h"
#include "ThirdParty/DearImGui/ImGui/imgui_stdlib.h"

// A template to build new components can be found in ComponentTemplate.h

class GameObject;
class Transform;

class Component
{
public:
	Component();
	Component(ComponentType aType);
	Component(const Component& aComponent);
	Component(Component&& aComponent) noexcept;
	Component(const Json::Value& aJson);
	virtual ~Component() = default;

	Component& operator=(const Component& aComponent);
	Component& operator=(Component&& aComponent) noexcept;

	virtual void Init(const Json::Value& aJson);
	virtual void Init(GameObject* aParent);
	virtual void Update();
	virtual void Render();

	const GameObject& GetParent() const;
	GameObject& GetParent();

	ComponentType GetType() const;
	unsigned GetParentID() const;
	unsigned GetComponentID() const;

	virtual void SetActive(bool aIsActive);
	virtual void ToogleActive();
	bool IsActive() const;

	virtual void TransformHasChanged() const;

	virtual void CreateImGuiComponents(const std::string& aWindowName);
	virtual inline std::string ToString() const;
	virtual Json::Value ToJson() const;
	virtual inline const Component* GetTypePointer() const;

	// Only call before creating another Component!
	void MarkAsPrefabComponent(unsigned anID = 0);
	void CopyID(const Component* aComponent, bool aDecrementIDCount = false);

	static void SetIDCount(unsigned aValue) { localIDCount = aValue; }
	static unsigned GetIDCount() { return localIDCount; }

protected:
	GameObject* myParent;
	ComponentType myType;
	const unsigned myID;
	bool myIsActive;

	static unsigned localIDCount;

	const Crimson::Blackboard<unsigned int>& GetComponentContainer() const;
	Crimson::Blackboard<unsigned int>& GetComponentContainer();

	const Transform* GetParentTransform() const;
	Transform* GetParentTransform();
};