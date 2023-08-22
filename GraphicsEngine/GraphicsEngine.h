#pragma once
#include <memory>
#include "InterOp/RHI.h"
#include "Rendering/Texture.h"
#include "Rendering/Material.h"
#include "Commands/GraphicsCommand.h"
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

	[[nodiscard]] HWND FORCEINLINE GetWindowHandle() const { return myWindowHandle; }
	[[nodiscard]] SIZE FORCEINLINE GetWindowSize() const { return myWindowSize; }
	inline const Material& GetDefaultMaterial() const { return myDefaultMaterial; }
	inline const Texture* GetDefaultCubeMap() const {	return &myDefaultCubeMap; }
	inline LineDrawer& GetLineDrawer() { return myLineDrawer; }

private:
	friend class LightCommand;
	friend class GraphicsCommand;

#ifdef _DEBUG
	DebugMode myDebugMode = DebugMode::Default;
	LightMode myLightMode = LightMode::Default;
	RenderMode myRenderMode = RenderMode::Mesh;
#endif // _DEBUG

	LineDrawer::LineHandle myGrid;

	HWND myWindowHandle{};
	ComPtr<ID3D11SamplerState> myDefaultSampler {};

	SIZE myWindowSize{0,0};
	CommonUtilities::Vector4f myBackGroundColor {0.f, 0.70980392156f, 0.8862745098f, 1.f};

	Shader myDefaultVertexShader{};
	Shader myDefaultPixelShader{};

	Texture myBackBuffer{};
	Texture myDepthBuffer{};

	Texture myDefaultNormalTexture{};
	Texture myMissingTexture{};
	Texture myDefaultCubeMap{};
	Material myDefaultMaterial{};

	FrameBuffer myFrameBuffer{};
	ObjectBuffer myObjectBuffer{};
	LightBuffer myLightBuffer{};
	MaterialBuffer myMaterialBuffer{};

	LineDrawer myLineDrawer{};

	std::vector<std::shared_ptr<LightCommand>> myLightCommands {};
	std::vector<std::shared_ptr<GraphicsCommand>> myRenderCommands {};

	// We're a container singleton, no instancing this outside the class.
	GraphicsEngine() = default;

	bool CreateDefaultSampler();
};

