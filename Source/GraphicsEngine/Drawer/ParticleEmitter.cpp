#include "GraphicsEngine.pch.h"
#include "ParticleEmitter.h"
#include "GraphicsEngine/GraphicsEngine.h"
#include "AssetManager/AssetManager.h"
#include "ParticleDrawer.h"
#include "ThirdParty/ImGui/imgui.h"
#include "ThirdParty/ImGui/misc/cpp/imgui_stdlib.h"
#include "AssetManager/Assets/ImguiTransform.h"

ParticleEmitter::ParticleEmitter(EmitterType aType) :
	myType(aType),
	myVertexCount(0),
	myPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST),
	myStride(sizeof(ParticleVertex)),
	myTexture(nullptr),
	myVertexShader(nullptr),
	myGeometryShader(nullptr),
	myPixelShader(nullptr)
{
	myBuffer.Initialize(L"ParticleBuffer");
}

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
	myBuffer.Initialize(L"ParticleBuffer");
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
{
	myBuffer.Initialize(L"ParticleBuffer");
	RHI::UpdateConstantBufferData(myBuffer);
}

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
{
	myBuffer.Initialize(L"ParticleBuffer");
	RHI::UpdateConstantBufferData(myBuffer);
}

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

	RHI::UpdateConstantBufferData(myBuffer);
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

	RHI::UpdateConstantBufferData(myBuffer);
	return *this;
}

void ParticleEmitter::Init(const EmitterData& someData, Texture* aTexture, Shader* aVertexShader, Shader* aGeometryShader, Shader* aPixelShader)
{
	myData = someData;
	myTexture = aTexture;

	// VertexShader
	if (aVertexShader)
	{
		myVertexShader = aVertexShader;
	}
	else
	{
		myVertexShader = GraphicsEngine::Get().GetDefaultParticleVS();
	}

	// GeometryShader
	if (aGeometryShader)
	{
		myGeometryShader = aGeometryShader;
	}
	else
	{
		myGeometryShader = GraphicsEngine::Get().GetDefaultParticleGS();
	}

	// PixelShader
	if (aPixelShader)
	{
		myPixelShader = aPixelShader;
	}
	else
	{
		myPixelShader = GraphicsEngine::Get().GetDefaultParticlePS();
	}

	CreateParticles();
	UpdateBuffer();
}

void ParticleEmitter::InitAfterJsonLoad()
{
	CreateParticles();
	UpdateBuffer();
}

void ParticleEmitter::Update()
{
	if (myTexture)
	{
		if (myTransform.HasChanged())
		{
			myBuffer.Data.Transform = myTransform.GetTransformMatrix();
			RHI::UpdateConstantBufferData(myBuffer);
		}
		GraphicsEngine::Get().GetParticleDrawer().AddEmitter(this);
	}
}

void ParticleEmitter::SetAsResource() const
{
	D3D11_MAPPED_SUBRESOURCE bufferData;
	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	auto result = RHI::Context->Map(myVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
	if (result == S_OK)
	{
		const rsize_t size = sizeof(ParticleVertex) * myParticles.size();
		memcpy_s(bufferData.pData, size, &myParticles[0], size);
	}
	RHI::Context->Unmap(myVertexBuffer.Get(), 0);

	RHI::SetVertexShader(myVertexShader);
	RHI::SetGeometryShader(myGeometryShader);
	RHI::SetPixelShader(myPixelShader);

	RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, TextureSlot_Albedo, myTexture);

	RHI::SetConstantBuffer(PIPELINE_STAGE_VERTEX_SHADER | PIPELINE_STAGE_GEOMETERY_SHADER | PIPELINE_STAGE_PIXEL_SHADER, GraphicsEngine::Get().myParticleBufferSlot , myBuffer);
}

void ParticleEmitter::Render() const
{
	RHI::ConfigureInputAssembler(myPrimitiveTopology, myVertexBuffer, nullptr, myStride, ParticleVertex::InputLayout);
	RHI::Draw(myVertexCount);
}

void ParticleEmitter::SetParentTransform(Transform& aParentTransform)
{
	myTransform.SetParent(&aParentTransform);
	UpdateBuffer();
}

