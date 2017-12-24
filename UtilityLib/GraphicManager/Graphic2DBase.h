/**
 *	@file   Graphic2DBase.h
 *	@date   2017 / 08 / 31
 *	@author Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <memory>
#include "../DirectX/Direct2DBase.h"
#include "../DirectX/DWriteBase.h"
#include "SpriteBatch/SpriteBatch.h"
#include "TextureImage/TextureImage.h"

struct ID3D11DeviceContext;
struct IDXGISwapChain;

namespace Utility
{
	class Graphic2DBase
	{
	private:
		class Impl;
		std::unique_ptr<Impl>pImpl;

	public:
		Graphic2DBase(ID3D11DeviceContext *argContext, IDXGISwapChain *argSwapChain, std::unique_ptr<SpriteBatch> &&argSpriteBatch);
		~Graphic2DBase();

	public:
		/**
		 *	@fn			CanUseArchive
		 *	@brief		アーカイブの設定
		 *	@param[in]	argCanUseArchive	!<	アーカイブを使用するか
		 */
		void CanUseArchive(bool argCanUseArchive);
		/**
		 *	@fn			AddAndLoadImage
		 *	@brief		テクスチャのロード
		 *	@param[in]	argTextureFilename	!<	ロードするテクスチャのパス
		 *	@return		Graphic2DBaseを返してそこから登録形式を指定する
		 */
		Graphic2DBase *AddAndLoadImage(const std::wstring argTextureFilename);
		/**
		 *	@fn			FindIndexTexture
		 *	@brief		テクスチャの取得
		 *	@param[in]	argKey	!<	取得したいテクスチャのキー
		 *	@return		テクスチャ
		 */
		std::shared_ptr<Texture> FindIndexTexture(const std::wstring argKey)const;
		/**
		 *	@fn			DrawAndWriteText
		 *	@brief		テキストの描画
		 *	@param[in]	argStr	!<	書き込むテキスト
		 *	@param[in]	argPos	!<	テキストを描画する座標
		 */
		void DrawAndWriteText(const std::wstring argStr, DirectX::XMFLOAT2 argPos)const;
		/**
		 *	@fn			DrawAndWriteText
		 *	@brief		テキストの描画
		 *	@param[in]	argStr			!<	書き込むテキスト
		 *	@param[in]	argPos			!<	テキストを描画する座標
		 *	@param[in]	argScreenSize	!<	スクリーンサイズ
		 */
		void DrawAndWriteText(const std::wstring argStr, DirectX::XMFLOAT2 argPos, DirectX::XMFLOAT2 argScreenSize)const;
		/**
		  *	@fn			ConfigTextColor
		  *	@brief		テキストブラシの色設定
		  *	@param[in]	argColor	!<	変更させたいテキストの色
		  */
		void ConfigTextColor(D2D1::ColorF argColor);
		/**
		 *	@fn			ConfigTextSize
		 *	@brief		テキストのサイズ設定
		 *	@param[in]	argColor	!<	変更させたいテキストのサイズ
		 */
		void ConfigTextSize(float argStrSize);

		/**
		 *	@fn		Png
		 *	@brief	Pngテクスチャの登録
		 */
		void Png();
		/**
		 *	@fn		Tex
		 *	@brief	拡張子の決まったテクスチャの登録
		 */
		void Tex();

#pragma region		Accessor
	public:
		SpriteBatch *GetSpriteBatch()const;
		DWriteBase *DirectWrite()const;
		ID2D1RenderTarget *RenderTarget2d(eRtDepth argType)const;
		ID2D1RenderTarget *RenderTarget2d()const;
		void ImagePath(std::wstring argPath)const;
		std::wstring ImagePath();
		void ChangeRt2dDepth(eRtDepth argType);
#pragma endregion	Accessor
	};

