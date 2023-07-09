#include "GraphicsEngine.pch.h"
#include "ConstantBuffer.h"

ConstantBufferBase::ConstantBufferBase(void* aPtr, size_t aDataSize): myDataSize(aDataSize), myDataPtr(aPtr)
{
}

ConstantBufferBase& ConstantBufferBase::operator=(const ConstantBufferBase& aBase)
{
	myBuffer = aBase.myBuffer;
	return *this;
}

bool ConstantBufferBase::Initialize()
{
	return RHI::CreateConstantBuffer(myBuffer, myDataSize);
}

ConstantBufferBase::~ConstantBufferBase()
{
	myBuffer.Reset();
}