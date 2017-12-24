/**
 *	@file	ViewportManager.h
 *	@date	2017 / 10 / 05
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "ViewportManager.h"

void Utility::ViewportManager::Create(DirectX::XMFLOAT2 argWindowPos, DirectX::XMFLOAT2 argWindowSize, DirectX::XMFLOAT2 argWindowDepth, std::string argViewportName)
{
	//	@note	ほぼ、CD3D11_VIEWPORTと同じ
	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = static_cast<FLOAT>(argWindowPos.x);
	viewport.TopLeftY = static_cast<FLOAT>(argWindowPos.y);
	viewport.Width = static_cast<FLOAT>(argWindowSize.x);
	viewport.Height = static_cast<FLOAT>(argWindowSize.y);
	viewport.MinDepth = argWindowDepth.x;
	viewport.MaxDepth = argWindowDepth.y;
	viewports_.insert(std::make_pair(argViewportName, viewport));
}

Utility::ViewportManager::ViewportManager() = default;
Utility::ViewportManager::~ViewportManager() = default;

void Utility::ViewportManager::CreateViewport(DirectX::XMFLOAT2 argWindowPos, DirectX::XMFLOAT2 argWindowSize, DirectX::XMFLOAT2 argWindowDepth, std::string argViewportName)
{
	Create(argWindowPos, argWindowSize, argWindowDepth, argViewportName);
}

void Utility::ViewportManager::CreateViewport(DirectX::XMFLOAT2 argWindowPos, DirectX::XMFLOAT2 argWindowSize, std::string argViewportName)
{
	Create(argWindowPos, argWindowSize, DirectX::XMFLOAT2(0.f, 1.f), argViewportName);
}

void Utility::ViewportManager::CreateViewport(DirectX::XMFLOAT2 argWindowPos, DirectX::XMFLOAT2 argWindowSize)
{
	Create(argWindowPos, argWindowSize, DirectX::XMFLOAT2(0.f, 1.f), "Default View");
}

void Utility::ViewportManager::CreateViewport(D3D11_VIEWPORT argViewport, std::string argViewportName)
{
	viewports_.insert(std::make_pair(argViewportName, argViewport));
}

void Utility::ViewportManager::CreateViewport(D3D11_VIEWPORT argViewport)
{
	viewports_.insert(std::make_pair("Default View", argViewport));
}

D3D11_VIEWPORT Utility::ViewportManager::GetViewport(std::string argViewportName) const
{
	return viewports_.at(argViewportName);
}

D3D11_VIEWPORT Utility::ViewportManager::GetNowViewport(ID3D11DeviceContext *argContext) const
{
	D3D11_VIEWPORT viewport;
	unsigned int num = 1;
	argContext->RSGetViewports(&num, &viewport);

	return viewport;
}

void Utility::ViewportManager::RSSetViewports(ID3D11DeviceContext *argContext, unsigned int argSetViewportsNum, D3D11_VIEWPORT *argViewports)
{
	argContext->RSSetViewports(argSetViewportsNum, argViewports);
}

void Utility::ViewportManager::RSSetViewports(ID3D11DeviceContext * argContext, unsigned int argSetViewportsNum, std::string argSetViewName)
{
	D3D11_VIEWPORT *viewport = &viewports_.at(argSetViewName);
	assert(viewport && "arg viewport don't found...");
	argContext->RSSetViewports(argSetViewportsNum, viewport);
}
