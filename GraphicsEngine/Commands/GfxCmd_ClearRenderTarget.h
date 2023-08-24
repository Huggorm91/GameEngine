#pragma once
#include "GraphicsCommand.h"
#include "../Rendering/Texture.h"

class GfxCmd_ClearRenderTarget : public GraphicsCommand
{
public:
	GfxCmd_ClearRenderTarget(const Texture* aTarget, const CommonUtilities::Vector4f& aColor = CommonUtilities::Vector4f::Null);

	void Execute() override;

private:
	const Texture* myTarget;
	CommonUtilities::Vector4f myColor;
};