/**
 *	@file	DeviceResources.cpp
 *	@date	2017 / 7 / 15
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "DeviceResources.h"
#include "../Function.h"
#include <sstream>

#pragma comment(lib, "d3d11.lib")

class Utility::DeviceResources::Impl
{
public:
	Impl(HWND hWnd)
	{
		if (FAILED(InitDevice(hWnd)))
		{
			Utility::Error::AssertMessageRelease(L"Deviceの初期化に失敗しました");
			exit(EXIT_FAILURE);
		}
	}
	~Impl() = default;

private:
	/**
	 *	@fn			InitDevice
	 *	@brief		デバイスの初期化
	 *	@param[in]	hWnd	ウィンドウのハンドル
	 *	@return		初期化が成功したか : S_OK << 成功 S_FALSE << 失敗
	 */
	HRESULT InitDevice(HWND hWnd)
	{
		HRESULT hr = S_OK;
		bool isFullScreen = false;

#if defined(DEBUG) || defined(_DEBUG)
		isFullScreen = false;
#else
		if (MessageBox(nullptr, L"フルスクリーンにしますか？", L"スクリーンサイズ設定", MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			isFullScreen = true;
		}
#endif
		D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_NULL;
		featureLevel_ = D3D_FEATURE_LEVEL_11_0;

		// クライアント領域の取得
		RECT rc;
		GetClientRect(hWnd, &rc);
		unsigned int width, height;
		width = rc.right - rc.left;
		height = rc.bottom - rc.top;

		unsigned int createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(DEBUG) || defined(_DEBUG)
		//	@enum		D3D11_CREATE_DEVICE_DEBUG
		//	@brief		デバックレイヤーをサポートするフラグ
		// 
		// デザインツールをダウンロードしないとコンパイルできなくなるので注意<br>
		// 
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};
		unsigned int numDriverTypes = ARRAYSIZE(driverTypes);

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};
		unsigned int numFeatureLevels = ARRAYSIZE(featureLevels);

		// スワップチェーンの設定
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 0;	// <--リフレッシュレートはDXGIの自動設定に任せる
		sd.BufferDesc.RefreshRate.Denominator = 0;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = !isFullScreen;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // <--フルスクリーンへの切り替えの時に表示モードを変更する

		//	デバイスとスワップチェーンの作成
		for (unsigned int driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
		{
			driverType = driverTypes[driverTypeIndex];
			hr = D3D11CreateDeviceAndSwapChain(nullptr, driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
				D3D11_SDK_VERSION, &sd, &swapChain_, &device_, &featureLevel_, &immediateContext_);
			if (SUCCEEDED(hr))
				break;
		}
		if (FAILED(hr))
			return hr;

		hr = deferredContext_.Create(device_.Get());
		if (FAILED(hr))
			return hr;

		//	Atl+Enterで画面モードを変更しないようにする
		Microsoft::WRL::ComPtr<IDXGIDevice1> pDXGIDevice;
		Microsoft::WRL::ComPtr<IDXGIAdapter> pDXGIAdapter;
		Microsoft::WRL::ComPtr<IDXGIFactory> pDXGIFactory;

		hr = device_.As(&pDXGIDevice);
		if (FAILED(hr))
			return hr;
		hr = pDXGIDevice->GetAdapter(&pDXGIAdapter);
		if (FAILED(hr)) 
			return hr;
		hr = pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), static_cast<LPVOID*>(&pDXGIFactory));
		if (FAILED(hr)) 
			return hr;

		// 画面モードの切り替え機能の設定
		hr = pDXGIFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_WINDOW_CHANGES);
		if (FAILED(hr)) 
			return hr;

		//	バックバッファの作成
		hr = InitBackBuffer();
		if (FAILED(hr))
			return hr;

		//	ビューポート作成
		CD3D11_VIEWPORT vp(0.0f, 0.0f, static_cast<FLOAT>(width), static_cast<FLOAT>(height));
		immediateContext_->RSSetViewports(1, &vp);

		return S_OK;

	}

	/**
	 *	@fn		InitBackBuffer
	 *	@brief	バッファの初期化
	 *  @return 初期化が成功したか : S_OK << 成功 S_FALSE << 失敗
	 */
	HRESULT InitBackBuffer()
	{
		HRESULT hr = S_OK;

		//	バックバッファの作成
		//	スワップチェーンからバックバッファを取得
		ID3D11Texture2D* pBackBuffer = nullptr;
		hr = swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&pBackBuffer));
		if (FAILED(hr))
			return hr;

		//	バックバッファに指定されているテクスチャ情報を取得
		D3D11_TEXTURE2D_DESC descBackBuffer;
		pBackBuffer->GetDesc(&descBackBuffer);

		//	レンダーターゲットビューを作成
		hr = device_->CreateRenderTargetView(pBackBuffer, nullptr, renderTargetView_.GetAddressOf());
		pBackBuffer->Release();
		if (FAILED(hr))
			return hr;

		//	深度バッファの作成
		//	深度ステンシルテクスチャの作成
		CD3D11_TEXTURE2D_DESC descDepth(descBackBuffer);	// <--バックバッファに指定されているテクスチャ情報を元に設定する
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		hr = device_->CreateTexture2D(&descDepth, nullptr, &depthStencil_);
		if (FAILED(hr))
			return hr;

		//	深度ステンシルビューの作成
		CD3D11_DEPTH_STENCIL_VIEW_DESC descDSV(D3D11_DSV_DIMENSION_TEXTURE2D, descDepth.Format);
		hr = device_->CreateDepthStencilView(depthStencil_.Get(), &descDSV, &depthStencilView_);
		if (FAILED(hr))
			return hr;

		//	レンダーターゲット設定
		immediateContext_->OMSetRenderTargets(1, renderTargetView_.GetAddressOf(), depthStencilView_.Get());

		return S_OK;
	}
