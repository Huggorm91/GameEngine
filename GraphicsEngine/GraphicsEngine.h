#pragma once
#include <memory>
#include "InterOp/RHI.h"
#include "Rendering/Texture.h"
#include "Rendering/Material.h"
#include "Commands/GfxCmd_RenderMeshShadow.h"
#include "Commands/Light/LightCommand.h"
#include "Drawer/LineDrawer.h"

#include <wrl.h>
using namespace Microsoft::WRL;

class GraphicsEngine
{
public:
	static GraphicsEngine& Get() { static GraphicsEngine myInstance; return myInstance;	}

#ifdef _DEBUG
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
#endif // _DEBUG

/**
 * Initializes the Graphics Engine with the specified settings.
 * @param windowHandle The window that will contain this Graphics Engine.
 * @param enableDeviceDebug If DirectX should write debug output in the Output.
 */
	bool Initialize(HWND windowHandle, bool enableDeviceDebug);

	void SetLoggingWindow(HANDLE aHandle);
	void SetBackGroundColor(const CommonUtilities::Vector4f& aColor);

	// Prepares the next frame for rendering by resetting states and clearing all render targets.
	void BeginFrame();
	
	// Finishes a rendered frame and swaps the screen buffers to display it on screen.	
	void EndFrame();
	
	// Renders the current scene to the BackBuffer.	
	void RenderFrame();	
	void AddGraphicsCommand(std::shared_ptr<GraphicsCommand> aCommand);
	void AddGraphicsCommand(std::shared_ptr<LightCommand> aCommand);
	void AddGraphicsCommand(std::shared_ptr<GfxCmd_RenderMeshShadow> aCommand);

	[[nodiscard]] HWND FORCEINLINE GetWindowHandle() const { return myWindowHandle; }
	[[nodiscard]] SIZE FORCEINLINE GetWindowSize() const { return myWindowSize; }

	inline const Material& GetDefaultMaterial() const { return myDefaultMaterial; }
	inline const Texture* GetDefaultCubeMap() const {	return &myDefaultCubeMap; }

	inline LineDrawer& GetLineDrawer() { return myLineDrawer; }

	inline const CommonUtilities::Vector3f& GetWorldBoundsMax() const { return myWorldMax; }
	inline const CommonUtilities::Vector3f& GetWorldBoundsMin() const { return myWorldMin; }

private:
	friend class LightCommand;
	friend class GraphicsCommand;

	struct Settings
	{
		std::string defaultMissingTexture;
		std::string defaultNormalTexture;
		std::string defaultMaterialTexture;
		std::string defaultCubeMap;
		std::string defaultMaterial;
		CommonUtilities::Vector4f backgroundColor;
		
	};

#ifdef _DEBUG
	DebugMode myDebugMode = DebugMode::Default;
	LightMode myLightMode = LightMode::Default;
	RenderMode myRenderMode = RenderMode::Mesh;
#endif // _DEBUG

	LineDrawer::LineHandle myGrid;

	HWND myWindowHandle{};
	ComPtr<ID3D11SamplerState> myDefaultSampler {};
	ComPtr<ID3D11SamplerState> myShadowSampler {};
	ComPtr<ID3D11SamplerState> myLUTSampler {};

	SIZE myWindowSize{0,0};
	CommonUtilities::Vector3f myWorldMax {};
	CommonUtilities::Vector3f myWorldMin {};
	CommonUtilities::Vector4f myBackgroundColor {};
	std::string mySettingsPath {"Settings/ge_settings.json"};

Texture* myDirectionalShadowMap;
Texture* myPointShadowMap[MAX_LIGHTS];
Texture* mySpotShadowMap[MAX_LIGHTS];

	Texture myBackBuffer{};
	Texture myDepthBuffer{};
	Texture myBrdfLUTTexture{};

	Texture myMissingTexture{};
	Texture myDefaultNormalTexture{};
	Texture myDefaultMaterialTexture{};
	Texture myDefaultCubeMap{};
	Material myDefaultMaterial{};

	FrameBuffer myFrameBuffer{};
	ObjectBuffer myObjectBuffer{};
	LightBuffer myLightBuffer{};
	MaterialBuffer myMaterialBuffer{};

	LineDrawer myLineDrawer{};

	std::vector<std::shared_ptr<LightCommand>> myLightCommands {};
	std::vector<std::shared_ptr<GfxCmd_RenderMeshShadow>> myShadowCommands {};
	std::vector<std::shared_ptr<GraphicsCommand>> myRenderCommands {};

	// We're a container singleton, no instancing this outside the class.
	GraphicsEngine() = default;

	bool CreateDefaultSampler();
	bool CreateShadowSampler();
	bool CreateLUTSampler();
	bool CreateLUTTexture();
	Settings LoadSettings();
	void SaveSettings(Settings someSettings);
};

