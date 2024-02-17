#pragma once
#include "../Component.h"
#include "GraphicsEngine/Drawer/ParticleEmitter.h"

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


	void Update() override;
	void Render() override;

	void SetEmitter(std::shared_ptr<ParticleEmitter> anEmitter);

	void CreateImGuiComponents(const std::string& aWindowName) override;
	Json::Value ToJson() const override;
	inline std::string ToString() const override;
	const ParticleEmitterComponent* GetTypePointer() const override;

private:
	std::shared_ptr<ParticleEmitter> myEmitter;
};