/**
 *	@file	RasterizerStateManager.cpp
 *	@date	2017 / 10 / 05
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "RasterizerStateManager.h"
#include "../../DeviceResources/DeviceResources.h"

#include <d3d11.h>
#include <assert.h>

Utility::RasterizerStateManager::RasterizerStateManager()
	:isOnce_(false)
{
}

Utility::RasterizerStateManager::RasterizerStateManager(ID3D11DeviceContext *argContext)
	: isOnce_(false)
{
	Initialize(argContext);
};

Utility::RasterizerStateManager::~RasterizerStateManager() = default;

void Utility::RasterizerStateManager::Initialize(ID3D11DeviceContext *argContext)
{
	if (isOnce_)
		return;

	isOnce_ = true;
	auto device = GetD3Ddevice(argContext);
	argContext->RSGetState(&stateList_[static_cast<int>(eShading::Default)]);

	D3D11_RASTERIZER_DESC desc = {};
	desc.CullMode = D3D11_CULL_BACK;
	desc.FillMode = D3D11_FILL_WIREFRAME;
	desc.FrontCounterClockwise = false;
	desc.DepthBias = 0;
	desc.SlopeScaledDepthBias = 0.f;
	desc.DepthBiasClamp = 0.f;
	desc.DepthClipEnable = true;
	desc.ScissorEnable = false;
	desc.MultisampleEnable = false;
	desc.AntialiasedLineEnable = false;
	HRESULT hr = device->CreateRasterizerState(&desc, &stateList_[static_cast<int>(eShading::Wireframe)]);
	assert(SUCCEEDED(hr) && "Create Wireframe State Failed...");

	desc.FillMode = D3D11_FILL_SOLID;
	hr = device->CreateRasterizerState(&desc, &stateList_[static_cast<int>(eShading::Solid)]);
	assert(SUCCEEDED(hr) && "Create Solid State Failed...");

	desc.CullMode = D3D11_CULL_NONE;
	hr = device->CreateRasterizerState(&desc, &stateList_[static_cast<int>(eShading::Cullnone)]);
	assert(SUCCEEDED(hr) && "Create CullNone State Failed...");
}

void Utility::RasterizerStateManager::ChangeState(ID3D11DeviceContext *argContext, eShading argIndex)
{
	argContext->RSSetState(stateList_[static_cast<int>(argIndex)].Get());
}

void Utility::RasterizerStateManager::ChangeState(ID3D11DeviceContext * argContext, ID3D11RasterizerState * argRasterizerState)
{
	argContext->RSSetState(argRasterizerState);
}
