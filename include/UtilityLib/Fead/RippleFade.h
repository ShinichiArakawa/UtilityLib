/**
 *	@file	RippleFade.h
 *	@date	2017 / 10 / 16
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <memory>
#include "CFade.h"

struct ID3D11DeviceContext;

namespace Utility
{
	class RippleFade : public CFade
	{
	private:
		class Impl;
		std::unique_ptr<Impl>pImpl;
	public:
		/**
		 *  @constructor	RippleFade
		 *  @brief			波紋フェード
		 *  @param[in]		argContext		!<	コンテキスト
		 *  @param[in]		argFadeType		!<	フェードの種類
		 *  @param[in]		argWindowSize	!<	ウィンドウサイズ
		 *  @param[in]		argTexture		!<	フェードに使うテクスチャ
		 */
		RippleFade(ID3D11DeviceContext *argContext, eFadeType argFadeType, Math::Vector2 argWindowSize, std::shared_ptr<Texture> argTexture);
		~RippleFade();

	public:
		/**
		 *	@fn			CrossFade
		 *	@brief		フェードインとアウト両方行う
		 *	@retval		true	!<	フェードインアウト中
		 *	@retval		false	!<	フェードインアウト終了
		 */	
		bool CrossFade() override;
		/**
		 *	@fn			Reset
		 *	@brief		再初期化
		 *	@param[in]	argFadeType	!<	フェードの種類
		 */	
		void Reset(eFadeType argFadeType) override;
		/**
		 *	@fn			Update
		 *	@brief		更新
		 *	@retval		true	!<	更新中
		 *	@retval		false	!<	更新終了
		 */	
		bool Update() override;
		/**
		 *	@fn			Draw
		 *	@brief		描画
		 *	@param[in]	argContext	!<	コンテキスト
		 */	
		void Draw(ID3D11DeviceContext *argContext) override;

	};
};