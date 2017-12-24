/**
 *	@file	ViewportManager.h
 *	@date	2017 / 10 / 05
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <map>
#include <string>

struct ID3D11DeviceContext;
struct D3D11_VIEWPORT;

namespace Utility
{
	class ViewportManager
	{
	private:
		std::map<std::string, D3D11_VIEWPORT>	viewports_;
		std::string								nowViewName_;
	private:
		void Create(DirectX::XMFLOAT2 argWindowPos, DirectX::XMFLOAT2 argWindowSize, DirectX::XMFLOAT2 argWindowDepth, std::string argViewportName);
	
	public:
		ViewportManager::ViewportManager();
		ViewportManager::~ViewportManager();
	public:

		void CreateViewport(DirectX::XMFLOAT2 argWindowPos, DirectX::XMFLOAT2 argWindowSize, DirectX::XMFLOAT2 argWindowDepth, std::string argViewportName);
		void CreateViewport(DirectX::XMFLOAT2 argWindowPos, DirectX::XMFLOAT2 argWindowSize, std::string argViewportName);
		void CreateViewport(DirectX::XMFLOAT2 argWindowPos, DirectX::XMFLOAT2 argWindowSize);
		void CreateViewport(D3D11_VIEWPORT argViewport, std::string argViewportName);
		void CreateViewport(D3D11_VIEWPORT argViewport);

		D3D11_VIEWPORT GetViewport(std::string argViewportName)const;
		D3D11_VIEWPORT GetNowViewport(ID3D11DeviceContext *argContext)const;
		void RSSetViewports(ID3D11DeviceContext *argContext, unsigned int argSetViewportsNum, D3D11_VIEWPORT *argViewports);
		void RSSetViewports(ID3D11DeviceContext *argContext, unsigned int argSetViewportsNum, std::string argSetViewName);
	};
}
