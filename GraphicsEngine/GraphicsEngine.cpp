#include "GraphicsEngine.pch.h"
#include "GraphicsEngine.h"
#include "AssetManager/AssetManager.h"
#include <filesystem>
#include <utility>
#include "Rendering/Vertex.h"
#include "Commands/Light/LitCmd_ResetLightBuffer.h"
#include <Vector3.hpp>
#include <Container/MinHeap.hpp>
#include <External/jsonCpp/json.h>
#include "InterOp/Helpers.h"
#include <fstream>
#include <Sort.hpp>

GraphicsEngine::GraphicsEngine() :myWindowHandle(), myDefaultSampler(), myShadowSampler(), myLUTSampler(), myWindowSize{ 0,0 }, myWorldMax(), myWorldMin(), myBackgroundColor(), mySettingsPath("Settings/ge_settings.json"), myRenderCommands(&myFirstCommandlist), 
myUpdateCommands(&mySecondCommandlist), myDirectionalShadowMap(nullptr), myPointShadowMap{ nullptr }, mySpotShadowMap{ nullptr }, myBackBuffer(), myDepthBuffer(), myBrdfLUTTexture(), myMissingTexture(), myDefaultNormalTexture(), myDefaultMaterialTexture(), myDefaultCubeMap(),
myDefaultMaterial(), myFrameBuffer(), myObjectBuffer(), myLightBuffer(), myMaterialBuffer(), myLineDrawer(), myFirstCommandlist(), mySecondCommandlist()
#ifdef _DEBUG
,myDebugMode(DebugMode::Default), myLightMode (LightMode::Default), myRenderMode (RenderMode::Mesh)
#endif // _DEBUG
#ifndef _RETAIL
,myGrid()
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
		GELogger.SetPrintToVSOutput(false);

		myWindowHandle = windowHandle;

		if (!RHI::Initialize(myWindowHandle,
							 enableDeviceDebug,
							 &myBackBuffer,
							 &myDepthBuffer))
		{
			GELogger.Err("Failed to initialize the RHI!");
			return false;
		}

		Settings settings = LoadSettings();
		myBackgroundColor = settings.backgroundColor;

		if (!CreateLUTTexture())
		{
			GELogger.Err("Failed to create LUT texture!");
		}
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 96, &myBrdfLUTTexture);

		if (!RHI::LoadTexture(&myDefaultMaterialTexture, Helpers::string_cast<std::wstring>(settings.defaultMaterialTexture)))
		{
			GELogger.Err("Failed to load default material texture!");
		}
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 97, &myDefaultMaterialTexture);

		if (!RHI::LoadTexture(&myDefaultNormalTexture, Helpers::string_cast<std::wstring>(settings.defaultNormalTexture)))
		{
			GELogger.Err("Failed to load default normal texture!");
		}
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 98, &myDefaultNormalTexture);

		if (!RHI::LoadTexture(&myMissingTexture, Helpers::string_cast<std::wstring>(settings.defaultMissingTexture)))
		{
			GELogger.Err("Failed to load default missing texture!");
		}
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 99, &myMissingTexture);

		if (!RHI::LoadTexture(&myDefaultCubeMap, Helpers::string_cast<std::wstring>(settings.defaultCubeMap)))
		{
			GELogger.Err("Failed to load default cubemap!");
		}
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 100, &myDefaultCubeMap);

		if (!CreateDefaultSampler())
		{
			GELogger.Err("Failed to create default sampler!");
		}
		RHI::SetSamplerState(myDefaultSampler, 0);

		if (!CreateShadowSampler())
		{
			GELogger.Err("Failed to create shadow sampler!");
		}
		RHI::SetSamplerState(myShadowSampler, 14);

		if (!CreateLUTSampler())
		{
			GELogger.Err("Failed to create LUT sampler!");
		}
		RHI::SetSamplerState(myLUTSampler, 15);

		myDefaultMaterial = AssetManager::GetAsset<Material>(settings.defaultMaterial);
		if (!RHI::CreateInputLayout(Vertex::InputLayout, Vertex::InputLayoutDefinition, myDefaultMaterial.GetVertexShader()->GetBlob(), myDefaultMaterial.GetVertexShader()->GetBlobSize()))
		{
			GELogger.Err("Failed to create InputLayout!");
			return false;
		}

		myFrameBuffer.Initialize();
		myObjectBuffer.Initialize();
		myLightBuffer.Initialize();
		myMaterialBuffer.Initialize();

		if (!myLineDrawer.Init())
		{
			GELogger.Err("Failed to initialize LineDrawer!");
			return false;
		}

