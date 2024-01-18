// The entire below document is only meant as a way to quickly start building a new component. 
// Simply copy the code from #pragma once to the end of file and replace Example with your new name

#pragma once
#include "Component.h"

class ExampleComponent : public Component
{
	// TODO:
	// 1. Add #include "Components/ExampleComponent.h" to ComponentInclude.h
	// 2. Add enum for new ComponentType in ComponentType.h
	// 3. Add 'case ComponentType::Example: { const ExampleComponent& component = *dynamic_cast<const ExampleComponent*>(aComponent); aParent.AddComponent(component); break; }' to switch in AddComponent() in ComponentType.cpp
	// 4. Add 'case ComponentType::Example: { break; }' to switch in LoadComponent() in ComponentType.cpp, and implement Constructor(Json::Value) and/or Init(Json::Value)
public:
	ExampleComponent(); // Use base constructor: Component(ComponentType::Example)
	ExampleComponent(const ExampleComponent& aComponent) = default;
	ExampleComponent(ExampleComponent&& aComponent) noexcept = default;
	ExampleComponent(const Json::Value& aJson); // Init(const Json::Value& aJson) can be used as an alternative
	~ExampleComponent() = default;

	ExampleComponent& operator=(const ExampleComponent& aComponent) = default;
	ExampleComponent& operator=(ExampleComponent&& aComponent) noexcept = default;

	// void Init(const Json::Value& aJson) override;
	// void Init(GameObject* aParent) override;
	// void Update() override;
	// void Render() override;

	// void OnCollisionEnter(eCollisionLayer aLayer, ColliderComponent* aCollider) override;
	// void OnCollisionStay(eCollisionLayer aLayer, ColliderComponent* aCollider) override;
	// void OnCollisionExit(eCollisionLayer aLayer, ColliderComponent* aCollider) override;

	// void OnTriggerEnter(eCollisionLayer aLayer, ColliderComponent* aCollider) override;
	// void OnTriggerStay(eCollisionLayer aLayer, ColliderComponent* aCollider) override;
	// void OnTriggerExit(eCollisionLayer aLayer, ColliderComponent* aCollider) override;

	// void SetActive(bool aIsActive) override;
	// void ToogleActive() override;

	// void TransformHasChanged() const override;

	void CreateImGuiComponents(const std::string& aWindowName) override;
	Json::Value ToJson() const override;
	inline std::string ToString() const override;
	inline const ExampleComponent* GetTypePointer() const override; // return this;

private:

};