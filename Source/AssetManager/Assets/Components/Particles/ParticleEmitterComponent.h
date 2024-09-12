#pragma once
#include "../Component.h"
#include "GraphicsEngine/Drawer/ParticleEmitter.h"

BEGIN_COMPONENT(ParticleEmitterComponent)
public:
	ParticleEmitterComponent();
	ParticleEmitterComponent(const ParticleEmitterComponent& aComponent) = default;
	ParticleEmitterComponent(ParticleEmitterComponent&& aComponent) noexcept = default;
	ParticleEmitterComponent(const Json::Value& aJson);
	~ParticleEmitterComponent() = default;

	ParticleEmitterComponent& operator=(const ParticleEmitterComponent& aComponent) = default;
	ParticleEmitterComponent& operator=(ParticleEmitterComponent&& aComponent) noexcept = default;

	void Init(GameObject* aParent) override;

	void Render() override;

	void SetEmitter(std::shared_ptr<ParticleEmitter> anEmitter);

	void TransformHasChanged() const override;

#ifdef EDITOR
	void CreateImGuiComponents(const std::string& aWindowName) override;
#endif // EDITOR
	Json::Value ToJson() const override;

private:
	std::shared_ptr<ParticleEmitter> myEmitter;
};