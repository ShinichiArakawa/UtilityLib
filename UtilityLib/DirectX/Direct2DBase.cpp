/**
*	@file   Direct2DBase.cpp
*	@date   2017 / 07 / 15
*	@author Katsumi Takei
*	Copyright (c) Kastumi Takei. All rights reserved.
*/
#include "Direct2DBase.h"
#include <wrl/client.h>

#pragma comment(lib,"D2d1.lib")

class Utility::Direct2DBase::Impl
{
public:
	// https://msdn.microsoft.com/ja-jp/library/windows/desktop/dd535473(v=vs.85).aspx
	Impl(IDXGISwapChain * argSwapChain)
	{
		// ファクトリの作成
		D2D1_FACTORY_OPTIONS options = {};
#if defined(DEBUG) || defined(_DEBUG)
		options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#else
		options.debugLevel = D2D1_DEBUG_LEVEL_NONE;
#endif
		D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, options, factory.GetAddressOf());

		CreateRanderTarget(argSwapChain, eRtDepth::Front);
		CreateRanderTarget(argSwapChain, eRtDepth::Back);
	}
	~Impl() = default;
public:

	void ReleaseRenderTarget(eRtDepth argType)
	{
		renderTarget_[static_cast<int>(argType)].Reset();
	}

	bool CreateRanderTarget(IDXGISwapChain* argSwapChain, eRtDepth argType)
	{
		// レンダーターゲットの作成
		Microsoft::WRL::ComPtr<IDXGISurface> surface;

		HRESULT hr = argSwapChain->GetBuffer(0, IID_PPV_ARGS(surface.GetAddressOf()));

		if (FAILED(hr))
			return false;

		const FLOAT dpiSize = 96.f;

		D2D1_RENDER_TARGET_PROPERTIES rtp = D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
			dpiSize,
			dpiSize);

		hr = factory->CreateDxgiSurfaceRenderTarget(surface.Get(), &rtp, renderTarget_[static_cast<int>(argType)].GetAddressOf());

		if (FAILED(hr)) 
			return false;

		return true;
	}

	void Draw(eRtDepth argType)
	{
		renderTarget_[static_cast<int>(argType)]->BeginDraw();

		renderTarget_[static_cast<int>(argType)]->EndDraw();
	}

public:
	Microsoft::WRL::ComPtr<ID2D1Factory> factory;
	Microsoft::WRL::ComPtr<ID2D1RenderTarget> renderTarget_[static_cast<int>(eRtDepth::Max)];
	Microsoft::WRL::ComPtr<ID2D1RenderTarget> renderTargetBack_;

};

Utility::Direct2DBase::Direct2DBase(IDXGISwapChain * argSwapChain)
	: pImpl(std::make_unique<Impl>(argSwapChain))
{
}

Utility::Direct2DBase::~Direct2DBase() = default;

void Utility::Direct2DBase::ReleaseRenderTarget(eRtDepth argType)
{
	pImpl->ReleaseRenderTarget(argType);
}

bool Utility::Direct2DBase::CreateRanderTarget(IDXGISwapChain * argSwapChain, eRtDepth argType)
{
	return pImpl->CreateRanderTarget(argSwapChain, argType);
}

ID2D1RenderTarget *Utility::Direct2DBase::RenderTarget(eRtDepth argType)
{
	return pImpl->renderTarget_[static_cast<int>(argType)].Get();
}

ID2D1Factory *Utility::Direct2DBase::ImageFactory()
{
	return pImpl->factory.Get();
}

