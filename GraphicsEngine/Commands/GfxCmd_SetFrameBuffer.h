#pragma once
#include "GraphicsCommand.h"

class GfxCmd_SetFrameBuffer : public GraphicsCommand
{
	CommonUtilities::Matrix4x4f myViewMatrix;
	CommonUtilities::Matrix4x4f myProjectionMatrix;
	CommonUtilities::Vector3f myCameraPosition;

public:
	GfxCmd_SetFrameBuffer(const CommonUtilities::Matrix4x4f& aViewMatrix, const CommonUtilities::Matrix4x4f& aProjectionMatrix, const CommonUtilities::Vector3f& aCameraPosition);

	void Execute() override;
};