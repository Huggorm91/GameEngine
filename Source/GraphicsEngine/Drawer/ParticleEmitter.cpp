#include "GraphicsEngine.pch.h"
#include "ParticleEmitter.h"
#include "GraphicsEngine/GraphicsEngine.h"
#include "AssetManager/AssetManager.h"
#include "ParticleDrawer.h"

ParticleEmitter::ParticleEmitter(EmitterType aType) :
	myType(aType),
	myVertexCount(0),
	myPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST),
	myStride(sizeof(ParticleVertex)),
	myTexture(nullptr),
	myVertexShader(nullptr),
	myGeometryShader(nullptr),
	myPixelShader(nullptr)
{}

ParticleEmitter::ParticleEmitter(const Json::Value& aJson) :
	myType(static_cast<EmitterType>(aJson["Type"].asInt())),
	myVertexCount(0),
	myPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST),
	myStride(sizeof(ParticleVertex)),
	myTexture(aJson["Texture"].isNull() ? nullptr : AssetManager::GetAsset<Texture*>(aJson["Texture"].asString())),
	myVertexShader(aJson["VertexShader"].isNull() ? nullptr : AssetManager::GetAsset<Shader*>(aJson["VertexShader"].asString())),
	myGeometryShader(aJson["GeometryShader"].isNull() ? nullptr : AssetManager::GetAsset<Shader*>(aJson["GeometryShader"].asString())),
	myPixelShader(aJson["PixelShader"].isNull() ? nullptr : AssetManager::GetAsset<Shader*>(aJson["PixelShader"].asString())),
	myTransform(aJson["Transform"]),
	myData(aJson["Data"])
{
	CreateParticles();
	InitBuffer();
}

ParticleEmitter::ParticleEmitter(const ParticleEmitter& anEmitter) :
	myType(anEmitter.myType),
	myData(anEmitter.myData),
	myVertexCount(anEmitter.myVertexCount),
	myPrimitiveTopology(anEmitter.myPrimitiveTopology),
	myStride(sizeof(ParticleVertex)),
	myVertexBuffer(anEmitter.myVertexBuffer),
	myParticles(anEmitter.myParticles),
	myTexture(anEmitter.myTexture),
	myVertexShader(anEmitter.myVertexShader),
	myGeometryShader(anEmitter.myGeometryShader),
	myPixelShader(anEmitter.myPixelShader),
	myBuffer(anEmitter.myBuffer),
	myTransform(anEmitter.myTransform)
{}

ParticleEmitter::ParticleEmitter(ParticleEmitter&& anEmitter) noexcept :
	myType(anEmitter.myType),
	myData(anEmitter.myData),
	myVertexCount(anEmitter.myVertexCount),
	myPrimitiveTopology(anEmitter.myPrimitiveTopology),
	myStride(sizeof(ParticleVertex)),
	myVertexBuffer(std::move(anEmitter.myVertexBuffer)),
	myParticles(std::move(anEmitter.myParticles)),
	myTexture(anEmitter.myTexture),
	myVertexShader(anEmitter.myVertexShader),
	myGeometryShader(anEmitter.myGeometryShader),
	myPixelShader(anEmitter.myPixelShader),
	myBuffer(std::move(anEmitter.myBuffer)),
	myTransform(anEmitter.myTransform)
{}

ParticleEmitter::~ParticleEmitter()
{
	GraphicsEngine::Get().GetParticleDrawer().RemoveEmitter(this);
}

ParticleEmitter& ParticleEmitter::operator=(const ParticleEmitter& anEmitter)
{
	myType = anEmitter.myType;
	myData = anEmitter.myData;
	myVertexCount = anEmitter.myVertexCount;
	myVertexBuffer = anEmitter.myVertexBuffer;
	myParticles = anEmitter.myParticles;

	myTexture = anEmitter.myTexture;
	myVertexShader = anEmitter.myVertexShader;
	myGeometryShader = anEmitter.myGeometryShader;
	myPixelShader = anEmitter.myPixelShader;

	myBuffer = anEmitter.myBuffer;
	myTransform = anEmitter.myTransform;
	return *this;
}

