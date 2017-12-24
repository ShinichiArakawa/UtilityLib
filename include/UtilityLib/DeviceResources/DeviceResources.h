/**
 *	@file	DeviceResources.h
 *	@date	2017 / 7 / 15
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <memory>
#include <d3d11.h>
#include <d3d11_1.h>
#include <wrl/client.h>
#include <assert.h>

namespace Utility
{
	/**
	 *  @class	DeferredContext
	 *  @brief	サブスレッド用遅延コンテキスト
	 */
	class DeferredContext
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context_;
		Microsoft::WRL::ComPtr<ID3D11CommandList> commandList_;

	public:

		/**
		 *  @constructor	DeferredContext
		 *  @brief			サブスレッド用遅延コンテキスト
		 *  @param[in]		argDevice	!<	デバイス
		 */
		DeferredContext(ID3D11Device *argDevice)
		{
			assert(argDevice);
			Create(argDevice);
		}
		DeferredContext() = default;
		~DeferredContext() = default;
	public:
		/**
		 *  @fn				Create
		 *  @brief			生成
		 *  @param[in]		argDevice	!<	デバイス
		 *  @retval			S_OK		!<	成功
		 *  @retval			!S_OK		!<	失敗
		 */
		inline HRESULT Create(ID3D11Device *argDevice)
		{
			assert(argDevice);
			return argDevice->CreateDeferredContext(false, &context_);
		}
		/**
		 *  @fn			Finish
		 *  @brief		描画コマンドの登録終了
		 */
		inline void Finish()
		{
			assert(context_.Get());
			context_->FinishCommandList(false, &commandList_);
		}
		inline Microsoft::WRL::ComPtr<ID3D11DeviceContext> Context()const { return context_; }
		inline Microsoft::WRL::ComPtr<ID3D11CommandList> CommandList()const { return commandList_; }

	public:
		ID3D11DeviceContext *Get()const
		{
			return context_.Get();
		}
		/**
		 *  @fn			operator&
		 *  @brief		読み取り用operator
		 *  @return		コンテキスト
		 */
		inline const ID3D11DeviceContext* const* operator&() const
		{
			assert(context_.GetAddressOf());
			return context_.GetAddressOf();
		}
		/**
		 *  @fn			operator&
		 *  @brief		書き込み用operator
		 *  @return		コンテキスト
		 */
		inline ID3D11DeviceContext ** operator&()
		{
			assert(context_.GetAddressOf());
			return context_.GetAddressOf();
		}
		/**
		 *  @fn			operato->
		 *  @brief		読み取り用operator
		 *  @return		コンテキストのポインタ
		 */
		inline ID3D11DeviceContext* operator->() const
		{
			assert(context_.Get());
			return context_.Get();
		}

	};
	/**
	 *  @class	ImmediateContext
	 *  @brief	メインスレッド用即時コンテキスト
	 */
	class ImmediateContext
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context_;

	public:
		ImmediateContext() = default;
		~ImmediateContext() = default;

	public:
		/**
		 *  @fn			Execute
		 *  @brief		遅延レンダリングの実行
		 *  @param[in]	argCommandList	!<	実行したい遅延レンダリング
		 */
		inline void Execute(ID3D11CommandList *argCommandList)
		{
			assert(context_.Get());
			context_->ExecuteCommandList(argCommandList, false);
		}
		inline Microsoft::WRL::ComPtr<ID3D11DeviceContext> Context()const { return context_; }
	public:
		ID3D11DeviceContext *Get()const
		{
			return context_.Get();
		}
		/**
		 *  @fn			operator&
		 *  @brief		読み取り用operator
		 *  @return		コンテキスト
		 */
		inline const ID3D11DeviceContext* const* operator&() const
		{
			assert(context_.GetAddressOf());
			return context_.GetAddressOf();
		}
		/**
		 *  @fn			operator&
		 *  @brief		書き込み用operator
		 *  @return		コンテキスト
		 */
		inline ID3D11DeviceContext ** operator&()
		{
			assert(context_.GetAddressOf());
			return context_.GetAddressOf();
		}
		/**
		 *  @fn			operator->
		 *  @brief		読み取り用operator
		 *  @return		コンテキスト
		 */
		inline ID3D11DeviceContext* operator->() const
		{
			assert(context_.Get());
			return context_.Get();
		}
	};

	class DeviceResources final
	{
	private:
		class Impl;
		std::unique_ptr<Impl>pImpl;

	public:
		/**
		 *  @constructor	DeviceResources
		 *  @brief			デバイスリソース
		 *  @param[in]		arghWnd	!<	ウィンドウハンドル
		 */
		DeviceResources(HWND arghWnd);
		DeviceResources();
		~DeviceResources();

		void Initialize(HWND arghWnd);
		void ClearWindow(const float *argColor);

		ID3D11Device *Device()const;
		ImmediateContext ImContext()const;
		DeferredContext DeContext()const;
		IDXGISwapChain *SwapChain()const;
		ID3D11RenderTargetView *RenderTargetView()const;
		ID3D11Texture2D *DepthStencil()const;
		ID3D11DepthStencilView *DepthStencilView()const;
	};

	/**
	 *  @fn			GetD3Ddevice
	 *  @brief		デバイスの取得
	 *  @param[in]	argContext	!<	コンテキスト
	 *  @return		デバイス
	 */
	inline Microsoft::WRL::ComPtr<ID3D11Device> GetD3Ddevice(ID3D11DeviceContext *argContext)
	{
		Microsoft::WRL::ComPtr<ID3D11Device> device;
		argContext->GetDevice(&device);
		return device;
	}
};