#include "GraphicsEngine.pch.h"
#include "GraphicsEngine.h"
#include <fstream>
#include "Rendering/Vertex.h"
#include "Commands/Light/LitCmd_ResetLightBuffer.h"
#include "Json/jsonCpp/json.h"
#include "Math/Sort.hpp"
#include "File/DirectoryFunctions.h"

GraphicsEngine::GraphicsEngine() :myWindowHandle(), myDefaultSampler(), myShadowSampler(), myLUTSampler(), myWorldRadius(1.f), myWindowSize{ 0,0 }, myWorldMax(), myWorldMin(), myWorldCenter(), myBackgroundColor(),
myRenderCommands(&myFirstCommandlist), myUpdateCommands(&mySecondCommandlist), myDirectionalShadowMap(nullptr), myPointShadowMap{ nullptr }, mySpotShadowMap{ nullptr }, myDefaultMaterial(), myFrameBuffer(), 
myObjectBuffer(), myLightBuffer(), myMaterialBuffer(), myLineDrawer(), myFirstCommandlist(), mySecondCommandlist(), myTextures(), myShaders(), myIsUsingBloom(true),
myAssetPath("Settings\\EngineAssets\\"), mySettingsPath("Settings\\ge_settings.json")
#ifndef _RETAIL
, myDebugMode(DebugMode::Default), myLightMode(LightMode::Default), myRenderMode(RenderMode::Mesh), myGrid()
#endif // !_RETAIL	
{
}

bool GraphicsEngine::Initialize(HWND windowHandle, bool enableDeviceDebug)
{
	GELogger = Logger::Create("GraphicsEngine");

#ifndef _RETAIL
	try
	{
#endif
		myWindowHandle = windowHandle;

		if (!RHI::Initialize(myWindowHandle,
							 enableDeviceDebug,
							 &myTextures.BackBuffer,
							 &myTextures.DepthBuffer))
		{
			GELogger.Err("Failed to initialize the RHI!");
			return false;
		}
		myWindowSize.cx = RHI::GetDeviceSize().Width;
		myWindowSize.cy = RHI::GetDeviceSize().Height;

		Settings settings = LoadSettings();
		myBackgroundColor = settings.BackgroundColor;
		myToneMap = static_cast<ToneMap>(settings.ToneMap);

		// Textures
		{
			myTextureSlots.MissingTextureSlot = 99u;
			myTextureSlots.DefaultAlbedoTextureSlot = 98u;
			myTextureSlots.DefaultNormalTextureSlot = 97u;
			myTextureSlots.DefaultMaterialTextureSlot = 96u;
			myTextureSlots.DefaultFXTextureSlot = 95u;
			myTextureSlots.BrdfLUTTextureSlot = 94u;
			myTextureSlots.DefaultCubeMapSlot = 100u;

			myTextureSlots.GBufferSlot = 10u;

			myTextureSlots.IntermediateASlot = 20u;
			myTextureSlots.IntermediateBSlot = 21u;

			myTextureSlots.DirectionalShadowMapSlot = 93u;
			myTextureSlots.PointShadowMapSlot = 120u;
			myTextureSlots.SpotShadowMapSlot = 85u;

			if (!CreateLUTTexture())
			{
				GELogger.Err("Failed to create LUT texture!");
				return false;
			}
			RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, myTextureSlots.BrdfLUTTextureSlot, &myTextures.BrdfLUTTexture);

			if (!myGBuffer.Init(10))
			{
				GELogger.Err("Failed to initialize GBuffer!");
				return false;
			}
			myTextureSlots.GBufferCount = myGBuffer.Count();

			if (!CreatePostProcessingTextures())
			{
				GELogger.Err("Failed to create Post-Processing textures!");
				return false;
			}

			LoadDefaultTextures(settings);
		}

		// Samplers
		{
			if (!CreateDefaultSampler())
			{
				GELogger.Err("Failed to create default sampler!");
				return false;
			}
			RHI::SetSamplerState(myDefaultSampler, 0);

			if (!CreateBlurSampler())
			{
				GELogger.Err("Failed to create Blur sampler!");
				return false;
			}
			RHI::SetSamplerState(myBlurSampler, 13);

			if (!CreateShadowSampler())
			{
				GELogger.Err("Failed to create shadow sampler!");
				return false;
			}
			RHI::SetSamplerState(myShadowSampler, 14);

			if (!CreateLUTSampler())
			{
				GELogger.Err("Failed to create LUT sampler!");
				return false;
			}
			RHI::SetSamplerState(myLUTSampler, 15);
		}

		// Blends
		{
			if (!CreateAlphaBlend())
			{
				GELogger.Err("Failed to create Alpha Blend");
				return false;
			}

			if (!CreateAdditiveBlend())
			{
				GELogger.Err("Failed to create Additive Blend");
				return false;
			}
		}

		// Shaders
		{
			if (!LoadShaders(settings))
			{
				return false;
			}
		}

		// Material
		{
			LoadMaterial(settings);
			if (!RHI::CreateInputLayout(Vertex::InputLayout, Vertex::InputLayoutDefinition, myDefaultMaterial.GetVertexShader()->GetBlob(), myDefaultMaterial.GetVertexShader()->GetBlobSize()))
			{
				GELogger.Err("Failed to create InputLayout!");
				return false;
			}
		}

		// Buffers
		{
			myFrameBufferSlot = 0;
			myObjectBufferSlot = 1;
			myLightBufferSlot = 2;
			myMaterialBufferSlot = 3;

			myFrameBuffer.Initialize(L"FrameBuffer");
			myObjectBuffer.Initialize(L"ObjectBuffer");
			myLightBuffer.Initialize(L"LightBuffer");
			myMaterialBuffer.Initialize(L"MaterialBuffer");

			myFrameBuffer.Data.ScreenSize = { static_cast<float>(RHI::GetDeviceSize().Width), static_cast<float>(RHI::GetDeviceSize().Height) };

			SetToneMap(myToneMap);
		}

		if (!myLineDrawer.Init())
		{
			GELogger.Err("Failed to initialize LineDrawer!");
			return false;
		}

#ifndef _RETAIL
		myGrid = myLineDrawer.AddAxisLines(Crimson::Vector3f::Null, 10000000.f, true);
	}
	catch (const std::exception& e)
	{
		GELogger.LogException(e);
		exit(-1);
	}
#endif	

	return true;
}

