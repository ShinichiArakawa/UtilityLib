/**
 *	@file   GraphManager.h
 *	@date   2017 / 08 / 31
 *	@author Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once
#include <memory>
#include <string>

#include "BufferObjectStructure.h"

#include "Graphic2DBase.h"
#include "Graphic3DBase.h"

struct ID3D11DeviceContext;
struct IDXGISwapChain;

namespace Utility
{
	class GraphManager final
	{
	private:
		std::unique_ptr<Graphic2DBase> graphic2d_;
		std::unique_ptr<Graphic3DBase> graphic3d_;

	public:
		GraphManager();
		~GraphManager();

	public:
		/**
		 *	@fn			Initialize
		 *	@brief		初期化
		 *	@param[in]	argContext		!<	コンテキスト
		 *	@param[in]	argSwapChain	!<	スワップチェイン
		 */
		void Initialize(ID3D11DeviceContext *argContext, IDXGISwapChain *argSwapChain);
		/**
		 *	@fn			DrawAndWriteText
		 *	@brief		DirectWriteでテキスト描画
		 *	@param[in]	argStr	!<	描画する文字列
		 *	@param[in]	argPos	!<	描画する座標
		 */
		void DrawAndWriteText(const std::wstring argStr, DirectX::XMFLOAT2 argPos)const;
		/**
		 *	@fn			ConfigTextColor
		 *	@brief		DirectWriteで描画するテキストの色を変える
		 *	@param[in]	argColor	!<	描画する文字列の色
		 */
		void ConfigTextColor(D2D1::ColorF argColor);
		/**
		 *	@fn			ConfigTextSize
		 *	@brief		DirectWriteで描画するテキストの大きさを変える
		 *	@param[in]	argStrSize	!<	描画する文字列の大きさ
		 */
		void ConfigTextSize(float argStrSize);
		/**
		 *	@fn			AddAndLoadImage
		 *	@brief		テクスチャのロード
		 *	@param[in]	argTextureFilename	!<	ロードするテクスチャのパス
		 *	@return		Graphic2DBaseを返してそこから登録形式を指定する
		 */
		Graphic2DBase *AddAndLoadImage(const std::wstring argTextureFilename);
		/**
		 *	@fn			AddAndLoadModel
		 *	@brief		モデルのロード
		 *	@param[in]	argModelFilename	!<	ロードするモデルのパス
		 *	@return		Graphic3DBaseを返してそこから登録形式を指定する
		 */
		Graphic3DBase *AddAndLoadModel(const std::string argModelFilename);
#pragma region	Accessor
	public:
		/**
		 *	@fn			FindIndexTexture
		 *	@brief		テクスチャの取得
		 *	@param[in]	argKey	!<	取得したいテクスチャのキー
		 *	@return		テクスチャ
		 */
		std::shared_ptr<Texture> FindIndexTexture(const std::wstring argKey)const;

		/**
		 *	@fn			DirectWrite
		 *	@brief		DirectWriteオブジェクトの取得
		 *	@return		DirectWriteオブジェクトの
		 */
		DWriteBase *DirectWrite()const;
		/**
		 *	@fn			RenderTarget
		 *	@brief		2D用RenderTargetの取得
		 *	@return		2D用RenderTarget
		 */
		ID2D1RenderTarget *RenderTarget()const;
		/**
		 *	@fn			ChangeRt2dDepth
		 *	@brief		設定したい2D用RenderTargetの指定
		 *	@param[in]	argType	!<	RenderTargetの指定
		 */
		void ChangeRt2dDepth(eRtDepth argType);
		/**
		 *	@fn			ChangeRt2dDepth
		 *	@brief		設定したい2D用RenderTargetの指定
		 *	@param[in]	argType	!<	RenderTargetの指定
		 */
		void ChangeRt2dDepth(int argType);
		/**
		 *	@fn			GetSpriteBatch
		 *	@brief		SpriteBatchオブジェクトの取得
		 *	@return		SpriteBatchオブジェクト
		 */
		SpriteBatch *GetSpriteBatch()const;
		/**
		 *	@fn			Graphic2D
		 *	@brief		2Dグラフィックスのマネージャー
		 *	@return		2Dグラフィックスのマネージャー
		 */
		Graphic2DBase *Graphic2D()const;
		/**
		 *	@fn			Graphic3D
		 *	@brief		3Dグラフィックスのマネージャー
		 *	@return		3Dグラフィックスのマネージャー
		 */
		Graphic3DBase *Graphic3D()const;
		/**
		 *	@fn			CanUseArchive
		 *	@brief		アーカイブの設定
		 *	@param[in]	argCanUseArchive	!<	アーカイブを使用するか
		 */
		void CanUseArchive(bool argCanUseArchive);

#pragma endregion	Accessor

	};
};