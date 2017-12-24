
#include "HullShader.h"
#include "../../DeviceResources/DeviceResources.h"

Utility::HullShader::HullShader(ID3D11DeviceContext * argContext, const char * argMemory, const size_t argSize)
{
	Create(argContext, argMemory, argSize);
}

Utility::HullShader::HullShader(ID3D11DeviceContext * argContext, const char * argFilename)
{
	Shader::Create(argContext, argFilename);
}

void Utility::HullShader::SetShader(ID3D11DeviceContext * argContext)
{
	argContext->HSSetShader(shader_.Get(), nullptr, 0);
}

HRESULT Utility::HullShader::Create(ID3D11DeviceContext * argContext, const char * argMemory, const size_t argSize)
{
	ID3D11Device* device = Utility::GetD3Ddevice(argContext).Get();
	HRESULT hr = device->CreateHullShader(argMemory, argSize, nullptr, &shader_);
	if (FAILED(hr))
	{
		OutputDebugString(L"Create Vertex Shader Failed...\n");
		return hr;
	}
	return hr;
}