void GraphicsEngine::SaveSettings() const
{
	Settings settings;

	auto& material = settings.DefaultMaterial;	
	material.Shininess = myDefaultMaterial.GetShininess();
	material.Metalness = myDefaultMaterial.GetMetalness();
	material.NormalStrength = myDefaultMaterial.GetNormalStrength();
	material.UVTiling = myDefaultMaterial.GetUVTiling();
	material.AlbedoColor = myDefaultMaterial.GetAlbedoColor();
	material.EmissionColor = myDefaultMaterial.GetEmissionColor();
	material.EmissionIntensity = myDefaultMaterial.GetEmissionIntensity();

	settings.DefaultMissingTexture = Crimson::ToString(myTextures.MissingTexture.GetName());
	settings.DefaultAlbedoTexture = Crimson::ToString(myTextures.DefaultAlbedoTexture.GetName());
	settings.DefaultNormalTexture = Crimson::ToString(myTextures.DefaultNormalTexture.GetName());
	settings.DefaultMaterialTexture = Crimson::ToString(myTextures.DefaultMaterialTexture.GetName());
	settings.DefaultFXTexture = Crimson::ToString(myTextures.DefaultFXTexture.GetName());
	settings.DefaultCubeMap = Crimson::ToString(myTextures.DefaultCubeMap.GetName());

	settings.DefaultVSShader = Crimson::ToString(myShaders.DefaultVS.GetName());
	settings.DefaultPSShader = Crimson::ToString(myShaders.DefaultPS.GetName());

	settings.LuminancePS = Crimson::ToString(myShaders.LuminancePS.GetName());
	settings.BlurPS = Crimson::ToString(myShaders.BlurPS.GetName());
	settings.BloomPS = Crimson::ToString(myShaders.BloomPS.GetName());
	settings.GammaPS = Crimson::ToString(myShaders.GammaPS.GetName());
	settings.CopyPS = Crimson::ToString(myShaders.CopyPS.GetName());

	settings.GBufferPS = Crimson::ToString(myShaders.GBufferPS.GetName());
	settings.EnvironmentPS = Crimson::ToString(myShaders.EnvironmentPS.GetName());
	settings.PointlightPS = Crimson::ToString(myShaders.PointlightPS.GetName());
	settings.SpotlightPS = Crimson::ToString(myShaders.SpotlightPS.GetName());

	settings.BackgroundColor = myBackgroundColor;
	settings.ToneMap = static_cast<int>(myToneMap);

	SaveSettings(settings);
}

void GraphicsEngine::SetLoggingWindow(HANDLE aHandle)
{
	GELogger.SetConsoleHandle(aHandle);
}

void GraphicsEngine::SetBackGroundColor(const Crimson::Vector3f& aColor)
{
	myBackgroundColor = aColor;
}

void GraphicsEngine::Swap()
{
	assert(false && "Not Implemented!");
	//Crimson::Swap(myRenderCommands, myUpdateCommands);
	// Not threadsafe until LineDrawer also swaps buffers!
}

GraphicsEngine::ToneMap GraphicsEngine::SetToneMap(ToneMap aMode)
{
	myToneMap = aMode;

	switch (aMode)
	{
	case GraphicsEngine::ToneMap::Count:
	{
		myToneMap = ToneMap::None;
		[[fallthrough]];
	}
	case GraphicsEngine::ToneMap::None:
	{
		GELogger.Log("ToneMap: None");
		break;
	}
	case GraphicsEngine::ToneMap::Reinhard:
	{
		GELogger.Log("ToneMap: Reinhard");
		break;
	}
	case GraphicsEngine::ToneMap::UnrealEngine:
	{
		GELogger.Log("ToneMap: UnrealEngine");
		break;
	}
	case GraphicsEngine::ToneMap::ACES:
	{
		GELogger.Log("ToneMap: ACES");
		break;
	}
	case GraphicsEngine::ToneMap::Lottes:
	{
		GELogger.Log("ToneMap: Lottes");
		break;
	}
	default:
		GELogger.Err("ToneMap: Invalid option");
		break;
	}

	myLightBuffer.Data.LB_ToneMapMode = static_cast<int>(myToneMap);
	RHI::UpdateConstantBufferData(myLightBuffer);
	RHI::SetConstantBuffer(PIPELINE_STAGE_PIXEL_SHADER, myLightBufferSlot, myLightBuffer);

	return myToneMap;
}

GraphicsEngine::ToneMap GraphicsEngine::NextToneMap()
{
	return SetToneMap(static_cast<ToneMap>(static_cast<int>(myToneMap) + 1));
}

#ifndef _RETAIL
GraphicsEngine::DebugMode GraphicsEngine::SetDebugMode(DebugMode aMode)
{
	myDebugMode = aMode;

	switch (myDebugMode)
	{
	case GraphicsEngine::DebugMode::Count:
	{
		myDebugMode = DebugMode::Default;
		[[fallthrough]];
	}
	case GraphicsEngine::DebugMode::Default:
	{
		GELogger.Log("DebugMode: Default");
		break;
	}
	case GraphicsEngine::DebugMode::UV:
	{
		GELogger.Log("DebugMode: UV");
		break;
	}
	case GraphicsEngine::DebugMode::PixelNormal:
	{
		GELogger.Log("DebugMode: PixelNormal");
		break;
	}
	case GraphicsEngine::DebugMode::Normal:
	{
		GELogger.Log("DebugMode: Normal");
		break;
	}
	case GraphicsEngine::DebugMode::Tangent:
	{
		GELogger.Log("DebugMode: Tangent");
		break;
	}
	case GraphicsEngine::DebugMode::Binormal:
	{
		GELogger.Log("DebugMode: Binormal");
		break;
	}
	case GraphicsEngine::DebugMode::AmbientOcclusion:
	{
		GELogger.Log("DebugMode: AmbientOcclusion");
		break;
	}
	case GraphicsEngine::DebugMode::Roughness:
	{
		GELogger.Log("DebugMode: Roughness");
		break;
	}
	case GraphicsEngine::DebugMode::Metalness:
	{
		GELogger.Log("DebugMode: Metalness");
		break;
	}
	case GraphicsEngine::DebugMode::Emission:
	{
		GELogger.Log("DebugMode: Emission");
		break;
	}
	case GraphicsEngine::DebugMode::VertexColors:
	{
		GELogger.Log("DebugMode: VertexColors");
		break;
	}
	case GraphicsEngine::DebugMode::AlbedoMap:
	{
		GELogger.Log("DebugMode: AlbedoMap");
		break;
	}
	case GraphicsEngine::DebugMode::NormalMap:
	{
		GELogger.Log("DebugMode: NormalMap");
		break;
	}
	case GraphicsEngine::DebugMode::DirectionallightUV:
	{
		GELogger.Log("DebugMode: Directionallight UV");
		break;
	}
	default:
		GELogger.Err("DebugMode: Invalid option");
		return myDebugMode;
	}

	myFrameBuffer.Data.DebugMode = static_cast<int>(myDebugMode);
	RHI::UpdateConstantBufferData(myFrameBuffer);
	RHI::SetConstantBuffer(PIPELINE_STAGE_VERTEX_SHADER | PIPELINE_STAGE_PIXEL_SHADER, myFrameBufferSlot, myFrameBuffer);

	return myDebugMode;
}

