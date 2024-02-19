#pragma once
#include "GraphicsEngine/Drawer/ParticleEmitter.h"

class BurstEmitter : public ParticleEmitter
{
public:
	BurstEmitter();
	BurstEmitter(const Json::Value& aJson);
	BurstEmitter(const BurstEmitter& anEmitter) = default;
	BurstEmitter(BurstEmitter&& anEmitter) noexcept = default;
	virtual ~BurstEmitter() = default;
	BurstEmitter& operator=(const BurstEmitter& anEmitter) = default;
	BurstEmitter& operator=(BurstEmitter&& anEmitter) noexcept = default;

	void Init(float aBurstInterval, const EmitterData& someData, Texture* aTexture, Shader* aVertexShader = nullptr, Shader* aGeometryShader = nullptr, Shader* aPixelShader = nullptr);
	void Update(float aDeltaTime) override;

	void SetLooping(bool aState);

	void TriggerBurst();

	void CreateImGuiElements() override;
	Json::Value ToJson() const override;

private:
	float myBurstInterval;
	float myTimer;
	bool myIsLooping;
	bool myIsActive;

	void CreateParticles() override;
	void UpdateParticles(float aDeltaTime);
	void CreateBurst();
};