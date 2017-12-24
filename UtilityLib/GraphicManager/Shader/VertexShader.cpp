/**
 *	@file	VertexShader.cpp
 *	@date	2017 / 12 / 27
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "VertexShader.h"
#include "InputLayout.h"
#include "../../DeviceResources/DeviceResources.h"

Utility::VertexShader::VertexShader(ID3D11DeviceContext * argContext, ID3D11InputLayout ** argInputLayout, const char * argFilename)
{
	create_ = [=](ID3D11DeviceContext * argContext, const char * argMemory, const size_t argSize) { return Create(argContext, argMemory, argSize, argInputLayout); };
	Shader::Create(argContext, argFilename);
}

Utility::VertexShader::VertexShader(ID3D11DeviceContext * argContext, ID3D11InputLayout ** argInputLayout, const char * argMemory, const size_t argSize)
{
	Create(argContext, argMemory, argSize, argInputLayout);
}

Utility::VertexShader::VertexShader(ID3D11DeviceContext * argContext, const char * argMemory, const size_t argSize)
{
	Create(argContext, argMemory, argSize);
}

Utility::VertexShader::VertexShader(ID3D11DeviceContext * argContext, const char * argFilename)
{
	Shader::Create(argContext, argFilename);
}

void Utility::VertexShader::SetShader(ID3D11DeviceContext * argContext)
{
	argContext->VSSetShader(shader_.Get(), nullptr, 0);
}

HRESULT Utility::VertexShader::Create(ID3D11DeviceContext * argContext, const char * argMemory, const size_t argSize)
{
	ID3D11Device* device = Utility::GetD3Ddevice(argContext).Get();
	HRESULT hr = device->CreateVertexShader(argMemory, argSize, nullptr, &shader_);
	if (FAILED(hr))
	{
		OutputDebugString(L"Create Vertex Shader Failed...\n");
		return hr;
	}
	return hr;
}

HRESULT Utility::VertexShader::Create(ID3D11DeviceContext * argContext, const char * argMemory, const size_t argSize, ID3D11InputLayout ** argInputLayout)
{
	HRESULT hr = Create(argContext, argMemory, argSize);
	if (FAILED(hr))
	{
		return hr;
	}

	ID3D11Device* device = Utility::GetD3Ddevice(argContext).Get();
	hr = InputLayout::Create(device, argInputLayout, argMemory, argSize);
	if (FAILED(hr))
	{
		OutputDebugString(L"Create Input Layout Failed...\n");
		return hr;
	}

	return hr;
}