#pragma region SpriteBatchDraw

	/**
	 *	@fn			DrawNum
	 *	@brief		数字のテクスチャ描画
	 *	@param[in]	描画位置
	 *	@param[in]	描画サイズ
	 *	@param[in]	描画したい数字
	 *	@param[in]	扱うテクスチャ
	 *	@param[in]	最大桁数
	 *	@param[in]	テクスチャの色
	 */
	extern void DrawNum(DirectX::XMFLOAT2 argPos, DirectX::XMFLOAT2 argSize, const float argNum, const std::weak_ptr<Texture> &argTexture, const int argDigitMax, DirectX::XMFLOAT4 argColor = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f));
	/**
	*	@fn			DrawNum
	*	@brief		数字のテクスチャ描画
	*	@param[in]	描画位置
	*	@param[in]	描画サイズ
	*	@param[in]	画像の大きさ
	*	@param[in]	描画したい数字
	*	@param[in]	扱うテクスチャ
	*	@param[in]	最大桁数
	*	@param[in]	テクスチャの色
	*/
	extern void DrawNum(DirectX::XMFLOAT2 argPos, DirectX::XMFLOAT2 argSize, float argExRate, const float argNum, const std::weak_ptr<Texture> &argTexture, const int argDigitMax, DirectX::XMFLOAT4 argColor);
	/**
	*	@fn			DrawGraph（描画サイズ、深度指定なし）
	*	@brief		指定位置、原寸、でテクスチャを描画
	*	@param[in]	argPos		!<	描画座標
	*	@param[in]	argTexture	!<	描画するテクスチャ
	*/
	extern void DrawGraph(DirectX::XMFLOAT2 argPos, const std::weak_ptr<Texture> &argTexture);
	/**
	*	@fn			DrawGraph（描画サイズ、深度指定あり）
	*	@brief		指定位置、指定サイズ、指定の奥行、でテクスチャを描画
	*	@param[in]	argPos		!<	描画座標
	*	@param[in]	argSize		!<	描画サイズ
	*	@param[in]	argDepth	!<	z軸の描画座標
	*	@param[in]	argTexture	!<	描画するテクスチャ
	*/
	extern void DrawGraph(DirectX::XMFLOAT2 argPos, DirectX::XMFLOAT2 argSize, float argDepth, const std::weak_ptr<Texture> &argTexture);
	/**
	*	@fn			DrawGraph（深度指定なし、色指定あり）
	*	@brief		指定位置、指定サイズ、指定の奥行、でテクスチャを描画
	*	@param[in]	argPos		!<	描画座標
	*	@param[in]	argSize		!<	描画サイズ
	*	@param[in]	argTexture	!<	描画するテクスチャ
	*	@param[in]	argColor	!<	テクスチャの色
	*/
	void DrawGraph(DirectX::XMFLOAT2 argPos, const std::weak_ptr<Texture> &argTexture, DirectX::XMVECTOR argColor);
	/**
	*	@fn			DrawRectGraph（色、アルファ指定無し）
	*	@brief		指定位置、指定サイズ、切り抜き、でテクスチャを描画
	*	@param[in]	argPos		!<	描画座標
	*	@param[in]	argCutPos	!<	描画するテクスチャ上の矩形
	*	@param[in]	argSize		!<	描画サイズ
	*	@param[in]	argTexture	!<	描画するテクスチャ
	*/
	extern void DrawRectGraph(DirectX::XMFLOAT2 argPos, DirectX::XMFLOAT2 argCutPos, DirectX::XMFLOAT2 argSize, const std::weak_ptr<Texture> &argTexture);
	/**
	*	@fn			DrawRectGraph（色、アルファ指定あり）
	*	@brief		指定位置、指定サイズ、切り抜き、指定の色、でテクスチャを描画
	*	@param[in]	argPos		!<	描画座標
	*	@param[in]	argCutPos	!<	描画するテクスチャ上の矩形
	*	@param[in]	argSize		!<	描画サイズ
	*	@param[in]	argTexture	!<	描画するテクスチャ
	*	@param[in]	argColor	!<	描画するテクスチャの色とアルファ
	*/
	extern void DrawRectGraph(DirectX::XMFLOAT2 argPos, DirectX::XMFLOAT2 argCutPos, DirectX::XMFLOAT2 argSize, const std::weak_ptr<Texture> &argTexture, DirectX::XMFLOAT4 argColor);
	/**
	*	@fn			DrawRotaGraph（回転中心無し）
	*	@brief		指定位置、指定サイズ、指定拡大率、指定角度、でテクスチャを描画
	*	@param[in]	argPos		!<	描画座標
	*	@param[in]	argSize		!<	描画サイズ
	*	@param[in]	argExRate	!<	拡大率
	*	@param[in]	argAngle	!<	描画角度
	*	@param[in]	argTexture	!<	描画するテクスチャ
	*/
	extern void DrawRotaGraph(DirectX::XMFLOAT2 argPos, DirectX::XMFLOAT2 argSize, float argExRate, float argAngle, const std::weak_ptr<Texture> &argTexture);
	/**
	*	@fn			DrawRotaGraph（回転中心あり）
	*	@brief		指定位置、指定サイズ、指定中心座標、指定拡大率、指定角度、でテクスチャを描画
	*	@param[in]	argPos		!<	描画座標
	*	@param[in]	argSize		!<	描画サイズ
	*	@param[in]	argCenter	!<	描画の中心座標
	*	@param[in]	argExRate	!<	拡大率
	*	@param[in]	argAngle	!<	描画角度
	*	@param[in]	argTexture	!<	描画するテクスチャ
	*/
	extern void DrawRotaGraph(DirectX::XMFLOAT2 argPos, DirectX::XMFLOAT2 argSize, DirectX::XMFLOAT2 argCenter, float argExRate, float argAngle, const std::weak_ptr<Texture> &argTexture);
	/**
	*	@fn			DrawRotaGraph（回転中心、縦横別の拡縮アリ）
	*	@brief		指定位置、指定サイズ、指定中心座標、指定拡大率、指定角度、でテクスチャを描画
	*	@param[in]	argPos		!<	描画座標
	*	@param[in]	argSize		!<	描画サイズ
	*	@param[in]	argCenter	!<	描画の中心座標
	*	@param[in]	argExRate	!<	拡大率
	*	@param[in]	argAngle	!<	描画角度
	*	@param[in]	argTexture	!<	描画するテクスチャ
	*/
	extern void DrawRotaGraph(DirectX::XMFLOAT2 argPos, DirectX::XMFLOAT2 argSize, DirectX::XMFLOAT2 argCenter, DirectX::XMFLOAT2 argExRate, float argAngle, const std::weak_ptr<Texture> &argTexture);
	/**
	*	@fn			DrawRotaGraph（回転中心、色、アルファ指定アリ）
	*	@brief		指定位置、指定サイズ、指定中心座標、指定拡大率、指定角度、指定の色、でテクスチャを描画
	*	@param[in]	argPos		!<	描画座標
	*	@param[in]	argSize		!<	描画サイズ
	*	@param[in]	argCenter	!<	描画の中心座標
	*	@param[in]	argExRate	!<	拡大率
	*	@param[in]	argAngle	!<	描画角度
	*	@param[in]	argTexture	!<	描画するテクスチャ
	*	@param[in]	argColor	!<	描画するテクスチャの色とアルファ
	*/
	extern void DrawRotaGraph(DirectX::XMFLOAT2 argPos, DirectX::XMFLOAT2 argSize, DirectX::XMFLOAT2 argCenter, DirectX::XMFLOAT2 argExRate, float argAngle, const std::weak_ptr<Texture> &argTexture, DirectX::XMFLOAT4 argColor);
	/**
	*	@fn			DrawRotaGraph（回転中心、色、アルファ指定アリ）
	*	@brief		指定位置、指定サイズ、指定中心座標、指定拡大率、指定角度、指定の色、でテクスチャを描画
	*	@param[in]	argPos		!<	描画座標
	*	@param[in]	argSize		!<	描画サイズ
	*	@param[in]	argCenter	!<	描画の中心座標
	*	@param[in]	argExRate	!<	拡大率
	*	@param[in]	argAngle	!<	描画角度
	*	@param[in]	argTexture	!<	描画するテクスチャ
	*	@param[in]	argColor	!<	描画するテクスチャの色とアルファ
	*/
	extern void DrawRotaGraph(DirectX::XMFLOAT2 argPos, DirectX::XMFLOAT2 argSize, DirectX::XMFLOAT2 argCenter, float argExRate, float argAngle, const std::weak_ptr<Texture> &argTexture, DirectX::XMFLOAT4 argColor);
	/**
	*	@fn			DrawRectRotaGraph（色とアルファの指定なし）
	*	@brief		指定位置、指定サイズ、切り抜き、指定拡大率、指定角度、でテクスチャを描画
	*	@param[in]	argPos		!<	描画座標
	*	@param[in]	argCutPos	!<	描画するテクスチャ上の矩形
	*	@param[in]	argSize		!<	描画サイズ
	*	@param[in]	argExRate	!<	拡大率
	*	@param[in]	argAngle	!<	描画角度
	*	@param[in]	argTexture	!<	描画するテクスチャ
	*/
	extern void DrawRectRotaGraph(DirectX::XMFLOAT2 argPos, DirectX::XMFLOAT2 argCutPos, DirectX::XMFLOAT2 argSize, float argExRate, float argAngle, const std::weak_ptr<Texture> &argTexture);
	/**
	*	@fn			DrawRectRotaGraph（色とアルファの指定あり）
	*	@brief		指定位置、指定サイズ、切り抜き、指定拡大率、指定角度、でテクスチャを描画
	*	@param[in]	argPos		!<	描画座標
	*	@param[in]	argCutPos	!<	描画するテクスチャ上の矩形
	*	@param[in]	argSize		!<	描画サイズ
	*	@param[in]	argExRate	!<	拡大率
	*	@param[in]	argAngle	!<	描画角度
	*	@param[in]	argTexture	!<	描画するテクスチャ
	*	@param[in]	argColor	!<	描画するテクスチャの色とアルファ
	*/
	extern void DrawRectRotaGraph(DirectX::XMFLOAT2 argPos, DirectX::XMFLOAT2 argCutPos, DirectX::XMFLOAT2 argSize, float argExRate, float argAngle, const std::weak_ptr<Texture> &argTexture, DirectX::XMFLOAT4 argColor);
#pragma endregion
};