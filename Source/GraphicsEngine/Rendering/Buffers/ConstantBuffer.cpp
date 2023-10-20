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

bool ConstantBufferBase::Initialize(const std::wstring& aName)
{
	return RHI::CreateConstantBuffer(myBuffer, myDataSize, aName);
}

ConstantBufferBase::~ConstantBufferBase()
{
	myBuffer.Reset();
}