GraphicsEngine::DebugMode GraphicsEngine::NextDebugMode()
{
	return SetDebugMode(static_cast<DebugMode>(static_cast<int>(myDebugMode) + 1));
}

GraphicsEngine::LightMode GraphicsEngine::SetLightMode(LightMode aMode)
{
	myLightMode = aMode;

	switch (myLightMode)
	{
	case GraphicsEngine::LightMode::Count:
	{
		myLightMode = LightMode::Default;
		[[fallthrough]];
	}
	case GraphicsEngine::LightMode::Default:
	{
		GELogger.Log("LightMode: Default");
		break;
	}
	case GraphicsEngine::LightMode::AmbientLight:
	{
		GELogger.Log("LightMode: AmbientLight");
		break;
	}
	case GraphicsEngine::LightMode::DirectLight:
	{
		GELogger.Log("LightMode: DirectLight");
		break;
	}
	case GraphicsEngine::LightMode::PointLight:
	{
		GELogger.Log("LightMode: PointLight");
		break;
	}
	case GraphicsEngine::LightMode::SpotLight:
	{
		GELogger.Log("LightMode: SpotLight");
		break;
	}
	case GraphicsEngine::LightMode::IgnoreLight:
	{
		GELogger.Log("LightMode: IgnoreLight");
		break;
	}
	default:
		GELogger.Err("LightMode: Invalid option");
		return myLightMode;
	}

	myFrameBuffer.Data.LightMode = static_cast<int>(myLightMode);
	RHI::UpdateConstantBufferData(myLightBuffer);
	RHI::SetConstantBuffer(PIPELINE_STAGE_VERTEX_SHADER | PIPELINE_STAGE_PIXEL_SHADER, myLightBufferSlot, myLightBuffer);

	return myLightMode;
}

GraphicsEngine::LightMode GraphicsEngine::NextLightMode()
{
	return SetLightMode(static_cast<LightMode>(static_cast<int>(myLightMode) + 1));
}

GraphicsEngine::RenderMode GraphicsEngine::SetRenderMode(RenderMode aMode)
{
	myRenderMode = aMode;

	switch (myRenderMode)
	{
	case GraphicsEngine::RenderMode::Count:
	{
		myRenderMode = RenderMode::Mesh;
		[[fallthrough]];
	}
	case GraphicsEngine::RenderMode::Mesh:
	{
		GELogger.Log("RenderMode: Mesh");
		RHI::SetRasterizerState(RasterizerState::RS_Default);
		break;
	}
	case GraphicsEngine::RenderMode::WireFrame:
	{
		GELogger.Log("RenderMode: WireFrame");
		RHI::SetRasterizerState(RasterizerState::RS_Wireframe);
		break;
	}
	default:
		GELogger.Err("RenderMode: Invalid option");
		break;
	}

	return myRenderMode;
}

GraphicsEngine::RenderMode GraphicsEngine::NextRenderMode()
{
	return SetRenderMode(static_cast<RenderMode>(static_cast<int>(myRenderMode) + 1));
}
#endif // _RETAIL

void GraphicsEngine::BeginFrame()
{
	myTextures.ClearTextures(myBackgroundColor);
	myGBuffer.ClearTextures();

	RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, myTextureSlots.DirectionalShadowMapSlot, nullptr); // Directionallight ShadowMap
	for (unsigned i = 0; i < MAX_LIGHTS; i++)
	{
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, myTextureSlots.SpotShadowMapSlot + i, nullptr); // Spotlight ShadowMap
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, myTextureSlots.PointShadowMapSlot + i, nullptr); // Pointlight ShadowMap
	}
	myGBuffer.UnbindAsResource(PIPELINE_STAGE_PIXEL_SHADER);
	LitCmd_ResetLightBuffer reset;
	reset.Execute(0);
}

void GraphicsEngine::EndFrame()
{
	RHI::Present();
	myRenderCommands->lightRenderCommands.clear();
	myRenderCommands->lightCommands.clear();
	myRenderCommands->shadowRenderCommands.clear();
	myRenderCommands->shadowCommands.clear();
	myRenderCommands->deferredRenderCommands.clear();
	myRenderCommands->deferredMeshCommands.clear();
	myRenderCommands->forwardRenderCommands.clear();
	myRenderCommands->forwardMeshCommands.clear();
	myRenderCommands->postProcessRenderCommands.clear();

	myPointLights.clear();
	mySpotLights.clear();

	myDirectionallight = nullptr;
	myDirectionalShadowMap = nullptr;
	for (size_t i = 0; i < MAX_LIGHTS; i++)
	{
		myPointShadowMap[i] = nullptr;
		mySpotShadowMap[i] = nullptr;
	}
}

struct ShadowData
{
	std::shared_ptr<GfxCmd_RenderMeshShadow> myCommand;
	float myDistanceFromLight;
	unsigned myIndex; // Used to check if distance has been calculated for current light

	ShadowData(std::shared_ptr<GfxCmd_RenderMeshShadow>& aCommand, unsigned anIndex) : myCommand(aCommand), myIndex(anIndex), myDistanceFromLight(){}
	bool operator<(const ShadowData& someData) {
		return myDistanceFromLight < someData.myDistanceFromLight;
	}
};

struct DeferredCommandData
{
	std::shared_ptr<GfxCmd_RenderMesh> myCommand;
	float myDistanceFromCamera;

	DeferredCommandData(std::shared_ptr<GfxCmd_RenderMesh>& aCommand, float aDistance) : myCommand(aCommand), myDistanceFromCamera(aDistance){}

	// Sorts Front to Back
	bool operator<(const DeferredCommandData& someData) {
		return myDistanceFromCamera < someData.myDistanceFromCamera;
	}
};

struct ForwardCommandData
{
	std::shared_ptr<GfxCmd_RenderMesh> myCommand;
	float myDistanceFromCamera;

	ForwardCommandData(std::shared_ptr<GfxCmd_RenderMesh>& aCommand, float aDistance) : myCommand(aCommand), myDistanceFromCamera(aDistance){}

	// Sorts Back to Front
	bool operator<(const ForwardCommandData& someData) {
		return myDistanceFromCamera > someData.myDistanceFromCamera;
	}
};

