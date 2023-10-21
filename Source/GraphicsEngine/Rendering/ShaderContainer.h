#pragma once
#include "Shader.h"

struct ShaderContainer
{
	Shader DefaultPS{};
	Shader DefaultVS{};
	Shader QuadVS{};

	Shader LuminancePS{};
	Shader BlurPS{};
	Shader BloomPS{};
	Shader GammaPS{};
	Shader CopyPS{};

	Shader GBufferPS{};
	Shader EnvironmentPS{};
	Shader PointlightPS{};
	Shader SpotlightPS{};
};