#include "GraphicsEngine.pch.h"
#include "GraphicsEngine.h"
#include "AssetManager/AssetManager.h"
#include <filesystem>
#include <utility>
#include "Rendering/Vertex.h"
#include "Commands/Light/LitCmd_ResetLightBuffer.h"
#include "Vector3.hpp"

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

		if (!RHI::LoadTexture(&myDefaultMaterialTexture, L"Content/Textures/Default/T_Default_M.dds"))
		{
			GELogger.Err("Failed to load default material texture!");
		}
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 97, &myDefaultMaterialTexture);

		if (!RHI::LoadTexture(&myDefaultNormalTexture, L"Content/Textures/Default/T_Default_N_Flat.dds"))
		{
			GELogger.Err("Failed to load default normal texture!");
		}
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 98, &myDefaultNormalTexture);

		if (!RHI::LoadTexture(&myMissingTexture, L"Content/Textures/Default/T_Missing_C.dds"))
		{
			GELogger.Err("Failed to load default missing texture!");
		}
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 99, &myMissingTexture);

		if (!RHI::LoadTexture(&myDefaultCubeMap, L"Content/Textures/CubeMaps/skansen_cubemap.dds"))
		{
			GELogger.Err("Failed to load default cubemap!");
		}
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 100, &myDefaultCubeMap);

		if (!CreateDefaultSampler())
		{
			GELogger.Err("Failed to create default sampler!");
		}
		RHI::SetSamplerState(myDefaultSampler, 0);

		myDefaultMaterial = AssetManager::GetAsset<Material>("Content/Materials/Default/Default_Material.mat");
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
	myBackGroundColor = aColor;
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
		break;
	}
	case GraphicsEngine::RenderMode::WireFrame:
	{
		GELogger.Log("RenderMode: WireFrame");
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
	RHI::ClearRenderTarget(&myBackBuffer, { myBackGroundColor.x, myBackGroundColor.y, myBackGroundColor.z, myBackGroundColor.w });
	RHI::ClearDepthStencil(&myDepthBuffer);
	LitCmd_ResetLightBuffer reset;
	reset.Execute(0);
}

void GraphicsEngine::EndFrame()
{
	RHI::Present();
	myRenderCommands.clear();
	myLightCommands.clear();
}

void GraphicsEngine::RenderFrame()
{
	RHI::SetRenderTarget(&myBackBuffer, &myDepthBuffer);

	{
		int pointIndex = 0;
		int spotIndex = 0;
		for (auto& command : myLightCommands)
		{
			switch (command->GetType())
			{
			case LightCommand::Type::PointLight:
			{
				command->Execute(pointIndex);
				if (pointIndex < 8)
				{
					++pointIndex;
				}
				break;
			}
			case LightCommand::Type::SpotLight:
			{
				command->Execute(spotIndex);
				if (spotIndex < 8)
				{
					++spotIndex;
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
			command->~LightCommand();
		}
	}

	for (auto& command : myRenderCommands)
	{
		command->Execute();
		command->~GraphicsCommand();
	}

	myLineDrawer.Render();
}

void GraphicsEngine::AddGraphicsCommand(std::shared_ptr<GraphicsCommand> aCommand)
{
	myRenderCommands.emplace_back(aCommand);
}

void GraphicsEngine::AddGraphicsCommand(std::shared_ptr<LightCommand> aCommand)
{
	myLightCommands.emplace_back(aCommand);
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