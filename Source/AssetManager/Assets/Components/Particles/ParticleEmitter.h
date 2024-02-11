#pragma once
#include "../Transform.h"
#include "GraphicsEngine/Drawer/EmitterData.h"
#include "GraphicsEngine/Rendering/ParticleVertex.h"

class Texture;
class Shader;

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
	virtual ~ParticleEmitter() = default;
	ParticleEmitter& operator=(const ParticleEmitter& anEmitter);
	ParticleEmitter& operator=(ParticleEmitter&& anEmitter) noexcept;

	void Init(const EmitterData& someData, Texture* aTexture, Shader* aVertexShader, Shader* aGeometryShader, Shader* aPixelShader);
	virtual void Update(float aDeltaTime) = 0;

	void SetAsResource() const;
	void Render() const;

	virtual Json::Value ToJson() const;

private:
	const unsigned myID;
	EmitterType myType;

	UINT myVertexCount;
	UINT myPrimitiveTopology;
	UINT myStride;
	ComPtr<ID3D11Buffer> myVertexBuffer;

	Texture* myTexture;
	Shader* myVertexShader;
	Shader* myGeometryShader;
	Shader* myPixelShader;

protected:
	EmitterData myData;
	std::vector<ParticleVertex> myParticles;

	virtual void InitParticle(ParticleVertex& aParticle);
};