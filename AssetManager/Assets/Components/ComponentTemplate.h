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
	// 5. Add 'case ComponentType::Example: return "Example";' to switch in ComponentTypeToString() in ComponentType.cpp
public:
	ExampleComponent(); // Use base constructor: Component(ComponentType::Example)
	ExampleComponent(const Component& aComponent);
	// ExampleComponent(const Json::Value& aJson);
	~ExampleComponent() = default;

	// ExampleComponent& operator=(const ExampleComponent& aComponent);

	// void Init(const Json::Value& aJson) override;
	// void Init(GameObject* aParent) override;
	// void Update() override;

	// void SetActive(bool aIsActive) override;
	// void ToogleActive() override;

	// void ComponentPointersInvalidated() override;

	const ExampleComponent* GetTypePointer() const override; // return this;
	Json::Value ToJson() const override;

private:

};