public:

	void ClearWindow(const float *argColor)
	{
		const auto Depth = 1.f;
		const unsigned int Stencil = 0;

		immediateContext_->ClearRenderTargetView(renderTargetView_.Get(), argColor);
		immediateContext_->ClearDepthStencilView(depthStencilView_.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, Depth, Stencil);

	}

public:

	D3D_FEATURE_LEVEL								featureLevel_;
	Microsoft::WRL::ComPtr<ID3D11Device>			device_;
	ImmediateContext								immediateContext_;
	DeferredContext									deferredContext_;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			swapChain_;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	renderTargetView_;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			depthStencil_;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	depthStencilView_;

};

Utility::DeviceResources::DeviceResources() = default;

Utility::DeviceResources::DeviceResources(HWND hWnd)
	:pImpl(std::make_unique<Impl>(hWnd))
{
}

Utility::DeviceResources::~DeviceResources() = default;

void Utility::DeviceResources::Initialize(HWND arghWnd)
{
	pImpl = std::make_unique<Impl>(arghWnd);
}

void Utility::DeviceResources::ClearWindow(const float *argColor)
{
	pImpl->ClearWindow(argColor);
}

ID3D11Device *Utility::DeviceResources::Device() const
{
	return pImpl->device_.Get();
}

Utility::ImmediateContext Utility::DeviceResources::ImContext() const
{
	return pImpl->immediateContext_;
}

Utility::DeferredContext Utility::DeviceResources::DeContext() const
{
	return pImpl->deferredContext_;
}

IDXGISwapChain *Utility::DeviceResources::SwapChain() const
{
	return pImpl->swapChain_.Get();
}

ID3D11RenderTargetView *Utility::DeviceResources::RenderTargetView() const
{
	return pImpl->renderTargetView_.Get();
}

ID3D11Texture2D *Utility::DeviceResources::DepthStencil() const
{
	return pImpl->depthStencil_.Get();
}

ID3D11DepthStencilView *Utility::DeviceResources::DepthStencilView() const
{
	return pImpl->depthStencilView_.Get();
}
