#pragma once
#include "../Component.h"

class Blueprint;

class ScriptComponent : public Component
{
public:
	ScriptComponent();
	ScriptComponent(const ScriptComponent& aComponent) = default;
	ScriptComponent(ScriptComponent&& aComponent) noexcept = default;
	ScriptComponent(const Json::Value& aJson);
	~ScriptComponent() = default;

	ScriptComponent& operator=(const ScriptComponent& aComponent) = default;
	ScriptComponent& operator=(ScriptComponent&& aComponent) noexcept = default;

	// void Init(GameObject* aParent) override;
	void Update() override;

	// Potential future event triggers
	// void OnCollisionEnter(eCollisionLayer aLayer, ColliderComponent* aCollider) override;
	// void OnCollisionStay(eCollisionLayer aLayer, ColliderComponent* aCollider) override;
	// void OnCollisionExit(eCollisionLayer aLayer, ColliderComponent* aCollider) override;
	// 
	// Potential future event triggers
	// void OnTriggerEnter(eCollisionLayer aLayer, ColliderComponent* aCollider) override;
	// void OnTriggerStay(eCollisionLayer aLayer, ColliderComponent* aCollider) override;
	// void OnTriggerExit(eCollisionLayer aLayer, ColliderComponent* aCollider) override;

	// Potential future event triggers
	// void SetActive(bool aIsActive) override;
	// void ToogleActive() override;
	// void TransformHasChanged() const override;

	void CreateImGuiComponents(const std::string& aWindowName) override;
	Json::Value ToJson() const override;
	void Serialize(std::ostream& aStream) const override;
	void Deserialize(std::istream& aStream) override;

	inline std::string ToString() const override;
	inline const ScriptComponent* GetTypePointer() const override;

private:
	std::shared_ptr<Blueprint> myScript;
};