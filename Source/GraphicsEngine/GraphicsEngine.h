#pragma once
#include "Settings.h"
#include "InterOp/RHI.h"
#include "Rendering/TextureContainer.h"
#include "Rendering/ShaderContainer.h"
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

	enum class ToneMap
	{
		None,
		Reinhard,
		UnrealEngine,
		ACES,
		Lottes,
		Count
	};

	ToneMap SetToneMap(ToneMap aMode);
	ToneMap NextToneMap();

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
		Emission,
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
	void SetBackGroundColor(const Crimson::Vector3f& aColor);

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
		return &myTextures.DefaultCubeMap;
	}

	inline LineDrawer& GetLineDrawer() {
		return myLineDrawer;
	}

	inline const Crimson::Vector3f& GetWorldBoundsMax() const {
		return myWorldMax;
	}
	inline const Crimson::Vector3f& GetWorldBoundsMin() const {
		return myWorldMin;
	}
	inline const Crimson::Vector3f& GetWorldBoundsOrigin() const {
		return myWorldCenter;
	}

	inline void SetUsingBloom(bool aState){
		myIsUsingBloom = aState;
	}
	inline void ToogleUsingBloom(){
		myIsUsingBloom = !myIsUsingBloom;
	}

private:
	friend class LightCommand;
	friend class GraphicsCommand;

	bool myIsUsingBloom;

	ToneMap myToneMap;
#ifndef _RETAIL
	DebugMode myDebugMode;
	LightMode myLightMode;
	RenderMode myRenderMode;
	LineHandle myGrid;
#endif // !_RETAIL	
	unsigned myFrameBufferSlot;
	unsigned myObjectBufferSlot;
	unsigned myLightBufferSlot;
	unsigned myMaterialBufferSlot;

	HWND myWindowHandle;
	ComPtr<ID3D11SamplerState> myDefaultSampler;
	ComPtr<ID3D11SamplerState> myShadowSampler;
	ComPtr<ID3D11SamplerState> myBlurSampler;
	ComPtr<ID3D11SamplerState> myLUTSampler;

	ComPtr<ID3D11BlendState> myAlphaBlend;
	ComPtr<ID3D11BlendState> myAdditiveBlend;

	CommandContainer* myRenderCommands;
	CommandContainer* myUpdateCommands;
	LightCommand* myDirectionallight;

	float myWorldRadius;
	SIZE myWindowSize;
	Crimson::Vector3f myWorldMax;
	Crimson::Vector3f myWorldMin;
	Crimson::Vector3f myWorldCenter;
	Crimson::Vector3f myBackgroundColor;
	std::string mySettingsPath;

	Texture* myDirectionalShadowMap;
	std::array<Texture*, MAX_LIGHTS> myPointShadowMap;
	std::array<Texture*, MAX_LIGHTS> mySpotShadowMap;

	SlotContainer myTextureSlots;
	Material myDefaultMaterial;
	GBuffer myGBuffer;
	ShaderContainer myShaders;
	TextureContainer myTextures;

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
	bool CreateBlurSampler();
	bool CreateLUTSampler();

	bool CreateAlphaBlend();
	bool CreateAdditiveBlend();

	bool CreateLUTTexture();
	bool CreatePostProcessingTextures();

	void LoadDefaultTextures(const Settings& someSettings);
	bool LoadShaders(const Settings& someSettings);

	Settings LoadSettings();
	void SaveSettings(const Settings& someSettings) const;
};