ParticleEmitter& ParticleEmitter::operator=(ParticleEmitter&& anEmitter) noexcept
{
	myType = anEmitter.myType;
	myData = anEmitter.myData;
	myVertexCount = anEmitter.myVertexCount;
	myVertexBuffer = std::move(anEmitter.myVertexBuffer);
	myParticles = std::move(anEmitter.myParticles);

	myTexture = anEmitter.myTexture;
	myVertexShader = anEmitter.myVertexShader;
	myGeometryShader = anEmitter.myGeometryShader;
	myPixelShader = anEmitter.myPixelShader;

	myBuffer = std::move(anEmitter.myBuffer);
	myTransform = anEmitter.myTransform;
	return *this;
}

void ParticleEmitter::Init(const EmitterData& someData, Texture* aTexture, Shader* aVertexShader, Shader* aGeometryShader, Shader* aPixelShader)
{
	myData = someData;
	myTexture = aTexture;
	myVertexShader = aVertexShader;
	myGeometryShader = aGeometryShader;
	myPixelShader = aPixelShader;

	CreateParticles();
	InitBuffer();
}

void ParticleEmitter::Update()
{
	GraphicsEngine::Get().GetParticleDrawer().AddEmitter(this);
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

void ParticleEmitter::SetParentTransform(Transform& aParentTransform)
{
	myTransform.SetParent(&aParentTransform);
}

Json::Value ParticleEmitter::ToJson() const
{
	Json::Value result;
	result["Type"] = static_cast<int>(myType);
	result["Data"] = myData;
	result["Transform"] = myTransform.ToJson();
	if (myTexture)
	{
		result["Texture"] = Crimson::ToString(myTexture->GetName());
	}
	else
	{
		result["Texture"] = Json::nullValue;
	}

	if (myVertexShader)
	{
		result["VertexShader"] = Crimson::ToString(myVertexShader->GetName());
	}
	else
	{
		result["VertexShader"] = Json::nullValue;
	}
	if (myGeometryShader)
	{
		result["GeometryShader"] = Crimson::ToString(myGeometryShader->GetName());
	}
	else
	{
		result["GeometryShader"] = Json::nullValue;
	}
	if (myPixelShader)
	{
		result["PixelShader"] = Crimson::ToString(myPixelShader->GetName());
	}
	else
	{
		result["PixelShader"] = Json::nullValue;
	}
	return result;
}

void ParticleEmitter::CreateParticles()
{
	myVertexCount = static_cast<UINT>(std::ceil(myData.SpawnRate * myData.LifeTime));
	myParticles.resize(myVertexCount);

	for (auto& particle : myParticles)
	{
		InitParticle(particle);
	}
}

void ParticleEmitter::InitParticle(ParticleVertex& aParticle)
{
	aParticle.myPosition = Crimson::Vector3f::Null;
	aParticle.myColor = myData.StartColor;
	aParticle.myVelocity = myData.StartVelocity;
	aParticle.myScale = myData.StartSize;
	aParticle.myLifeTime = 0.f;
}

void ParticleEmitter::InitBuffer()
{
	myBuffer.Initialize(L"ParticleBuffer");
	myBuffer.Data.MaxLifeTime = myData.LifeTime;
	myBuffer.Data.StartSize = myData.StartSize;
	myBuffer.Data.EndSize = myData.EndSize;
	myBuffer.Data.StartColor = myData.StartColor;
	myBuffer.Data.EndColor = myData.EndColor;
	myBuffer.Data.Transform = myTransform.GetTransformMatrix();
}

void ParticleEmitter::UpdateBuffer()
{
	myBuffer.Data.Transform = myTransform.GetTransformMatrix();
}