void ParticleEmitter::CreateImGuiElements()
{
	constexpr float imageSize = 75.f;

	::CreateImGuiComponents(myTransform);

	ImGui::Image(myTexture->GetSRV().Get(), ImVec2(imageSize, imageSize));
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Dragged_Texture"))
		{
			IM_ASSERT(payload->DataSize == sizeof(Texture*));
			myTexture = *static_cast<Texture**>(payload->Data);
		}
		ImGui::EndDragDropTarget();
	}
	bool hasChanged = false;
	if (ImGui::DragFloat("Spawn Rate", &myData.SpawnRate))
	{
		hasChanged = true;
	}
	
	if (ImGui::DragFloat("Life Time", &myData.LifeTime))
	{
		hasChanged = true;
	}
	
	if (ImGui::DragFloat("Gravity Scale", &myData.GravityScale))
	{
		hasChanged = true;
	}

	if (ImGui::DragFloat("Start Speed", &myData.StartSpeed))
	{
		hasChanged = true;
	}
	
	if (ImGui::DragFloat("End Speed", &myData.EndSpeed))
	{
		hasChanged = true;
	}

	if (ImGui::DragFloat3("Start Scale", &myData.StartSize.x))
	{
		hasChanged = true;
	}
	
	if (ImGui::DragFloat3("End Scale", &myData.EndSize.x))
	{
		hasChanged = true;
	}

	if (ImGui::ColorEdit4("Start Color", &myData.StartColor.x))
	{
		hasChanged = true;
	}
	
	if (ImGui::ColorEdit4("End Color", &myData.EndColor.x))
	{
		hasChanged = true;
	}

	if (hasChanged)
	{
		UpdateBuffer();
	}
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

void ParticleEmitter::CreateParticles(unsigned anAmount)
{
	myVertexCount = anAmount;
	myParticles.resize(myVertexCount);

	for (auto& particle : myParticles)
	{
		InitParticle(particle);
		particle.myScale = Crimson::Vector3f::Null;
		myInactiveParticles.emplace_back(&particle);
	}

	RHI::CreateDynamicVertexBuffer(myVertexBuffer, myParticles.size(), sizeof(ParticleVertex));
}

void ParticleEmitter::CreateParticles()
{
	CreateParticles(static_cast<unsigned>(std::ceil(myData.SpawnRate * myData.LifeTime)));
}

void ParticleEmitter::InitParticle(ParticleVertex& aParticle)
{
	aParticle.myPosition = Crimson::Vector4f::NullPosition;
	aParticle.myColor = myData.StartColor;
	aParticle.mySpeed = myData.StartSpeed;
	aParticle.myScale = myData.StartSize;
	aParticle.myLifeTime = 0.f;
	aParticle.myDirection = Crimson::Vector3f::Forward;
}

auto ParticleEmitter::ActivateParticle(ParticleVertex* aParticle) -> decltype(myInactiveParticles.begin())
{
	InitParticle(*aParticle);
	myActiveParticles.emplace_back(aParticle);
	auto iter = myInactiveParticles.begin();
	while (iter != myInactiveParticles.end())
	{
		if (*iter == aParticle)
		{
			return myInactiveParticles.erase(iter);
		}
		iter++;
	}

	return myInactiveParticles.end();
}

auto ParticleEmitter::DeactivateParticle(ParticleVertex* aParticle) -> decltype(myActiveParticles.begin())
{
	aParticle->myScale = Crimson::Vector3f::Null;
	myInactiveParticles.emplace_back(aParticle);
	auto iter = myActiveParticles.begin();
	while (iter != myActiveParticles.end())
	{
		if (*iter == aParticle)
		{
			return myActiveParticles.erase(iter);
		}
		iter++;
	}

	return myActiveParticles.end();
}

void ParticleEmitter::UpdateBuffer()
{
	myBuffer.Data.MaxLifeTime = myData.LifeTime;
	myBuffer.Data.StartSize = myData.StartSize;
	myBuffer.Data.EndSize = myData.EndSize;
	myBuffer.Data.StartColor = myData.StartColor;
	myBuffer.Data.EndColor = myData.EndColor;
	myBuffer.Data.Transform = myTransform.GetTransformMatrix();

	RHI::UpdateConstantBufferData(myBuffer);
}

float ParticleEmitter::GetGravity(float aDeltaTime)
{
	return globalParticleGravity * aDeltaTime * myData.GravityScale;
}
