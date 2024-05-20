#pragma once
#include "../Rendering/Buffers/ParticleBuffer.h"
#include "../Rendering/ParticleVertex.h"
#include "Math/Transform.h"
#include "EmitterData.h"

class Texture;
class Shader;

const float globalParticleGravity = 98.1f;

class ParticleEmitter
{
public:
	enum class EmitterType
	{
		Unknown,
		Burst,
		Stream
	};

	ParticleEmitter(EmitterType aType);
	ParticleEmitter(const Json::Value& aJson);
	ParticleEmitter(const ParticleEmitter& anEmitter);
	ParticleEmitter(ParticleEmitter&& anEmitter) noexcept;
	virtual ~ParticleEmitter();
	ParticleEmitter& operator=(const ParticleEmitter& anEmitter);
	ParticleEmitter& operator=(ParticleEmitter&& anEmitter) noexcept;

	void Init(const EmitterData& someData, Texture* aTexture, Shader* aVertexShader = nullptr, Shader* aGeometryShader = nullptr, Shader* aPixelShader = nullptr);
	void InitAfterJsonLoad();
	virtual void Update(float aDeltaTime) = 0;

	void SetAsResource() const;
	void Render() const;

	void SetParentTransform(Transform& aParentTransform);

	virtual void CreateImGuiElements();
	virtual Json::Value ToJson() const;

private:
	EmitterType myType;

	UINT myVertexCount;
	UINT myPrimitiveTopology;
	UINT myStride;
	ComPtr<ID3D11Buffer> myVertexBuffer;

	const Texture* myTexture;
	const Shader* myVertexShader;
	const Shader* myGeometryShader;
	const Shader* myPixelShader;

protected:
	ParticleBuffer myBuffer;
	Transform myTransform;
	EmitterData myData;
	std::vector<ParticleVertex> myParticles;
	std::vector<ParticleVertex*> myActiveParticles;
	std::vector<ParticleVertex*> myInactiveParticles;

	void Update();

	void CreateParticles(unsigned anAmount);
	virtual void CreateParticles();
	virtual void InitParticle(ParticleVertex& aParticle);
	virtual auto ActivateParticle(ParticleVertex* aParticle) -> decltype(myInactiveParticles.begin());
	virtual auto DeactivateParticle(ParticleVertex* aParticle) -> decltype(myActiveParticles.begin());

	virtual void UpdateBuffer();

	float GetGravity(float aDeltaTime);
};

