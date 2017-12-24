
#include "ComputeShader.h"
#include "../../DeviceResources/DeviceResources.h"

Utility::ComputeShader::ComputeShader(ID3D11DeviceContext * argContext, const char * argMemory, const size_t argSize)
{
	Create(argContext, argMemory, argSize);
}

Utility::ComputeShader::ComputeShader(ID3D11DeviceContext * argContext, const char * argFilename)
{
	Shader::Create(argContext, argFilename);
}

void Utility::ComputeShader::SetShader(ID3D11DeviceContext * argContext)
{
	argContext->CSSetShader(shader_.Get(), nullptr, 0);
}

HRESULT Utility::ComputeShader::Create(ID3D11DeviceContext * argContext, const char * argMemory, const size_t argSize)
{
	ID3D11Device* device = Utility::GetD3Ddevice(argContext).Get();
	HRESULT hr = device->CreateComputeShader(argMemory, argSize, nullptr, &shader_);
	if (FAILED(hr))
	{
		OutputDebugString(L"Create Vertex Shader Failed...\n");
		return hr;
	}
	return hr;
}
