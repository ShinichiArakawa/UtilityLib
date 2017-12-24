/**
 *	@file   Direct2DBase.h
 *	@date   2017 / 07 / 15
 *	@author Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <memory>
#include <d2d1.h>

namespace Utility
{
	enum class eRtDepth
	{
		Front,
		Back,
		Max,
	};

	class Direct2DBase
	{
	private:
		class Impl;
		std::unique_ptr<Impl> pImpl;
	public:
		Direct2DBase(IDXGISwapChain * argSwapChain);
		~Direct2DBase();

	public:
		/**
		 *	@fn			ReleaseRenderTarget
		 *	@brief		レンダーターゲットの解放
		 */
		void ReleaseRenderTarget(eRtDepth argType);
		/**
		 *	@fn			CreateRanderTarget
		 *	@brief		レンダーターゲットの作成
		 *	@param[in]	argSwapChain	!<	スワップチェインのポインタ
		 *	@note		https://msdn.microsoft.com/query/dev14.query?appId=Dev14IDEF1&l=JA-JP&k=k(d2d1%2FID2D1Factory%3A%3ACreateDxgiSurfaceRenderTarget);k(ID2D1Factory%3A%3ACreateDxgiSurfaceRenderTarget);k(CreateDxgiSurfaceRenderTarget);k(DevLang-C%2B%2B);k(TargetOS-Windows)&rd=true
		 */
		bool CreateRanderTarget(IDXGISwapChain* argSwapChain, eRtDepth argType);
		/**
		 *	@fn			RanderTarget
		 *	@brief		レンダーターゲットのポインタの取得
		 *	@return		ID2D1RenderTargetのポインタ
		 */
		ID2D1RenderTarget* RenderTarget(eRtDepth argType);
		/**
		 *	@fn			ImageFactory
		 *	@brief		ID2D1Factoryのポインタの取得
		 *	@return		ID2D1Factoryのポインタ
		 */
		ID2D1Factory* ImageFactory();

	};
};