void GraphicsEngine::RenderFrame()
{
	unsigned pointIndex = 0;
	unsigned spotIndex = 0;
	{
		RHI::SetPixelShader(nullptr);

		bool hasDirectional = false;

		// Light Commands
		for (auto& command : myRenderCommands->lightRenderCommands)
		{
			command->Execute();
		}
		for (auto& command : myRenderCommands->lightCommands)
		{
			switch (command->GetType())
			{
			case LightCommand::Type::PointLight:
			{
				myPointLights.emplace_back(command.get());
				if (pointIndex < MAX_LIGHTS)
				{
					command->Execute(pointIndex);
					++pointIndex;
				}
				break;
			}
			case LightCommand::Type::SpotLight:
			{
				mySpotLights.emplace_back(command.get());
				if (spotIndex < MAX_LIGHTS)
				{
					++spotIndex;
				}
				break;
			}
			case LightCommand::Type::Directional:
			{
				if (!hasDirectional)
				{
					myDirectionallight = command.get();
					command->Execute(0);
					hasDirectional = true;
				}
				break;
			}
			case LightCommand::Type::Other:
			{
				command->Execute(0);
				break;
			}
			default:
				break;
			}
		}

		// Shadow Commands
		for (auto& command : myRenderCommands->shadowRenderCommands)
		{
			command->Execute();
		}

		std::vector<ShadowData> objectList;
		objectList.reserve(myRenderCommands->shadowCommands.size());
		for (unsigned i = 0; i < myRenderCommands->shadowCommands.size(); i++)
		{
			objectList.emplace_back(myRenderCommands->shadowCommands[i], i);
		}

		std::unordered_set<unsigned> updatedDistance;
		updatedDistance.reserve(myRenderCommands->shadowCommands.size());
		Crimson::Vector3f position = Crimson::Vector3f::Null;

		// Update position before calling a sorting algorithm with this function, and clear updatedDistance after
		std::function<bool(ShadowData&, ShadowData&)> compare = [&position, &updatedDistance](ShadowData& aFirst, ShadowData& aSecond) -> bool{
			if (updatedDistance.find(aFirst.myIndex) == updatedDistance.end())
			{
				aFirst.myDistanceFromLight = (aFirst.myCommand->GetWorldPosition() - position).LengthSqr();
				updatedDistance.emplace(aFirst.myIndex);
			}
			if (updatedDistance.find(aSecond.myIndex) == updatedDistance.end())
			{
				aSecond.myDistanceFromLight = (aSecond.myCommand->GetWorldPosition() - position).LengthSqr();
				updatedDistance.emplace(aSecond.myIndex);
			}

			return aFirst < aSecond;
		};

		// Create Directionallight shadows
		if (myLightBuffer.Data.CastDirectionalShadows && myLightBuffer.Data.DirectionallightIntensity > 0.f)
		{
			RHI::BeginEvent(L"Directlight Shadows");
			RHI::SetRenderTarget(nullptr, myDirectionalShadowMap);

			// Depth sort
			position = 3.f * myWorldRadius * myLightBuffer.Data.InvertedDirection;
			Crimson::QuickSort(objectList, compare);
			updatedDistance.clear();

			const Crimson::Matrix4x4f& view = Crimson::Matrix4x4f::LookAt(position, myWorldCenter);

			const Crimson::Vector3f& center = Crimson::Vector4f(myWorldCenter, 1.f) * view;
			const Crimson::Vector3f& leftBotNear = center - myWorldRadius;
			const Crimson::Vector3f& rightTopFar = center + myWorldRadius;
			const Crimson::Matrix4x4f& projection = Crimson::Matrix4x4f::CreateOrthographicMatrix(leftBotNear.x, rightTopFar.x, leftBotNear.y, rightTopFar.y, 1.f, myWorldRadius * 4.f);

			myLightBuffer.Data.DirectionalView = view;
			myLightBuffer.Data.DirectionalProjection = projection;
			RHI::UpdateConstantBufferData(myLightBuffer);
			RHI::SetConstantBuffer(PIPELINE_STAGE_PIXEL_SHADER, myLightBufferSlot, myLightBuffer);

			myFrameBuffer.Data.View = view;
			myFrameBuffer.Data.Projection = projection;
			RHI::UpdateConstantBufferData(myFrameBuffer);
			RHI::SetConstantBuffer(PIPELINE_STAGE_VERTEX_SHADER | PIPELINE_STAGE_PIXEL_SHADER, myFrameBufferSlot, myFrameBuffer);

			// Draw to ShadowMap
			for (auto& data : objectList)
			{
				data.myCommand->Execute();
			}
			RHI::EndEvent();
		}

		// Create Spotlight shadows
		//RHI::BeginEvent(L"Spotlight Shadows");
		//for (auto iter = mySpotLights.begin(); iter != mySpotLights.end(); iter++)
		//{
		//	if ((*iter)->CastsShadow()) // TODO: Add culling
		//	{
		//		(*iter)->Execute(0);
		//		auto& spotLight = myLightBuffer.Data.Spotlights[0];
		//		RHI::SetRenderTarget(nullptr, mySpotShadowMap[0]);

		//		// Depth sort
		//		position = spotLight.Position;
		//		Crimson::QuickSort(objectList, compare);
		//		updatedDistance.clear();

		//		const Crimson::Vector3f& target = position + spotLight.LightDirection * spotLight.Range;
		//		const Crimson::Matrix4x4f& view = Crimson::Matrix4x4f::LookAt(position, target);
		//		const Crimson::Matrix4x4f& projection = Crimson::Matrix4x4f::CreatePerspectiveMatrix(spotLight.OuterAngle * 2.f, 1.f, spotLight.Range, { 1.f, 1.f });

		//		spotLight.View = view;
		//		spotLight.Projection = projection;

		//		myFrameBuffer.Data.View = view;
		//		myFrameBuffer.Data.Projection = projection;

		//		RHI::UpdateConstantBufferData(myFrameBuffer);
		//		RHI::SetConstantBuffer(PIPELINE_STAGE_VERTEX_SHADER | PIPELINE_STAGE_PIXEL_SHADER, myFrameBufferSlot, myFrameBuffer);

		//		// Draw to ShadowMap
		//		for (auto& data : objectList)
		//		{
		//			data.myCommand->Execute();
		//		}
		//	}
		//	/*else
		//	{
		//		iter = mySpotLights.erase(iter);
		//	}*/
		//}
		//RHI::EndEvent();

		// Create Pointlight shadows
		// RHI::BeginEvent(L"Bloom");
		//for (auto iter = myPointLights.begin(); iter != myPointLights.end(); iter++)
		//{
		//	if ((*iter)->CastsShadow()) // TODO: Add culling
		//	{
		//		(*iter)->Execute(0);
		//		auto& pointLight = myLightBuffer.Data.Pointlights[0];
		//		RHI::SetRenderTarget(nullptr, myPointShadowMap[0]);
		// 
		//		// Depth sort
		//		position = pointLight.Position;
		//		Crimson::QuickSort(objectList, compare);
		//		updatedDistance.clear();

		//		// Draw to ShadowMap
		//		for (auto& data : objectList)
		//		{
		//			data.myCommand->Execute();
		//		}
		//	}
		//	/*else
		//	{
		//		iter = mySpotLights.erase(iter);
		//	}*/
		//}
		//RHI::EndEvent();

		RHI::SetRenderTarget(nullptr, nullptr);

		// Set ShadowMap
		if (myDirectionallight)
		{
			myDirectionallight->SetShadowMap(0);
		}
	}

	// Deferred Rendering
	{
		RHI::BeginEvent(L"Deferred");
		RHI::SetBlendState(nullptr);
		myGBuffer.SetAsTarget(&myTextures.DepthBuffer);
		RHI::SetPixelShader(&myShaders.GBufferPS);

		std::vector<DeferredCommandData> meshes;
		meshes.reserve(myRenderCommands->deferredMeshCommands.size());

		// Deferred Commands
		for (auto& command : myRenderCommands->deferredRenderCommands)
		{
			command->Execute();
		}

		// Get all meshes distances to camera
		for (auto& command : myRenderCommands->deferredMeshCommands)
		{
			meshes.emplace_back(command, (command->GetWorldPosition() - myFrameBuffer.Data.CameraPosition).LengthSqr());
		}

		Crimson::QuickSort(meshes);

		// Draw to GBuffer
		for (auto& mesh : meshes)
		{
			mesh.myCommand->Execute();
		}

		RHI::SetRenderTarget(&myTextures.Scenebuffer, nullptr);
		myGBuffer.SetAsResource(PIPELINE_STAGE_PIXEL_SHADER);

		// Draw ScreenQuad
		RHI::SetVertexShader(&myShaders.QuadVS);
		RHI::ConfigureInputAssembler(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, nullptr, nullptr, 0, nullptr);

		// Draw Ambient- and Directionallight
		RHI::SetPixelShader(&myShaders.EnvironmentPS);
		RHI::Draw(4);
		RHI::SetBlendState(myAdditiveBlend);
		//RHI::SetBlendState(myAlphaBlend);

#ifndef _RETAIL
		if (myDebugMode == DebugMode::Default)
		{
			if (myLightMode == LightMode::Default || myLightMode == LightMode::SpotLight)
			{
#endif // !_RETAIL

				// Draw Spotlights
				RHI::SetPixelShader(&myShaders.SpotlightPS);
				for (auto& light : mySpotLights)
				{
					light->Execute(0);
					light->SetShadowMap(0);
					RHI::Draw(4);
				}

#ifndef _RETAIL
			}

			if (myLightMode == LightMode::Default || myLightMode == LightMode::PointLight)
			{
#endif // !_RETAIL

				// Draw Pointlight
				RHI::SetPixelShader(&myShaders.PointlightPS);
				for (auto& light : myPointLights)
				{
					light->Execute(0);
					light->SetShadowMap(0);
					RHI::Draw(4);
				}

#ifndef _RETAIL
			}
		}
#endif // !_RETAIL

		RHI::SetBlendState(nullptr);
		RHI::EndEvent();
	}

	// Forward Rendering
	if (myRenderCommands->forwardMeshCommands.size() > 0)
	{
		RHI::BeginEvent(L"Forward");
		RHI::SetRenderTarget(&myTextures.Scenebuffer, &myTextures.DepthBuffer);

		pointIndex = 0;
		for (auto& command : myPointLights)
		{
			if (pointIndex < MAX_LIGHTS)
			{
				command->Execute(pointIndex);
				command->SetShadowMap(pointIndex);
				++pointIndex;
			}
		}

		spotIndex = 0;
		for (auto& command : mySpotLights)
		{
			if (spotIndex < MAX_LIGHTS)
			{
				command->Execute(spotIndex);
				command->SetShadowMap(spotIndex);
				++spotIndex;
			}
		}

		std::vector<ForwardCommandData> meshes;
		meshes.reserve(myRenderCommands->forwardMeshCommands.size());

		// Forward Commands
		for (auto& command : myRenderCommands->forwardRenderCommands)
		{
			command->Execute();
		}

		for (auto& command : myRenderCommands->forwardMeshCommands)
		{
			meshes.emplace_back(command, (command->GetWorldPosition() - myFrameBuffer.Data.CameraPosition).LengthSqr());
		}

		Crimson::QuickSort(meshes);

		for (auto& mesh : meshes)
		{
			mesh.myCommand->Execute();
		}
		RHI::EndEvent();
	}

	// PostProcessing
#ifndef _RETAIL
	if (myDebugMode == DebugMode::Default && myLightMode != LightMode::IgnoreLight)
#endif // !_RETAIL
	{
		RHI::SetRenderTarget(nullptr, nullptr);

		for (auto& command : myRenderCommands->postProcessRenderCommands)
		{
			command->Execute();
		}

		// Render particles

		RHI::SetVertexShader(&myShaders.QuadVS);
		RHI::ConfigureInputAssembler(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, nullptr, nullptr, 0, nullptr);

		// Bloom
		if (myIsUsingBloom)
		{
			RHI::BeginEvent(L"Bloom");
			// Luminance
			RHI::SetRenderTarget(&myTextures.IntermediateA, nullptr);
			RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, myTextureSlots.IntermediateASlot, &myTextures.Scenebuffer);
			RHI::SetPixelShader(&myShaders.LuminancePS);
			RHI::Draw(4);

			// Downsample 1/2
			RHI::SetPixelShader(&myShaders.CopyPS);
			RHI::SetRenderTarget(&myTextures.HalfScenebuffer, nullptr);
			RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, myTextureSlots.IntermediateASlot, &myTextures.IntermediateA);
			RHI::Draw(4);

			// Downsample 1/4
			RHI::SetRenderTarget(&myTextures.QuarterScenebufferA, nullptr);
			RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, myTextureSlots.IntermediateASlot, &myTextures.HalfScenebuffer);
			RHI::Draw(4);

			// Blur
			RHI::SetPixelShader(&myShaders.BlurPS);
			RHI::SetRenderTarget(&myTextures.QuarterScenebufferB, nullptr);
			RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, myTextureSlots.IntermediateASlot, &myTextures.QuarterScenebufferA);
			RHI::Draw(4);
			//RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, myTextureSlots.IntermediateASlot, nullptr); // Release QuarterScenebufferA
			//RHI::SetRenderTarget(&myTextures.QuarterScenebufferA, nullptr);
			//RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, myTextureSlots.IntermediateASlot, &myTextures.QuarterScenebufferB);
			//RHI::Draw(4); QuarterScenebufferA // Set below

			// Upsample
			RHI::SetPixelShader(&myShaders.CopyPS);
			RHI::SetRenderTarget(&myTextures.HalfScenebuffer, nullptr);
			RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, myTextureSlots.IntermediateASlot, &myTextures.QuarterScenebufferB);
			RHI::Draw(4);

			// Bloom
			RHI::SetPixelShader(&myShaders.BloomPS);
			RHI::SetRenderTarget(&myTextures.IntermediateB, nullptr);
			RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, myTextureSlots.IntermediateASlot, &myTextures.Scenebuffer);
			RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, myTextureSlots.IntermediateBSlot, &myTextures.HalfScenebuffer);
			RHI::Draw(4);

