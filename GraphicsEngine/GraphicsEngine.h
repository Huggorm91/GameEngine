#pragma once
#include "Settings.h"
#include "InterOp/RHI.h"
#include "Rendering/Texture.h"
#include "Rendering/Material.h"
#include "Rendering/GBuffer.h"
#include "Commands/CommandContainer.h"
#include "Drawer/LineDrawer.h"

#include <wrl.h>
using namespace Microsoft::WRL;

class GraphicsEngine
{
public:
	static GraphicsEngine& Get() {
		static GraphicsEngine myInstance; return myInstance;
	}

#ifndef _RETAIL
	enum class DebugMode
	{
		Default,
		UV,
		PixelNormal,
		Normal,
		Tangent,
		Binormal,
		AmbientOcclusion,
		Roughness,
		Metalness,
		VertexColors,
		AlbedoMap,
		NormalMap,
		DirectionallightUV,
		Count
	};

	enum class LightMode
	{
		Default,
		AmbientLight,
		DirectLight,
		PointLight,
		SpotLight,
		IgnoreLight,
		Count
	};

	enum class RenderMode
	{
		Mesh,
		WireFrame,
		Count
	};

	DebugMode SetDebugMode(DebugMode aMode);
	DebugMode NextDebugMode();

	LightMode SetLightMode(LightMode aMode);
	LightMode NextLightMode();

	RenderMode SetRenderMode(RenderMode aMode);
	RenderMode NextRenderMode();
#endif // !_RETAIL

/**
 * Initializes the Graphics Engine with the specified settings.
 * @param windowHandle The window that will contain this Graphics Engine.
 * @param enableDeviceDebug If DirectX should write debug output in the Output.
 */
	bool Initialize(HWND windowHandle, bool enableDeviceDebug);

	// Saves current settings to file
	void SaveSettings() const;

	void SetLoggingWindow(HANDLE aHandle);
	void SetBackGroundColor(const CommonUtilities::Vector4f& aColor);

	// Swaps the render buffers. Should be called before BeginFrame or after EndFrame
	void Swap();

	// Prepares the next frame for rendering by resetting states and clearing all render targets.
	void BeginFrame();

	// Finishes a rendered frame and swaps the screen buffers to display it on screen.	
	void EndFrame();

	// Renders the current scene to the BackBuffer.	
	void RenderFrame();
	void AddGraphicsCommand(std::shared_ptr<GraphicsCommand> aCommand);
	void AddGraphicsCommand(std::shared_ptr<LightCommand> aCommand);
	void AddGraphicsCommand(std::shared_ptr<GfxCmd_RenderMeshShadow> aCommand);
	void AddGraphicsCommand(std::shared_ptr<GfxCmd_RenderMesh> aCommand);

	[[nodiscard]] HWND FORCEINLINE GetWindowHandle() const {
		return myWindowHandle;
	}
	[[nodiscard]] SIZE FORCEINLINE GetWindowSize() const {
		return myWindowSize;
	}

	inline const Material& GetDefaultMaterial() const {
		return myDefaultMaterial;
	}
	inline const Texture* GetDefaultCubeMap() const {
		return &myDefaultCubeMap;
	}

	inline LineDrawer& GetLineDrawer() {
		return myLineDrawer;
	}

	inline const CommonUtilities::Vector3f& GetWorldBoundsMax() const {
		return myWorldMax;
	}
	inline const CommonUtilities::Vector3f& GetWorldBoundsMin() const {
		return myWorldMin;
	}
	inline const CommonUtilities::Vector3f& GetWorldBoundsOrigin() const {
		return myWorldCenter;
	}

private:
	friend class LightCommand;
	friend class GraphicsCommand;	

#ifndef _RETAIL
	DebugMode myDebugMode;
	LightMode myLightMode;
	RenderMode myRenderMode;
	LineDrawer::LineHandle myGrid;
#endif // !_RETAIL	

	HWND myWindowHandle;
	ComPtr<ID3D11SamplerState> myDefaultSampler;
	ComPtr<ID3D11SamplerState> myShadowSampler;
	ComPtr<ID3D11SamplerState> myLUTSampler;

	ComPtr<ID3D11BlendState> myAlphaBlend;
	ComPtr<ID3D11BlendState> myAdditiveBlend;

	CommandContainer* myRenderCommands;
	CommandContainer* myUpdateCommands;
	LightCommand* myDirectionallight;

	float myWorldRadius;
	SIZE myWindowSize;
	CommonUtilities::Vector3f myWorldMax;
	CommonUtilities::Vector3f myWorldMin;
	CommonUtilities::Vector3f myWorldCenter;
	CommonUtilities::Vector4f myBackgroundColor;
	std::string mySettingsPath;

	Texture* myDirectionalShadowMap;
	std::array<Texture*, MAX_LIGHTS> myPointShadowMap;
	std::array<Texture*, MAX_LIGHTS> mySpotShadowMap;

	Shader myQuadVSShader;
	Shader myGBufferPSShader;
	Shader myEnvironmentPSShader;
	Shader myPointlightPSShader;
	Shader mySpotlightPSShader;

	Texture myBackBuffer;
	Texture myDepthBuffer;
	Texture myBrdfLUTTexture;

	Texture myMissingTexture;
	Texture myDefaultNormalTexture;
	Texture myDefaultMaterialTexture;
	Texture myDefaultCubeMap;
	Material myDefaultMaterial;
	GBuffer myGBuffer;

	FrameBuffer myFrameBuffer;
	ObjectBuffer myObjectBuffer;
	LightBuffer myLightBuffer;
	MaterialBuffer myMaterialBuffer;

	LineDrawer myLineDrawer;

	std::vector<LightCommand*> myPointLights;
	std::vector<LightCommand*> mySpotLights;

	CommandContainer myFirstCommandlist;
	CommandContainer mySecondCommandlist;

	// We're a container singleton, no instancing this outside the class.
	GraphicsEngine();

	bool CreateDefaultSampler();
	bool CreateShadowSampler();
	bool CreateLUTSampler();

	bool CreateAlphaBlend();
	bool CreateAdditiveBlend();

	bool CreateLUTTexture();

	Settings LoadSettings();
	void SaveSettings(const Settings& someSettings) const;
};

