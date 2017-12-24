/**
 *	@file	BlendStateManager.h
 *	@date	2017 / 10 / 05
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <wrl/client.h>

struct ID3D11BlendState;
struct ID3D11DeviceContext;

namespace Utility
{
	/**
	 *	@enum	eBlendMode
	 *	@brief	画像合成の種類
	 */
	enum eBlendMode
	{
		Default,			//	!<	デフォルト(null)
		NonPremultiplied,	//	!<	通常描画
		Alpha,				//	!<	透過合成描画
		Additive,			//	!<	加算合成描画
		Subtractive,		//	!<	減算合成描画
		Multiplication,		//	!<	乗算合成描画
		Inverse,			//	!<	反転合成描画
		Max,
	};

	class BlendStateManager
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11BlendState> stateList_[eBlendMode::Max];
		float blendFactor_[4];
		unsigned int sampleMask_;
		bool isOnce_;
	public:
		/**
		 *  @constructor	BlendStateManager
		 *  @brief			ブレンドマネージャー
		 *  @param[in]		argContext	!<	コンテキスト
		 */
		BlendStateManager(ID3D11DeviceContext *argContext);
		BlendStateManager();
		~BlendStateManager();
	public:
		/**
		 *  @fn				Reset
		 *  @brief			初期化
		 *  @param[in]		argContext	!<	コンテキスト
		 */
		void Initialize(ID3D11DeviceContext *argContext);
		/**
		 *  @fn				GetBlendState
		 *  @brief			指定のブレンドステートを取得
		 *  @param[in]		argIndex	!<	インデックス
		 */
		inline Microsoft::WRL::ComPtr<ID3D11BlendState> GetBlendState(unsigned int argIndex) const { return stateList_[argIndex]; }
		/**
		 *  @fn				ChangeState
		 *  @brief			ブレンドステートの変更
		 *  @param[in]		argContext	!<	コンテキスト
		 *  @param[in]		argIndex	!<	インデックス
		 */
		void ChangeState(ID3D11DeviceContext *argContext, unsigned int argIndex, float *argBlendFactor = nullptr);

	};
}