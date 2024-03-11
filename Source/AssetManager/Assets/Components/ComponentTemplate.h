// The entire below document is only meant as a way to quickly start building a new component. 
// Simply copy the code from #pragma once to the end of file and replace Example with your new name

// Functions that appear as comments are optional

#pragma once
#include "Component.h"

	// TODO:
	// 1. Add #include "Subfolder/ExampleComponent.h" to ComponentInclude.h
	// 2. Add enum 'ExampleComponent' in ComponentType.h
	// 3. Add 'case ComponentType::Example: { aParent.AddComponent<ExampleComponent>(); break; }' to switch in AddComponent in ComponentType.cpp
	// 4. Add 'case ComponentType::Example: { aParent.AddComponent(ExampleComponent(aJson)); break; }' to switch in LoadComponent in ComponentType.cpp
	// 5. Add 'case ComponentType::Example: { auto& example = aParent.AddComponent<ExampleComponent>(); example.Deserialize(aStream); break; }' to switch in LoadComponent in ComponentType.cpp
	// 6. Add 'case ComponentType::Example: { return "Example"; }' to switch in ComponentTypeToString() in ComponentType.cpp
BEGIN_COMPONENT(ExampleComponent)
// BEGIN_COMPONENT(ExampleComponent, ParentComponent)
// BEGIN_COMPONENT_MULTI_INHERITANCE(ExampleComponent, ParentComponent, public Crimson::InputObserver)
// BEGIN_COMPONENT_ABSTRACT(ExampleComponent)
public:
	ExampleComponent(); // Use base constructor: Component(ComponentType::Example)
	ExampleComponent(const ExampleComponent& aComponent) = default;
	ExampleComponent(ExampleComponent&& aComponent) noexcept = default;
	ExampleComponent(const Json::Value& aJson);
	~ExampleComponent() = default;

	ExampleComponent& operator=(const ExampleComponent& aComponent) = default;
	ExampleComponent& operator=(ExampleComponent&& aComponent) noexcept = default;

	// void Init(GameObject* aParent) override;
	// void Update() override;
	// void DebugDraw() override;

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

private:

};