#ifndef _RETAIL
			RHI::SetRenderTarget(&myTextures.Scenebuffer, nullptr);
#else
			RHI::SetRenderTarget(&myTextures.BackBuffer, nullptr);
#endif // !_RETAIL
			
			RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, myTextureSlots.IntermediateASlot, &myTextures.IntermediateB);
			RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, myTextureSlots.IntermediateBSlot, nullptr);
			RHI::EndEvent();
		}
		else
		{
#ifndef _RETAIL
			// Copy Scenebuffer onto IntermediateA in order to be able to Gamma correct onto Scenebuffer
			RHI::SetPixelShader(&myShaders.CopyPS);
			RHI::SetRenderTarget(&myTextures.IntermediateA, nullptr);
			RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, myTextureSlots.IntermediateASlot, &myTextures.Scenebuffer);
			RHI::Draw(4);

			RHI::SetRenderTarget(&myTextures.Scenebuffer, nullptr);
			RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, myTextureSlots.IntermediateASlot, &myTextures.IntermediateA);
#else
			RHI::SetRenderTarget(&myTextures.BackBuffer, nullptr);
			RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, myTextureSlots.IntermediateASlot, &myTextures.Scenebuffer);
#endif // !_RETAIL			
		}

		// Gamma correction
		RHI::SetBlendState(nullptr);
		RHI::SetPixelShader(&myShaders.GammaPS);
		RHI::Draw(4);
	}
