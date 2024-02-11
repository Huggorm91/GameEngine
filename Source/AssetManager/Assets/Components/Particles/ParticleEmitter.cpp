#include "AssetManager.pch.h"
#include "ParticleEmitter.h"
#include "GraphicsEngine/GraphicsEngine.h"

ParticleEmitter::ParticleEmitter(EmitterType aType) : myID(GraphicsEngine::Get().GetParticleDrawer().GetNewEmitterID()), myType(aType), myData(), myVertexCount(0), myPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST), myStride(sizeof(ParticleVertex)),
myTexture(nullptr), myVertexShader(nullptr), myGeometryShader(nullptr), myPixelShader(nullptr)
{}

ParticleEmitter::ParticleEmitter(const Json::Value& aJson) : myID(aJson["ID"].asUInt()), myType(static_cast<EmitterType>(aJson["Type"].asInt())), myData(aJson["Data"]), myVertexCount(0), myPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST),
myStride(sizeof(ParticleVertex)), myTexture(nullptr), myVertexShader(nullptr), myGeometryShader(nullptr), myPixelShader(nullptr)
{}

ParticleEmitter::ParticleEmitter(const ParticleEmitter& anEmitter) : myID(GraphicsEngine::Get().GetParticleDrawer().GetNewEmitterID()), myType(anEmitter.myType), myData(anEmitter.myData), myVertexCount(anEmitter.myVertexCount), myPrimitiveTopology(anEmitter.myPrimitiveTopology),
myStride(sizeof(ParticleVertex)), myVertexBuffer(anEmitter.myVertexBuffer), myParticles(anEmitter.myParticles), myTexture(anEmitter.myTexture), myVertexShader(anEmitter.myVertexShader), myGeometryShader(anEmitter.myGeometryShader), myPixelShader(anEmitter.myPixelShader)
{}

ParticleEmitter::ParticleEmitter(ParticleEmitter&& anEmitter) noexcept : myID(anEmitter.myID), myType(anEmitter.myType), myData(anEmitter.myData), myVertexCount(anEmitter.myVertexCount), myPrimitiveTopology(anEmitter.myPrimitiveTopology),
myStride(sizeof(ParticleVertex)), myVertexBuffer(std::move(anEmitter.myVertexBuffer)), myParticles(std::move(anEmitter.myParticles)), myTexture(anEmitter.myTexture), myVertexShader(anEmitter.myVertexShader), myGeometryShader(anEmitter.myGeometryShader), myPixelShader(anEmitter.myPixelShader)
{}

ParticleEmitter& ParticleEmitter::operator=(const ParticleEmitter& anEmitter)
{
	const_cast<unsigned&>(myID) = GraphicsEngine::Get().GetParticleDrawer().GetNewEmitterID();
	myType = anEmitter.myType;
	myData = anEmitter.myData;
	myVertexCount = anEmitter.myVertexCount;
	myVertexBuffer = anEmitter.myVertexBuffer;
	myParticles = anEmitter.myParticles;

	myTexture = anEmitter.myTexture;
	myVertexShader = anEmitter.myVertexShader;
	myGeometryShader = anEmitter.myGeometryShader;
	myPixelShader = anEmitter.myPixelShader;
	return *this;
}

ParticleEmitter& ParticleEmitter::operator=(ParticleEmitter&& anEmitter) noexcept
{
	const_cast<unsigned&>(myID) = anEmitter.myID;
	myType = anEmitter.myType;
	myData = anEmitter.myData;
	myVertexCount = anEmitter.myVertexCount;
	myVertexBuffer = std::move(anEmitter.myVertexBuffer);
	myParticles = std::move(anEmitter.myParticles);

	myTexture = anEmitter.myTexture;
	myVertexShader = anEmitter.myVertexShader;
	myGeometryShader = anEmitter.myGeometryShader;
	myPixelShader = anEmitter.myPixelShader;
	return *this;
}

void ParticleEmitter::Init(const EmitterData& someData, Texture* aTexture, Shader* aVertexShader, Shader* aGeometryShader, Shader* aPixelShader)
{
	myData = someData;
	myTexture = aTexture;
	myVertexShader = aVertexShader;
	myGeometryShader = aGeometryShader;
	myPixelShader = aPixelShader;

	const size_t maxCount = static_cast<size_t>(std::ceil(myData.SpawnRate * myData.LifeTime));
	myParticles.resize(maxCount);

	myVertexCount = maxCount;
}

void ParticleEmitter::SetAsResource() const
{
	D3D11_MAPPED_SUBRESOURCE bufferData;
	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	RHI::Context->Map(myVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
	{
		const rsize_t size = sizeof(ParticleVertex) * myParticles.size();
		memcpy_s(bufferData.pData, size, myParticles.data(), size);
	}
	RHI::Context->Unmap(myVertexBuffer.Get(), 0);

	RHI::SetVertexShader(myVertexShader);
	RHI::SetGeometryShader(myGeometryShader);
	RHI::SetPixelShader(myPixelShader);

	RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, TextureSlot_Albedo, myTexture);
}

void ParticleEmitter::Render() const
{
	RHI::Draw(myVertexCount);
}

Json::Value ParticleEmitter::ToJson() const
{
	Json::Value result;
	result["ID"] = myID;
	result["Type"] = static_cast<int>(myType);
	result["Data"] = myData;
	return result;
}
