#pragma once
#include <typeindex>
#include "../Binary.h"
#include "ComponentType.h"
#include "ComponentMacros.h"
#pragma warning (push,0)
#include "CrimsonUtilities/UUID/uuid_v4.h"
#pragma warning (pop)

#pragma warning(push)
#pragma warning(disable: 4100) // 4100 = Unreferenced Parameter

namespace CollisionLayer
{
	enum Layer : unsigned;
}
class ColliderComponent;
class Transform;
namespace Network { struct GameObjectMessage; }

// A template to build new components can be found in ComponentTemplate.h
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

	// Called when bound to a GameObject
	virtual void Init(GameObject* aParent);

	virtual void Update(){}
	virtual void Render(){}
	virtual void DebugDraw(){}

	virtual void OnCollisionEnter(CollisionLayer::Layer aLayer, ColliderComponent* aComponent){}
	virtual void OnCollisionStay(CollisionLayer::Layer aLayer, ColliderComponent* aComponent) {}
	virtual void OnCollisionExit(CollisionLayer::Layer aLayer, ColliderComponent* aComponent) {}

	virtual void OnTriggerEnter(CollisionLayer::Layer aLayer, ColliderComponent* aComponent) {}
	virtual void OnTriggerStay(CollisionLayer::Layer aLayer, ColliderComponent* aComponent) {}
	virtual void OnTriggerExit(CollisionLayer::Layer aLayer, ColliderComponent* aComponent){}

	virtual void RecieveNetmessage(const Network::GameObjectMessage& aMessage) {}

	// Only use this when Init(GameObject*) can not be called!
	void SetParent(GameObject* aParent);
	const GameObject& GetParent() const;
	GameObject& GetParent();

	ComponentType GetType() const;
	const UUIDv4::UUID& GetParentID() const;

	virtual void SetActive(bool aIsActive);
	virtual void ToogleActive();
	bool IsActive() const;

	virtual void TransformHasChanged() const{}

#ifdef EDITOR
	virtual void CreateImGuiComponents(const std::string& aWindowName);
#endif // EDITOR

	inline std::string ToString() const { return ComponentTypeToString(myType); }
	virtual Json::Value ToJson() const;

	virtual void Serialize(std::ostream& aStream) const;
	virtual void Deserialize(std::istream& aStream);

	virtual inline const Component* GetTypePointer() const { return this; }
	virtual inline Component* GetTypePointer() { return this; }

	virtual inline void MoveTo(void* aTarget) = 0;
	virtual inline void CopyTo(void* aTarget) const = 0;

	virtual inline size_t SizeOf() const = 0;
	virtual inline std::type_index TypeId() const = 0;

protected:
	GameObject* myParent;
	ComponentType myType;
	bool myIsActive;

	const Transform* GetParentTransform() const;
	Transform* GetParentTransform();
};

#pragma warning(pop)