#ifndef _RETAIL
	RHI::SetRenderTarget(&myTextures.Scenebuffer, &myTextures.DepthBuffer);
#else
	RHI::SetRenderTarget(&myTextures.BackBuffer, &myTextures.DepthBuffer);
#endif // !_RETAIL

	myLineDrawer.Render();

#ifndef _RETAIL
	RHI::SetRenderTarget(&myTextures.BackBuffer, &myTextures.DepthBuffer);
#endif // !_RETAIL
}

void GraphicsEngine::AddGraphicsCommand(std::shared_ptr<GraphicsCommand> aCommand)
{
	switch (aCommand->GetRenderStage())
	{
	case GraphicsCommand::RenderStage::Light:
	{
		myRenderCommands->lightRenderCommands.emplace_back(aCommand);
		break;
	}
	case GraphicsCommand::RenderStage::Shadow:
	{
		myRenderCommands->shadowRenderCommands.emplace_back(aCommand);
		break;
	}
	case GraphicsCommand::RenderStage::Deferred:
	{
		myRenderCommands->deferredRenderCommands.emplace_back(aCommand);
		break;
	}
	case GraphicsCommand::RenderStage::Forward:
	{
		myRenderCommands->forwardRenderCommands.emplace_back(aCommand);
		break;
	}
	case GraphicsCommand::RenderStage::PostProcess:
	{
		myRenderCommands->postProcessRenderCommands.emplace_back(aCommand);
		break;
	}
	default:
		GELogger.Err("Invalid RenderStage in AddGraphicsCommand!");
		break;
	}
}

void GraphicsEngine::AddGraphicsCommand(std::shared_ptr<LightCommand> aCommand)
{
	myRenderCommands->lightCommands.emplace_back(aCommand);
}

void GraphicsEngine::AddGraphicsCommand(std::shared_ptr<GfxCmd_RenderMeshShadow> aCommand)
{
	myRenderCommands->shadowCommands.emplace_back(aCommand);
}

void GraphicsEngine::AddGraphicsCommand(std::shared_ptr<GfxCmd_RenderMesh> aCommand)
{
	switch (aCommand->GetRenderStage())
	{
	case GraphicsCommand::RenderStage::Deferred:
	{
		myRenderCommands->deferredMeshCommands.emplace_back(aCommand);
		break;
	}
	case GraphicsCommand::RenderStage::Forward:
	{
		myRenderCommands->forwardMeshCommands.emplace_back(aCommand);
		break;
	}
	default:
		GELogger.Err("Invalid RenderStage in AddGraphicsCommand -> RenderMesh!");
		break;
	}
}

bool GraphicsEngine::CreateDefaultSampler()
{
	D3D11_SAMPLER_DESC desc{};
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.MipLODBias = 0.f;
	desc.MaxAnisotropy = 1;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.BorderColor[0] = 1.f;
	desc.BorderColor[1] = 1.f;
	desc.BorderColor[2] = 1.f;
	desc.BorderColor[3] = 1.f;
	desc.MinLOD = -D3D11_FLOAT32_MAX;
	desc.MaxLOD = D3D11_FLOAT32_MAX;

	if (!RHI::CreateSamplerState(myDefaultSampler, desc))
	{
		return false;
	}
	return true;
}

bool GraphicsEngine::CreateShadowSampler()
{
	D3D11_SAMPLER_DESC desc{};
	desc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	desc.BorderColor[0] = 0.f;
	desc.BorderColor[1] = 0.f;
	desc.BorderColor[2] = 0.f;
	desc.BorderColor[3] = 0.f;
	desc.MipLODBias = 0.f;
	desc.MaxAnisotropy = 1;
	desc.MinLOD = 0;
	desc.MaxLOD = 0;

	if (!RHI::CreateSamplerState(myShadowSampler, desc))
	{
		return false;
	}
	return true;
}

bool GraphicsEngine::CreateBlurSampler()
{
	D3D11_SAMPLER_DESC desc{};
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.MipLODBias = 0.f;
	desc.MaxAnisotropy = 1;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.BorderColor[0] = 1.f;
	desc.BorderColor[1] = 1.f;
	desc.BorderColor[2] = 1.f;
	desc.BorderColor[3] = 1.f;
	desc.MinLOD = -D3D11_FLOAT32_MAX;
	desc.MaxLOD = D3D11_FLOAT32_MAX;

	if (!RHI::CreateSamplerState(myBlurSampler, desc))
	{
		return false;
	}
	return true;
}

