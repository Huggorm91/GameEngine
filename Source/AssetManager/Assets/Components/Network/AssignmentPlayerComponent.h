#pragma once
#include "../Component.h"

// Created to fulfill needs for school assignments
// Moving Orbs
BEGIN_COMPONENT(AssignmentPlayerComponent)
public:
	AssignmentPlayerComponent();
	AssignmentPlayerComponent(const AssignmentPlayerComponent& aComponent) = default;
	AssignmentPlayerComponent(AssignmentPlayerComponent&& aComponent) noexcept = default;
	AssignmentPlayerComponent(const Json::Value& aJson);
	~AssignmentPlayerComponent() = default;

	AssignmentPlayerComponent& operator=(const AssignmentPlayerComponent& aComponent) = default;
	AssignmentPlayerComponent& operator=(AssignmentPlayerComponent&& aComponent) noexcept = default;

	//void Init(GameObject* aParent) override;
	// void Update() override;
	// void DebugDraw() override;

	// void OnCollisionEnter(CollisionLayer::Layer aLayer, ColliderComponent* aCollider) override;
	// void OnCollisionStay(CollisionLayer::Layer aLayer, ColliderComponent* aCollider) override;
	// void OnCollisionExit(CollisionLayer::Layer aLayer, ColliderComponent* aCollider) override;

	// void OnTriggerEnter(CollisionLayer::Layer aLayer, ColliderComponent* aCollider) override;
	// void OnTriggerStay(CollisionLayer::Layer aLayer, ColliderComponent* aCollider) override;
	// void OnTriggerExit(CollisionLayer::Layer aLayer, ColliderComponent* aCollider) override;

	// void RecieveNetmessage(const Network::GameObjectMessage& aMessage) override;

	// void SetActive(bool aIsActive) override;
	// void ToogleActive() override;

	// void TransformHasChanged() const override;

	// Json::Value ToJson() const override;

private:
	
};