#ifndef _RETAIL
		myGrid = myLineDrawer.AddAxisLines(CommonUtilities::Vector3f::Null, 10000000.f, true);
	}
	catch (const std::exception& e)
	{
		GELogger.LogException(e);
		exit(-1);
	}
#endif	

	return true;
}

void GraphicsEngine::SetLoggingWindow(HANDLE aHandle)
{
	GELogger.SetConsoleHandle(aHandle);
}

void GraphicsEngine::SetBackGroundColor(const CommonUtilities::Vector4f& aColor)
{
	myBackgroundColor = aColor;
}

void GraphicsEngine::Swap()
{

}

#ifdef _DEBUG
GraphicsEngine::DebugMode GraphicsEngine::SetDebugMode(DebugMode aMode)
{
	myDebugMode = aMode;

	// TODO: Set the correct Shaders as active shaders for each DebugMode
	switch (myDebugMode)
	{
	case GraphicsEngine::DebugMode::Count:
	{
		myDebugMode = DebugMode::Default;
		// Letting it fall through
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
	default:
		GELogger.Err("DebugMode: Invalid option");
		return myDebugMode;
	}

	myFrameBuffer.Data.myDebugMode = static_cast<int>(myDebugMode);
	RHI::UpdateConstantBufferData(myFrameBuffer);
	RHI::SetConstantBuffer(PIPELINE_STAGE_VERTEX_SHADER | PIPELINE_STAGE_PIXEL_SHADER, 0, myFrameBuffer);

	return myDebugMode;
}

GraphicsEngine::DebugMode GraphicsEngine::NextDebugMode()
{
	return SetDebugMode(static_cast<DebugMode>(static_cast<int>(myDebugMode) + 1));
}

GraphicsEngine::LightMode GraphicsEngine::SetLightMode(LightMode aMode)
{
	myLightMode = aMode;

	// TODO: Set the correct Shaders as active shaders for each LightMode
	switch (myLightMode)
	{
	case GraphicsEngine::LightMode::Count:
	{
		myLightMode = LightMode::Default;
		// Letting it fall through
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

	myLightBuffer.Data.myLightMode = static_cast<int>(myLightMode);
	RHI::UpdateConstantBufferData(myLightBuffer);
	RHI::SetConstantBuffer(PIPELINE_STAGE_VERTEX_SHADER | PIPELINE_STAGE_PIXEL_SHADER, 2, myLightBuffer);

	return myLightMode;
}

GraphicsEngine::LightMode GraphicsEngine::NextLightMode()
{
	return SetLightMode(static_cast<LightMode>(static_cast<int>(myLightMode) + 1));
}

GraphicsEngine::RenderMode GraphicsEngine::SetRenderMode(RenderMode aMode)
{
	myRenderMode = aMode;

	// TODO: Set the correct Shaders as active shaders for each RenderMode
	switch (myRenderMode)
	{
	case GraphicsEngine::RenderMode::Count:
	{
		myRenderMode = RenderMode::Mesh;
		// Letting it fall through
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
#endif // _DEBUG

void GraphicsEngine::BeginFrame()
{
	RHI::ClearRenderTarget(&myBackBuffer, { myBackgroundColor.x, myBackgroundColor.y, myBackgroundColor.z, myBackgroundColor.w });
	RHI::ClearDepthStencil(&myDepthBuffer);
	LitCmd_ResetLightBuffer reset;
	reset.Execute(0);
	myRenderCommands->renderCommands.emplace_back();
	myRenderCommands->meshCommands.emplace_back();
}

void GraphicsEngine::EndFrame()
{
	RHI::Present();
	myRenderCommands->renderCommands.clear();
	myRenderCommands->lightCommands.clear();
	myRenderCommands->shadowCommands.clear();
	myRenderCommands->meshCommands.clear();
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

struct MeshCommandData
{
	std::shared_ptr<GfxCmd_RenderMesh> myCommand;
	float myDistanceFromCamera;

	MeshCommandData(std::shared_ptr<GfxCmd_RenderMesh>& aCommand, float aDistance) : myCommand(aCommand), myDistanceFromCamera(aDistance){}
	bool operator<(const MeshCommandData& someData) {
		return myDistanceFromCamera < someData.myDistanceFromCamera;
	}
};

void GraphicsEngine::RenderFrame()
{
	RHI::SetPixelShader(nullptr);
	{
		int pointIndex = 0;
		int spotIndex = 0;
		bool hasDirectional = false;
		for (auto& command : myRenderCommands->lightCommands)
		{
			switch (command->GetType())
			{
			case LightCommand::Type::PointLight:
			{
				if (pointIndex < 8)
				{
					command->Execute(pointIndex);
					++pointIndex;
				}
				break;
			}
			case LightCommand::Type::SpotLight:
			{
				if (spotIndex < 8)
				{
					command->Execute(spotIndex);
					++spotIndex;
				}
				break;
			}
			case LightCommand::Type::Directional:
			{
				if (!hasDirectional)
				{
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

		std::vector<ShadowData> objectList;
		objectList.reserve(myRenderCommands->shadowCommands.size());
		for (unsigned i = 0; i < myRenderCommands->shadowCommands.size(); i++)
		{
			objectList.emplace_back(myRenderCommands->shadowCommands[i], i);
		}

		std::unordered_set<unsigned> updatedDistance;
		CommonUtilities::Vector3f position = { 10.f, 10.f, 10.f };

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

		if (myLightBuffer.Data.myDirectionallightIntensity > 0.f)
		{
			RHI::SetRenderTarget(nullptr, myDirectionalShadowMap);
			//position = directLightPos;
			CommonUtilities::QuickSort(objectList, compare);
			updatedDistance.clear();

			for (auto& data : objectList)
			{
				//data.myCommand->Execute();
			}
		}

		for (int i = 0; i < MAX_LIGHTS; i++)
		{
			if (mySpotShadowMap[i])
			{
				RHI::SetRenderTarget(nullptr, mySpotShadowMap[i]);
				// calculate spotlight shadows
			}

			if (myPointShadowMap[i])
			{
				RHI::SetRenderTarget(nullptr, myPointShadowMap[i]);
				// calculate point light shadows
			}
		}

		pointIndex = 0;
		spotIndex = 0;
		hasDirectional = false;
		for (auto& command : myRenderCommands->lightCommands)
		{
			switch (command->GetType())
			{
			case LightCommand::Type::PointLight:
			{
				if (pointIndex < 8)
				{
					command->SetShadowMap(pointIndex);
					++pointIndex;
				}
				break;
			}
			case LightCommand::Type::SpotLight:
			{
				if (spotIndex < 8)
				{
					command->SetShadowMap(spotIndex);
					++spotIndex;
				}
				break;
			}
			case LightCommand::Type::Directional:
			{
				if (!hasDirectional)
				{
					command->SetShadowMap(0);
					hasDirectional = true;
				}
				break;
			}
			default:
				break;
			}
		}
	}

	{
		RHI::SetRenderTarget(&myBackBuffer, &myDepthBuffer);

		std::vector<MeshCommandData> meshes;
		meshes.reserve(myRenderCommands->meshCommands[0].size());
		for (size_t i = 0; i < myRenderCommands->renderCommands.size(); i++)
		{
			for (auto& command : myRenderCommands->renderCommands[i])
			{
				command->Execute();
			}
			for (auto& command : myRenderCommands->meshCommands[i])
			{
				meshes.emplace_back(command, (command->GetWorldPosition() - myFrameBuffer.Data.CameraPosition).LengthSqr());
			}

			CommonUtilities::QuickSort(meshes);

			for (auto& mesh : meshes)
			{
				mesh.myCommand->Execute();
			}
		}
	}

	myLineDrawer.Render();
}

void GraphicsEngine::AddGraphicsCommand(std::shared_ptr<GraphicsCommand> aCommand)
{
	myRenderCommands->renderCommands.back().emplace_back(aCommand);
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
	myRenderCommands->meshCommands.back().emplace_back(aCommand);
}

void GraphicsEngine::AddGraphicsCommand(std::shared_ptr<GfxCmd_NewRenderlist> aCommand)
{
	myRenderCommands->renderCommands.emplace_back();
	myRenderCommands->meshCommands.emplace_back();
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

	if (!RHI::CreateSamplerState(myLUTSampler, desc))
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

bool GraphicsEngine::CreateLUTTexture()
{
	if (!RHI::CreateTexture(&myBrdfLUTTexture, L"brdfLUT", 512, 512, DXGI_FORMAT_R16G16_FLOAT, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET))
	{
		return false;
	}
	RHI::ClearRenderTarget(&myBrdfLUTTexture);

	Shader brdfVS;
#ifdef _DEBUG
	std::wstring vsPath = L"Content/Shaders/Debug/BrdfLUT_VS.cso";
#elif _RELEASE
	std::wstring vsPath = L"Content/Shaders/Release/BrdfLUT_VS.cso";
#elif _RETAIL
	std::wstring vsPath = L"Content/Shaders/Retail/BrdfLUT_VS.cso";
#endif // _DEBUG
	if (!RHI::LoadShader(&brdfVS, vsPath))
	{
		return false;
	}

	Shader brdfPS;
#ifdef _DEBUG
	std::wstring psPath = L"Content/Shaders/Debug/BrdfLUT_PS.cso";
#elif _RELEASE
	std::wstring psPath = L"Content/Shaders/Release/BrdfLUT_PS.cso";
#elif _RETAIL
	std::wstring psPath = L"Content/Shaders/Retail/BrdfLUT_PS.cso";
#endif // _DEBUG
	if (!RHI::LoadShader(&brdfPS, psPath))
	{
		return false;
	}

	RHI::SetVertexShader(&brdfVS);
	RHI::SetPixelShader(&brdfPS);
	RHI::SetRenderTarget(&myBrdfLUTTexture, nullptr);
	RHI::ConfigureInputAssembler(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, nullptr, nullptr, 0, nullptr);

	RHI::Draw(4);

	RHI::SetRenderTarget(nullptr, nullptr);
	brdfVS.DeleteData();
	brdfPS.DeleteData();

	return true;
}

GraphicsEngine::Settings GraphicsEngine::LoadSettings()
{
	Settings settings;
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
		return settings;
	}

	settings.defaultMaterialTexture = json["MaterialTexture"].asString();
	settings.defaultNormalTexture = json["NormalTexture"].asString();
	settings.defaultMissingTexture = json["MissingTexture"].asString();
	settings.defaultCubeMap = json["CubeMap"].asString();
	settings.defaultMaterial = json["DefaultMaterial"].asString();
	settings.backgroundColor.x = json["BackgroundColor"]["R"].asFloat();
	settings.backgroundColor.y = json["BackgroundColor"]["G"].asFloat();
	settings.backgroundColor.z = json["BackgroundColor"]["B"].asFloat();
	settings.backgroundColor.w = json["BackgroundColor"]["A"].asFloat();
	return settings;
}

void GraphicsEngine::SaveSettings(Settings someSettings)
{
	Json::Value json;
	json["MaterialTexture"] = someSettings.defaultMaterialTexture;
	json["NormalTexture"] = someSettings.defaultNormalTexture;
	json["MissingTexture"] = someSettings.defaultMissingTexture;
	json["CubeMap"] = someSettings.defaultCubeMap;
	json["DefaultMaterial"] = someSettings.defaultMaterial;
	json["BackgroundColor"]["R"] = someSettings.backgroundColor.x;
	json["BackgroundColor"]["G"] = someSettings.backgroundColor.y;
	json["BackgroundColor"]["B"] = someSettings.backgroundColor.z;
	json["BackgroundColor"]["A"] = someSettings.backgroundColor.w;

	std::fstream fileStream(mySettingsPath, std::ios::out);
	if (fileStream)
	{
		Json::StreamWriterBuilder builder;
		std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
		writer->write(json, &fileStream);
		fileStream.flush();
	}
	else
	{
		GELogger.Err("Could not save settings!");
	}
	fileStream.close();
}
