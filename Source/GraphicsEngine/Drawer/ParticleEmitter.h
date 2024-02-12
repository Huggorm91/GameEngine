#pragma once
#include "../Rendering/Buffers/ParticleBuffer.h"
#include "../Rendering/ParticleVertex.h"
#include "Math/Transform.h"
#include "EmitterData.h"

class Texture;
class Shader;

const float globalParticleGravity = 981.f;

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

	void Init(const EmitterData& someData, Texture* aTexture, Shader* aVertexShader, Shader* aGeometryShader, Shader* aPixelShader);
	virtual void Update(float aDeltaTime) = 0;

	void SetAsResource() const;
	void Render() const;

	void SetParentTransform(Transform& aParentTransform);

	virtual Json::Value ToJson() const;

private:
	EmitterType myType;

	UINT myVertexCount;
	UINT myPrimitiveTopology;
	UINT myStride;
	ComPtr<ID3D11Buffer> myVertexBuffer;

	Texture* myTexture;
	Shader* myVertexShader;
	Shader* myGeometryShader;
	Shader* myPixelShader;

	void CreateParticles();

protected:
	ParticleBuffer myBuffer;
	Transform myTransform;
	EmitterData myData;
	std::vector<ParticleVertex> myParticles;

	void Update();

	virtual void InitParticle(ParticleVertex& aParticle);

	virtual void InitBuffer();
	virtual void UpdateBuffer();
};

