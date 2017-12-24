/**
 *	@file	RasterizerStateManager.h
 *	@date	2017 / 10 / 05
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <wrl/client.h>

struct ID3D11DeviceContext;
struct ID3D11RasterizerState;

namespace Utility
{
	enum class eShading
	{
		Default,
		Wireframe,
		Solid,
		Cullnone,

		Max,
	};
	class RasterizerStateManager
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> stateList_[static_cast<int>(eShading::Max)];
		bool isOnce_;

	public:
		/**
		 *  @constructor	RasterizerStateManager
		 *  @brief			ラスタライザマネージャー
		 *  @param[in]		argContext	!<	コンテキスト
		 */
		RasterizerStateManager(ID3D11DeviceContext *argContext);
		RasterizerStateManager();
		~RasterizerStateManager();
	public:
		/**
		 *  @fn				Reset
		 *  @brief			初期化
		 *  @param[in]		argContext	!<	コンテキスト
		 */
		void Initialize(ID3D11DeviceContext *argContext);
		/**
		 *  @fn				GetRasterizerState
		 *  @brief			指定のラスタライザステートを取得
		 *  @param[in]		argIndex	!<	インデックス
		 */
		inline Microsoft::WRL::ComPtr<ID3D11RasterizerState> GetRasterizerState(eShading argIndex) const { return stateList_[static_cast<int>(argIndex)]; }
		/**
		 *  @fn				ChangeState
		 *  @brief			ブレンドステートの変更
		 *  @param[in]		argContext	!<	コンテキスト
		 *  @param[in]		argIndex	!<	インデックス
		 */
		void ChangeState(ID3D11DeviceContext *argContext, eShading argIndex);
	public:
		/**
		 *  @fn				ChangeState
		 *  @brief			ブレンドステートの変更
		 *  @param[in]		argContex		t	!<	コンテキスト
		 *  @param[in]		argRasterizerState	!<	変更するステート
		 */
		static void ChangeState(ID3D11DeviceContext *argContext, ID3D11RasterizerState *argRasterizerState);

	};

}