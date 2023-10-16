#pragma once
#include "GraphicsCommand.h"
#include "../Rendering/Texture.h"

class GfxCmd_ClearRenderTarget : public GraphicsCommand
{
public:
	GfxCmd_ClearRenderTarget(const Texture* aTarget, const Crimson::Vector4f& aColor = Crimson::Vector4f::Null, RenderStage aRenderStage = RenderStage::Deferred);
	~GfxCmd_ClearRenderTarget() override = default;

	void Execute() override;

private:
	const Texture* myTarget;
	Crimson::Vector4f myColor;
};