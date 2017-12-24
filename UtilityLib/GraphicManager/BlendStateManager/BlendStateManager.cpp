/**
 *	@file	BlendStateManager.cpp
 *	@date	2017 / 10 / 05
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "BlendStateManager.h"
#include "../../DeviceResources/DeviceResources.h"

#include <d3d11.h>
#include <assert.h>

Utility::BlendStateManager::BlendStateManager()
	:isOnce_(false)
{
}

Utility::BlendStateManager::BlendStateManager(ID3D11DeviceContext *argContext)
	:isOnce_(false)
{
	Initialize(argContext);
}

Utility::BlendStateManager::~BlendStateManager() = default;

void Utility::BlendStateManager::Initialize(ID3D11DeviceContext * argContext)
{
	if (isOnce_)
		return;

	isOnce_ = true;
	auto device = GetD3Ddevice(argContext);
	//	デフォルト
	argContext->OMGetBlendState(&stateList_[eBlendMode::Default], blendFactor_, &sampleMask_);

	D3D11_BLEND_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.AlphaToCoverageEnable = false;
	bd.IndependentBlendEnable = false;
	bd.RenderTarget[0].BlendEnable = true;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	//	半透明合成
	HRESULT hr = device->CreateBlendState(&bd, &stateList_[eBlendMode::NonPremultiplied]);
	assert(SUCCEEDED(hr) && "Create NonPremultiplied BlendState Failed...");

	//	加算合成
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	hr = device->CreateBlendState(&bd, &stateList_[eBlendMode::Additive]);
	assert(SUCCEEDED(hr) && "Create Additive BlendState Failed...");
}

void Utility::BlendStateManager::ChangeState(ID3D11DeviceContext * argContext, unsigned int argIndex, float *argBlendFactor)
{
	float *blendFactor = (argBlendFactor) ? argBlendFactor : blendFactor_;
	argContext->OMSetBlendState(stateList_[argIndex].Get(), blendFactor, sampleMask_);
}
