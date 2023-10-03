#pragma once
#include "../Component.h"
#include "../Transform.h"

class ParticleEmitterComponent : public Component
{
	// TODO:
	// 1. Add #include "Components/ExampleComponent.h" to ComponentInclude.h
	// 2. Add enum for new ComponentType in ComponentType.h
	// 3. Add 'case ComponentType::Example: { const ExampleComponent& component = *dynamic_cast<const ExampleComponent*>(aComponent); aParent.AddComponent(component); break; }' to switch in AddComponent() in ComponentType.cpp
	// 4. Add 'case ComponentType::Example: { break; }' to switch in LoadComponent() in ComponentType.cpp, and implement Constructor(Json::Value) and/or Init(Json::Value)
	// 5. Add 'case ComponentType::Example: return "Example";' to switch in ComponentTypeToString() in ComponentType.cpp
public:
	ParticleEmitterComponent(); // Use base constructor: Component(ComponentType::Example)
	ParticleEmitterComponent(const ParticleEmitterComponent& aComponent) = default;
	ParticleEmitterComponent(ParticleEmitterComponent&& aComponent) noexcept = default;
	ParticleEmitterComponent(const Json::Value& aJson); // Init(const Json::Value& aJson) can be used as an alternative
	~ParticleEmitterComponent() = default;

	ParticleEmitterComponent& operator=(const ParticleEmitterComponent& aComponent) = default;
	ParticleEmitterComponent& operator=(ParticleEmitterComponent&& aComponent) noexcept = default;

	// void Init(const Json::Value& aJson) override;
	//void Init(GameObject* aParent) override;
	//void Update() override;

	// void SetActive(bool aIsActive) override;
	// void ToogleActive() override;

	//void TransformHasChanged() const override;

	//void CreateImGuiComponents(const std::string& aWindowName) override;
	//Json::Value ToJson() const override;
	inline std::string ToString() const override;
	const ParticleEmitterComponent* GetTypePointer() const override; // return this;

private:
	Transform myTransform;
};