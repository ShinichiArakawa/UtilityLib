/**
 *	@file	CFade.h
 *	@date	2017 / 10 / 16
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <memory>
#include "../Math/Math.h"
#include "../GraphicManager/GraphManager.h"

struct ID3D11DeviceContext;

namespace Utility
{
	template<typename T>
	class ConstantBuffer;

	/**
	 *	@enum	eFadeType
	 *	@brief	フェードの種類
	 */
	enum class eFadeType
	{
		In,
		Out,
	};

	class CFade abstract
	{
	protected:
		bool shouldFade_;
		eFadeType	fadeType_;
		const int VertexMax = 4;
		std::string		csoPath_;
		BlendStateManager*				blendStateManager_;
		std::unique_ptr<ConstantBuffer<TextureMatrix>>	cbTextureMatrix_;
		VertexShader vertexShader_;
		PixelShader pixelShader_;
		std::shared_ptr<Texture> srcTex_;
		VertexBuffer<TextureVertex> vertices_;
		InputLayout inputLayout_;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rt_;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> ds_;
	public:
		/**
		 *	@fn			Reset
		 *	@brief		再初期化
		 *	@param[in]	argFadeType	!<	フェードの種類
		 */	
		virtual void Reset(eFadeType argFadeType) = 0;
		/**
		 *	@fn			Update
		 *	@brief		更新
		 *	@retval		true	!<	更新中
		 *	@retval		false	!<	更新終了
		 */	
		virtual bool Update() = 0;
		/**
		 *	@fn			CrossFade
		 *	@brief		フェードインとアウト両方行う
		 *	@retval		true	!<	フェードインアウト中
		 *	@retval		false	!<	フェードインアウト終了
		 */	
		virtual bool CrossFade() = 0;
		/**
		 *	@fn			Draw
		 *	@brief		描画
		 *	@param[in]	argContext	!<	コンテキスト
		 */	
		virtual void Draw(ID3D11DeviceContext * argContext) = 0;

		inline eFadeType CurrentFadeType()const { return fadeType_; }
	public:

		/**
		 *  @constructor	CFade
		 *  @brief			フェードの抽象クラス
		 *  @param[in]		argContext		!<	コンテキスト
		 *  @param[in]		argWindowSize	!<	ウィンドウサイズ
		 *  @param[in]		argFadeType		!<	フェードの種類
		 *  @param[in]		argTexture		!<	フェードに使うテクスチャ
		 */
		CFade(ID3D11DeviceContext *argContext, Math::Vector2 argWindowSize, eFadeType argFadeType, std::shared_ptr<Texture> argTexture);
		virtual ~CFade();
	};
}