
#include "GeometryShader.h"
#include "../../DeviceResources/DeviceResources.h"

Utility::GeometryShader::GeometryShader(ID3D11DeviceContext * argContext, const char * argMemory, const size_t argSize)
{
	Create(argContext, argMemory, argSize);
}

Utility::GeometryShader::GeometryShader(ID3D11DeviceContext * argContext, const char * argFilename)
{
	Shader::Create(argContext, argFilename);
}

void Utility::GeometryShader::SetShader(ID3D11DeviceContext * argContext)
{
	argContext->GSSetShader(shader_.Get(), nullptr, 0);
}

HRESULT Utility::GeometryShader::Create(ID3D11DeviceContext * argContext, const char * argMemory, const size_t argSize)
{
	ID3D11Device* device = Utility::GetD3Ddevice(argContext).Get();
	HRESULT hr = device->CreateGeometryShader(argMemory, argSize, nullptr, &shader_);
	if (FAILED(hr))
	{
		OutputDebugString(L"Create Vertex Shader Failed...\n");
		return hr;
	}
	return hr;
}
