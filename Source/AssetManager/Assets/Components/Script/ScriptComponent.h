#pragma once
#include "../Component.h"

class ScriptGraph;

BEGIN_COMPONENT(ScriptComponent)
public:
	ScriptComponent();
	ScriptComponent(const ScriptComponent& aComponent) = default;
	ScriptComponent(ScriptComponent&& aComponent) noexcept = default;
	ScriptComponent(const Json::Value& aJson);
	~ScriptComponent() = default;

	ScriptComponent& operator=(const ScriptComponent& aComponent) = default;
	ScriptComponent& operator=(ScriptComponent&& aComponent) noexcept = default;

	void Update() override;

	void Init(GameObject* aParent) override;

	std::shared_ptr<ScriptGraph> GetScriptGraph();

	// void OnCollisionEnter(eCollisionLayer aLayer, ColliderComponent* aCollider) override;
	// void OnCollisionStay(eCollisionLayer aLayer, ColliderComponent* aCollider) override;
	// void OnCollisionExit(eCollisionLayer aLayer, ColliderComponent* aCollider) override;

	// void OnTriggerEnter(eCollisionLayer aLayer, ColliderComponent* aCollider) override;
	// void OnTriggerStay(eCollisionLayer aLayer, ColliderComponent* aCollider) override;
	// void OnTriggerExit(eCollisionLayer aLayer, ColliderComponent* aCollider) override;

	// void TransformHasChanged() const override;

	void CreateImGuiComponents() override;
	Json::Value ToJson() const override;

private:
	std::shared_ptr<ScriptGraph> myScriptGraph;
	std::string myScriptPath;
};