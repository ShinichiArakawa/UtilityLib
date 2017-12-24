/**
 *	@file	RenderTarget.h
 *	@date	2017 / 11 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <map>
#include <string>


namespace Utility
{
	class RenderTarget
	{
	private:
		std::map<std::string,  
			Microsoft::WRL::ComPtr<ID3D11RenderTargetView>>	rts_;
		std::string											nowViewName_;
	public:
		RenderTarget();
		~RenderTarget();


	};

}