#pragma once
#include "../Component.h"

// Created to fulfill needs for school assignments
BEGIN_COMPONENT(AssignmentComponent)
public:
	AssignmentComponent();
	AssignmentComponent(const Crimson::Vector3f& aDirection);
	AssignmentComponent(const AssignmentComponent& aComponent) = default;
	AssignmentComponent(AssignmentComponent&& aComponent) noexcept = default;
	AssignmentComponent(const Json::Value& aJson);
	~AssignmentComponent() = default;

	AssignmentComponent& operator=(const AssignmentComponent& aComponent) = default;
	AssignmentComponent& operator=(AssignmentComponent&& aComponent) noexcept = default;

	// void Init(GameObject* aParent) override;
	void Update() override;
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
	float myLifeTime;
	Crimson::Vector3f myDirection;
};