bool GraphicsEngine::CreateLUTSampler()
{
	D3D11_SAMPLER_DESC desc{};
	desc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.MipLODBias = 0.f;
	desc.MaxAnisotropy = 1;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.BorderColor[0] = 1.f;
	desc.BorderColor[1] = 1.f;
	desc.BorderColor[2] = 1.f;
	desc.BorderColor[3] = 1.f;
	desc.MinLOD = 0;
	desc.MaxLOD = 0;

	if (!RHI::CreateSamplerState(myLUTSampler, desc))
	{
		return false;
	}
	return true;
}

bool GraphicsEngine::CreateAlphaBlend()
{
	D3D11_BLEND_DESC blendDesc = {};
	D3D11_RENDER_TARGET_BLEND_DESC& target = blendDesc.RenderTarget[0];

	target.BlendEnable = TRUE;
	target.SrcBlend = D3D11_BLEND_ONE;
	target.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	target.BlendOp = D3D11_BLEND_OP_ADD;
	target.SrcBlendAlpha = D3D11_BLEND_ONE;
	target.DestBlendAlpha = D3D11_BLEND_ONE;
	target.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	target.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	if (!RHI::CreateBlendState(myAlphaBlend, blendDesc))
	{
		return false;
	}
	return true;
}

bool GraphicsEngine::CreateAdditiveBlend()
{
	D3D11_BLEND_DESC blendDesc = {};
	D3D11_RENDER_TARGET_BLEND_DESC& target = blendDesc.RenderTarget[0];

	target.BlendEnable = TRUE;
	target.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	target.DestBlend = D3D11_BLEND_ONE;
	target.BlendOp = D3D11_BLEND_OP_ADD;
	target.SrcBlendAlpha = D3D11_BLEND_ZERO;
	target.DestBlendAlpha = D3D11_BLEND_ONE;
	target.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	target.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	if (!RHI::CreateBlendState(myAdditiveBlend, blendDesc))
	{
		return false;
	}
	return true;
}

bool GraphicsEngine::CreateLUTTexture()
{
	if (!RHI::CreateTexture(&myTextures.BrdfLUTTexture, L"brdfLUT", 512, 512, DXGI_FORMAT_R16G16_FLOAT, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET))
	{
		return false;
	}
	RHI::ClearRenderTarget(&myTextures.BrdfLUTTexture);

	std::wstring path = Settings::GetShaderPath(myAssetPath);

	if (!RHI::LoadShader(&myShaders.QuadVS, path + L"ScreenQuad_VS.cso"))
	{
		return false;
	}

	Shader brdfPS;
	if (!RHI::LoadShader(&brdfPS, path + L"BrdfLUT_PS.cso"))
	{
		return false;
	}

	RHI::SetVertexShader(&myShaders.QuadVS);
	RHI::SetPixelShader(&brdfPS);
	RHI::SetRenderTarget(&myTextures.BrdfLUTTexture, nullptr);
	RHI::ConfigureInputAssembler(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, nullptr, nullptr, 0, nullptr);

	RHI::Draw(4);

	RHI::SetRenderTarget(nullptr, nullptr);
	brdfPS.DeleteData();

	return true;
}

bool GraphicsEngine::CreatePostProcessingTextures()
{
	const RHI::DeviceSize& size = RHI::GetDeviceSize();

	// Scenebuffer
	if (!RHI::CreateTexture(&myTextures.Scenebuffer, L"Scene_Buffer", size.Width, size.Height, DXGI_FORMAT_R16G16B16A16_FLOAT, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET))
	{
		GELogger.Err("Failed to create Scenebuffer texture!");
		return false;
	}
	RHI::ClearRenderTarget(&myTextures.Scenebuffer);

	// Scenebuffer, half size
	if (!RHI::CreateTexture(&myTextures.HalfScenebuffer, L"Half_Scene_Buffer", static_cast<size_t>(size.Width * 0.5f), static_cast<size_t>(size.Height * 0.5f), DXGI_FORMAT_R16G16B16A16_FLOAT, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET))
	{
		GELogger.Err("Failed to create Half_Scenebuffer texture!");
		return false;
	}
	RHI::ClearRenderTarget(&myTextures.HalfScenebuffer);

	// Scenebuffer, quarter size A
	if (!RHI::CreateTexture(&myTextures.QuarterScenebufferA, L"Quarter_Scene_Buffer_A", static_cast<size_t>(size.Width * 0.25f), static_cast<size_t>(size.Height * 0.25f), DXGI_FORMAT_R16G16B16A16_FLOAT, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET))
	{
		GELogger.Err("Failed to create Quarter_Scenebuffer_A texture!");
		return false;
	}
	RHI::ClearRenderTarget(&myTextures.QuarterScenebufferA);

	// Scenebuffer, quarter size B
	if (!RHI::CreateTexture(&myTextures.QuarterScenebufferB, L"Quarter_Scene_Buffer_B", static_cast<size_t>(size.Width * 0.25f), static_cast<size_t>(size.Height * 0.25f), DXGI_FORMAT_R16G16B16A16_FLOAT, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET))
	{
		GELogger.Err("Failed to create Quarter_Scenebuffer_B texture!");
		return false;
	}
	RHI::ClearRenderTarget(&myTextures.QuarterScenebufferB);

	// Intermediate A
	if (!RHI::CreateTexture(&myTextures.IntermediateA, L"Intermediate_A", size.Width, size.Height, DXGI_FORMAT_R16G16B16A16_FLOAT, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET))
	{
		GELogger.Err("Failed to create Intermediate_A texture!");
		return false;
	}
	RHI::ClearRenderTarget(&myTextures.IntermediateA);

	// Intermediate B
	if (!RHI::CreateTexture(&myTextures.IntermediateB, L"Intermediate_B", size.Width, size.Height, DXGI_FORMAT_R16G16B16A16_FLOAT, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET))
	{
		GELogger.Err("Failed to create Intermediate_B texture!");
		return false;
	}
	RHI::ClearRenderTarget(&myTextures.IntermediateB);
	return true;
}

