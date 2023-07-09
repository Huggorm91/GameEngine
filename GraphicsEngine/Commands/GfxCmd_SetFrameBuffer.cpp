#include "GraphicsEngine.pch.h"
#include "GfxCmd_SetFrameBuffer.h"
#include <Timer.h>

GfxCmd_SetFrameBuffer::GfxCmd_SetFrameBuffer(const CommonUtilities::Matrix4x4f& aViewMatrix, const CommonUtilities::Matrix4x4f& aProjectionMatrix, const CommonUtilities::Vector3f& aCameraPosition) :
	myViewMatrix(aViewMatrix), myProjectionMatrix(aProjectionMatrix), myCameraPosition(aCameraPosition)
{
}

void GfxCmd_SetFrameBuffer::Execute()
{
	FrameBuffer& buffer = GetFrameBuffer();
	buffer.Data.View = myViewMatrix;
	buffer.Data.Projection = myProjectionMatrix;
	buffer.Data.Position = myCameraPosition;
	buffer.Data.TotalTime = CommonUtilities::Timer::GetTotalTime();
	buffer.Data.DeltaTime = CommonUtilities::Timer::GetDeltaTime();

	RHI::UpdateConstantBufferData(buffer);
	RHI::SetConstantBuffer(PIPELINE_STAGE_VERTEX_SHADER, 0, buffer);
}