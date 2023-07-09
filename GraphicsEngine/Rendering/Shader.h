﻿#pragma once
#include "../InterOp/ShaderInfo.h"

struct ID3D11DeviceChild;
struct ID3D11ShaderReflection;

class Shader
{
	friend class RHI;

	unsigned char* myBlob = nullptr;
	size_t myBlobSize = 0;
	ComPtr<ID3D11DeviceChild> myShaderObject;
	ShaderInfo myShaderInfo{};

	std::wstring myName;

public:
	FORCEINLINE ShaderType GetShaderType() const { return myShaderInfo.Type; }
	FORCEINLINE ComPtr<ID3D11DeviceChild> GetShader() const { return myShaderObject; }
	FORCEINLINE const unsigned char* GetBlob() const { return myBlob; }
	FORCEINLINE size_t GetBlobSize() const { return myBlobSize; }
	FORCEINLINE const ShaderInfo& GetShaderInfo() const { return myShaderInfo; }
	FORCEINLINE const std::wstring& GetName() const { return myName; }
};
