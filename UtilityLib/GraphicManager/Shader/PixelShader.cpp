
#include "PixelShader.h"
#include "../../DeviceResources/DeviceResources.h"

Utility::PixelShader::PixelShader(ID3D11DeviceContext * argContext, const char * argMemory, const size_t argSize)
{
	Create(argContext, argMemory, argSize);
}

Utility::PixelShader::PixelShader(ID3D11DeviceContext * argContext, const char * argFilename)
{
	Shader::Create(argContext, argFilename);
}

void Utility::PixelShader::SetShader(ID3D11DeviceContext * argContext)
{
	argContext->PSSetShader(shader_.Get(), nullptr, 0);
}

HRESULT Utility::PixelShader::Create(ID3D11DeviceContext * argContext, const char * argMemory, const size_t argSize)
{
	ID3D11Device* device = Utility::GetD3Ddevice(argContext).Get();
	HRESULT hr = device->CreatePixelShader(argMemory, argSize, nullptr, &shader_);
	if (FAILED(hr))
	{
		OutputDebugString(L"Create Vertex Shader Failed...\n");
		return hr;
	}
	return hr;
}
