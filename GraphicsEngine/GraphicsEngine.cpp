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

		if (!RHI::LoadTexture(&myDefaultNormalTexture, L"Content/Textures/Default/T_Default_N.dds"))
		{
			GELogger.Err("Failed to load default normal texture!");
		}
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 98, &myDefaultNormalTexture);

		if (!RHI::LoadTexture(&myMissingTexture, L"Content/Textures/Default/T_Missing_M.dds"))
		{
			GELogger.Err("Failed to load missing texture!");
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
void GraphicsEngine::SetRenderMode(RenderMode aMode)
{
	myRendermode = aMode;

	// TODO: Set the correct Shaders as active shaders for each RenderMode
	switch (myRendermode)
	{
	case GraphicsEngine::Default:
	{
		break;
	}
	case GraphicsEngine::UV:
	{
		break;
	}
	case GraphicsEngine::PixelNormal:
	{
		break;
	}
	case GraphicsEngine::Normal:
	{
		break;
	}
	case GraphicsEngine::Tangent:
	{
		break;
	}
	case GraphicsEngine::Binormal:
	{
		break;
	}
	case GraphicsEngine::AmbientLight:
	{
		break;
	}
	case GraphicsEngine::DirectLight:
	{
		break;
	}
	case GraphicsEngine::PointLight:
	{
		break;
	}
	case GraphicsEngine::SpotLight:
	{
		break;
	}
	default:
		break;
	}
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