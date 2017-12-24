/**
*	@file	Shader.h
*	@date	2017 / 8 / 27
*	@author	Katsumi Takei
*	Copyright (c) Kastumi Takei. All rights reserved.
*/
#pragma once

#include "Shader.h"

namespace Utility
{
	class ComputeShader final : public Shader<ID3D11ComputeShader>
	{
	public:
		ComputeShader(ID3D11DeviceContext *argContext, const char* argMemory, const size_t argSize);
		ComputeShader(ID3D11DeviceContext *argContext, const char* argFilename);
		ComputeShader() = default;
		~ComputeShader() = default;

	public:

		virtual void SetShader(ID3D11DeviceContext *argContext) override;
		virtual HRESULT Create(ID3D11DeviceContext *argContext, const char * argMemory, const size_t argSize) override;

	};
}