#pragma once
#include "../Component.h"
#include "ParticleEmitter.h"

class ParticleEmitterComponent : public Component
{
public:
	ParticleEmitterComponent();
	ParticleEmitterComponent(const ParticleEmitterComponent& aComponent) = default;
	ParticleEmitterComponent(ParticleEmitterComponent&& aComponent) noexcept = default;
	ParticleEmitterComponent(const Json::Value& aJson);
	~ParticleEmitterComponent() = default;

	ParticleEmitterComponent& operator=(const ParticleEmitterComponent& aComponent) = default;
	ParticleEmitterComponent& operator=(ParticleEmitterComponent&& aComponent) noexcept = default;

	void Init(GameObject* aParent) override;
	void Update() override;

	void TransformHasChanged() const override;

	void AddEmitter();

	void CreateImGuiComponents(const std::string& aWindowName) override;
	Json::Value ToJson() const override;
	inline std::string ToString() const override;
	const ParticleEmitterComponent* GetTypePointer() const override;

private:
	Transform myTransform;
	std::shared_ptr<ParticleEmitter> myEmitter;
};