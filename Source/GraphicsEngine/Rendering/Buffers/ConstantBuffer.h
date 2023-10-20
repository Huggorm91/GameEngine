#pragma once

struct ID3D11Buffer;

#include <wrl.h>
using namespace Microsoft::WRL;

/**
 * Provides the base implementation of a Constant Buffer
 * POD object on the CPU.
 */
struct ConstantBufferBase
{
	friend class RHI;
private:
	const size_t myDataSize;
	const void* myDataPtr = nullptr;
	ComPtr<ID3D11Buffer> myBuffer;

protected:
	ConstantBufferBase(void* aPtr, size_t aDataSize);
	ConstantBufferBase& operator=(const ConstantBufferBase& aBase);

public:
	bool Initialize(const std::wstring& aName);
	virtual ~ConstantBufferBase();
};

/**
 * Templated base for Constant Buffers, allowing unified access
 * to contained data via the Data property regardless of type.
 */
template<typename DataStruct>
struct ConstantBuffer : public ConstantBufferBase
{
	DataStruct Data;

	ConstantBuffer(): ConstantBufferBase(&Data, sizeof(DataStruct))	{}
	ConstantBuffer(const ConstantBuffer& aConstantBuffer): ConstantBufferBase(&Data, sizeof(DataStruct)), Data(aConstantBuffer.Data) {}
	ConstantBuffer<DataStruct>& operator=(const ConstantBuffer<DataStruct>& aConstantBuffer);
};

template<typename DataStruct>
ConstantBuffer<DataStruct>& ConstantBuffer<DataStruct>::operator=(const ConstantBuffer<DataStruct>& aConstantBuffer)
{
	ConstantBufferBase::operator=(aConstantBuffer);
	Data = aConstantBuffer.Data;
	return *this;
}