void GraphicsEngine::LoadDefaultTextures(const Settings& someSettings)
{
	// FX
	if (!RHI::LoadTexture(&myTextures.DefaultFXTexture, Crimson::ToWString(someSettings.DefaultFXTexture)))
	{
		GELogger.Err("Failed to load default FX texture!");
	}
	RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, myTextureSlots.DefaultFXTextureSlot, &myTextures.DefaultFXTexture);

	// Material
	if (!RHI::LoadTexture(&myTextures.DefaultMaterialTexture, Crimson::ToWString(someSettings.DefaultMaterialTexture)))
	{
		GELogger.Err("Failed to load default material texture!");
	}
	RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, myTextureSlots.DefaultMaterialTextureSlot, &myTextures.DefaultMaterialTexture);

	// Normal
	if (!RHI::LoadTexture(&myTextures.DefaultNormalTexture, Crimson::ToWString(someSettings.DefaultNormalTexture)))
	{
		GELogger.Err("Failed to load default normal texture!");
	}
	RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, myTextureSlots.DefaultNormalTextureSlot, &myTextures.DefaultNormalTexture);

	// Albedo
	if (!RHI::LoadTexture(&myTextures.DefaultAlbedoTexture, Crimson::ToWString(someSettings.DefaultAlbedoTexture)))
	{
		GELogger.Err("Failed to load default normal texture!");
	}
	RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, myTextureSlots.DefaultAlbedoTextureSlot, &myTextures.DefaultAlbedoTexture);

	// Missing
	if (!RHI::LoadTexture(&myTextures.MissingTexture, Crimson::ToWString(someSettings.DefaultMissingTexture)))
	{
		GELogger.Err("Failed to load default missing texture!");
	}
	RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, myTextureSlots.MissingTextureSlot, &myTextures.MissingTexture);

	// CubeMap
	if (!RHI::LoadTexture(&myTextures.DefaultCubeMap, Crimson::ToWString(someSettings.DefaultCubeMap)))
	{
		GELogger.Err("Failed to load default cubemap!");
	}
	RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, myTextureSlots.DefaultCubeMapSlot, &myTextures.DefaultCubeMap);
}

bool GraphicsEngine::LoadShaders(const Settings& someSettings)
{
	// Default VS
	if (!RHI::LoadShader(&myShaders.DefaultVS, Crimson::ToWString(someSettings.DefaultVSShader)))
	{
		GELogger.Err("Failed to load Default vertex Shader!");
		return false;
	}

	// Default PS
	if (!RHI::LoadShader(&myShaders.DefaultPS, Crimson::ToWString(someSettings.DefaultPSShader)))
	{
		GELogger.Err("Failed to load Default pixel Shader!");
		return false;
	}

	// Luminance
	if (!RHI::LoadShader(&myShaders.LuminancePS, Crimson::ToWString(someSettings.LuminancePS)))
	{
		GELogger.Err("Failed to load Luminance Shader!");
		return false;
	}

	// Blur
	if (!RHI::LoadShader(&myShaders.BlurPS, Crimson::ToWString(someSettings.BlurPS)))
	{
		GELogger.Err("Failed to load Blur Shader!");
		return false;
	}

	// Bloom
	if (!RHI::LoadShader(&myShaders.BloomPS, Crimson::ToWString(someSettings.BloomPS)))
	{
		GELogger.Err("Failed to load Bloom Shader!");
		return false;
	}

	// Gamma
	if (!RHI::LoadShader(&myShaders.GammaPS, Crimson::ToWString(someSettings.GammaPS)))
	{
		GELogger.Err("Failed to load Gamma Shader!");
		return false;
	}

	// Copy
	if (!RHI::LoadShader(&myShaders.CopyPS, Crimson::ToWString(someSettings.CopyPS)))
	{
		GELogger.Err("Failed to load Copy Shader!");
		return false;
	}

	// GBuffer
	if (!RHI::LoadShader(&myShaders.GBufferPS, Crimson::ToWString(someSettings.GBufferPS)))
	{
		GELogger.Err("Failed to load GBuffer Shader!");
		return false;
	}

	// Environment
	if (!RHI::LoadShader(&myShaders.EnvironmentPS, Crimson::ToWString(someSettings.EnvironmentPS)))
	{
		GELogger.Err("Failed to load Environment Shader!");
		return false;
	}

	// Pointlight
	if (!RHI::LoadShader(&myShaders.PointlightPS, Crimson::ToWString(someSettings.PointlightPS)))
	{
		GELogger.Err("Failed to load Pointlight Shader!");
		return false;
	}

	// Spotlight
	if (!RHI::LoadShader(&myShaders.SpotlightPS, Crimson::ToWString(someSettings.SpotlightPS)))
	{
		GELogger.Err("Failed to load Spotlight Shader!");
		return false;
	}
	return true;
}

void GraphicsEngine::LoadMaterial(const Settings& someSettings)
{
	myDefaultMaterial.SetVertexShader(&myShaders.DefaultVS);
	myDefaultMaterial.SetPixelShader(&myShaders.DefaultPS);

	myDefaultMaterial.SetAlbedoTexture(&myTextures.DefaultAlbedoTexture);
	myDefaultMaterial.SetNormalTexture(&myTextures.DefaultAlbedoTexture);
	myDefaultMaterial.SetMaterialTexture(&myTextures.DefaultMaterialTexture);
	myDefaultMaterial.SetFXTexture(&myTextures.DefaultFXTexture);


	myDefaultMaterial.SetShininess(someSettings.DefaultMaterial.Shininess);
	myDefaultMaterial.SetMetalness(someSettings.DefaultMaterial.Metalness);
	myDefaultMaterial.SetNormalStrength(someSettings.DefaultMaterial.NormalStrength);
	myDefaultMaterial.SetUVTiling(someSettings.DefaultMaterial.UVTiling);
	myDefaultMaterial.SetAlbedoColor(someSettings.DefaultMaterial.AlbedoColor);
	myDefaultMaterial.SetEmissionColor(someSettings.DefaultMaterial.EmissionColor);
	myDefaultMaterial.SetEmissionIntensity(someSettings.DefaultMaterial.EmissionIntensity);
}

Settings GraphicsEngine::LoadSettings()
{
	Json::Value json;
	std::fstream fileStream(mySettingsPath, std::ios::in);
	if (fileStream)
	{
		fileStream >> json;
		fileStream.flush();
		fileStream.close();
	}
	else
	{
		GELogger.Err("Could not load settings!");
		fileStream.close();
		return Settings();
	}

	ColorManager::Init(json["Colors"]);
	return Settings(json, myAssetPath);
}

void GraphicsEngine::SaveSettings(const Settings& someSettings) const
{
	std::fstream fileStream(mySettingsPath, std::ios::out);
	if (fileStream)
	{
		Json::Value settings = someSettings;
		settings["Colors"] = ColorManager::ToJson();

		Json::StreamWriterBuilder builder;
		std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
		writer->write(settings, &fileStream);
		fileStream.flush();
	}
	else
	{
		GELogger.Err("Could not save settings!");
	}
	fileStream.close();
}
