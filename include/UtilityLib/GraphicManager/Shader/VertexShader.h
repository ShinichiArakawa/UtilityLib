/**
 *	@file	VertexShader.h
 *	@date	2017 / 12 / 27
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "Shader.h"

namespace Utility
{
	class VertexShader final : public Shader <ID3D11VertexShader>
	{
	public:
		VertexShader(ID3D11DeviceContext *argContext, ID3D11InputLayout ** argInputLayout, const char* argFilename);
		VertexShader(ID3D11DeviceContext *argContext, ID3D11InputLayout ** argInputLayout, const char* argMemory, const size_t argSize);
		VertexShader(ID3D11DeviceContext *argContext, const char* argMemory, const size_t argSize);
		VertexShader(ID3D11DeviceContext *argContext, const char* argFilename);
		VertexShader() = default;
		~VertexShader() = default;

	public:

		virtual void SetShader(ID3D11DeviceContext *argContext) override;
		virtual HRESULT Create(ID3D11DeviceContext *argContext, const char * argMemory, const size_t argSize) override;
		HRESULT Create(ID3D11DeviceContext *argContext, const char * argMemory, const size_t argSize, ID3D11InputLayout **argInputLayout);

	};
}