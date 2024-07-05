#pragma once
#include "../Component.h"

class ScriptGraph;

BEGIN_COMPONENT(ScriptComponent)
public:
	ScriptComponent();
	ScriptComponent(const ScriptComponent& aComponent);
	ScriptComponent(ScriptComponent&& aComponent) noexcept;
	ScriptComponent(const Json::Value& aJson);
	~ScriptComponent() = default;

	ScriptComponent& operator=(const ScriptComponent& aComponent);
	ScriptComponent& operator=(ScriptComponent&& aComponent) noexcept;

	void Update() override;

	void Init(GameObject* aParent) override;

	std::shared_ptr<ScriptGraph> GetScriptGraph();

	void OnCollisionEnter(CollisionLayer::Layer aLayer, ColliderComponent* aCollider) override;
	void OnCollisionStay(CollisionLayer::Layer aLayer, ColliderComponent* aCollider) override;
	void OnCollisionExit(CollisionLayer::Layer aLayer, ColliderComponent* aCollider) override;

	void OnTriggerEnter(CollisionLayer::Layer aLayer, ColliderComponent* aCollider) override;
	void OnTriggerStay(CollisionLayer::Layer aLayer, ColliderComponent* aCollider) override;
	void OnTriggerExit(CollisionLayer::Layer aLayer, ColliderComponent* aCollider) override;

	// void TransformHasChanged() const override;

	void CreateImGuiComponents() override;
	Json::Value ToJson() const override;

private:
	std::shared_ptr<ScriptGraph> myScriptGraph;
	std::string myScriptPath;
};