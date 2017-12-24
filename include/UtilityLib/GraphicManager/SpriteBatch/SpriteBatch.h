/**
 *	@file	SpriteBatch.h
 *	@brief	テクスチャの描画クラス
 *	@date	2017 / 8 / 27
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 *	@note	http://marupeke296.com/NGDV_No3_Sprite.html
 */
#pragma once

#include <memory>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <Windows.h>

#include "../BlendStateManager/BlendStateManager.h"

struct ID3D11DeviceContext;

namespace Utility
{
	class Texture;

	struct Sprite
	{
	public:

		DirectX::XMFLOAT3 Pos;			//	!<	基準座標
		DirectX::XMFLOAT2 PolygonSize;	//	!<	板ポリゴンサイズ
		DirectX::XMFLOAT2 PivotPos;		//	!<	ピボット座標
		DirectX::XMFLOAT2 Scale;		//	!<	スケール
		DirectX::XMFLOAT2 UvPos;		//	!<	UV左上座標
		DirectX::XMFLOAT2 UvSize;		//	!<	UVサイズ
		DirectX::XMFLOAT4 Color;		//	!<	r,g,b,a
		float Radian;					//	!<	回転角度（ラジアン）
		std::shared_ptr<Texture> Tex;	//	!<	テクスチャ

	public:
		/**
		 *	@constructor	Sprite
		 *	@brief			画像フーリエ変換用
		 *	@param[in]		argTexture		!<	描画するテクスチャクラス
		 *	@param[in]		argScreenSize	!<	スクリーンのサイズ
		 */
		Sprite(const std::shared_ptr<Texture> &argTexture, DirectX::XMFLOAT2 argScreenSize)
			: Pos(DirectX::XMFLOAT3(0.f, 0.f, 0.f)), PolygonSize(argScreenSize),
			PivotPos(DirectX::XMFLOAT2(0.f, 0.f)), Scale(DirectX::XMFLOAT2(0.f, 0.f)), UvPos(DirectX::XMFLOAT2(0.f, 0.f)), UvSize(DirectX::XMFLOAT2(1.f, 1.f)),
			Color(DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f)), Radian(0.f), Tex(argTexture)
		{
		}
		/**
		 *	@constructor	Sprite
		 *	@brief			画像フーリエ変換用
		 *	@param[in]		argTexture		!<	描画するテクスチャクラス
		 */
		Sprite(const std::shared_ptr<Texture> &argTexture)
			: Pos(DirectX::XMFLOAT3(0.f, 0.f, 0.f)), PolygonSize(DirectX::XMFLOAT2(128.f, 128.f)),
			PivotPos(DirectX::XMFLOAT2(0.f, 0.f)), Scale(DirectX::XMFLOAT2(0.f, 0.f)), UvPos(DirectX::XMFLOAT2(0.f, 0.f)), UvSize(DirectX::XMFLOAT2(1.f, 1.f)),
			Color(DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f)), Radian(0.f), Tex(argTexture)
		{
		}
		Sprite() = default;
		~Sprite() = default;

	};

	class SpriteEffect
	{
	private:
		class Impl;
		std::unique_ptr<Impl>pImpl;

	public:
		SpriteEffect(Sprite *argSprite);
		~SpriteEffect();
	};

	class SpriteBatch final
	{
	private:
		class Impl;
		std::unique_ptr<Impl>pImpl;

	public:
		/**
		 *	@constructor	SpriteBatch
		 *	@brief			画像をバッチ処理で描画する
		 *	@param[in]		argContext		!<	コンテキスト
		 */
		SpriteBatch(ID3D11DeviceContext *argContext);
		/**
		 *	@constructor	SpriteBatch
		 *	@brief			画像をバッチ処理で描画する
		 *	@param[in]		argContext		!<	コンテキスト
		 *	@param[in]		argScreenSize	!<	スクリーンのサイズ
		 */
		SpriteBatch(ID3D11DeviceContext *argContext, DirectX::XMFLOAT2 argScreenSize);
		~SpriteBatch();

	public:
		/**
		 *	@fn				ChangeBlendMode
		 *	@brief			画像の合成モードの変更
		 *	@param[in]		argBlendMode	!<	変更したいブレンドモード
		 */
		void ChangeBlendMode(eBlendMode argBlendMode);
		/**
		 *	@fn				Draw
		 *	@brief			登録した画像を一括で描画
		 */
		void Draw();
		/**
		 *	@fn			Entry
		 *	@brief		座標、色、指定で描画リストへの登録（XMFLOAT.ver）
		 *	@param[in]	argTexture	!<	描画したいテクスチャ
		 *	@param[in]	argPosition	!<	描画する座標
		 *	@param[in]	argColor	!<	テクスチャの色
		 */
		void Entry(const std::weak_ptr<Texture> &argTexture, DirectX::XMFLOAT2 const& argPosition, DirectX::FXMVECTOR argColor = DirectX::Colors::White);
		/**
		 *	@fn			Entry
		 *	@brief		座標、色、角度、中心座標、大きさ（縦横同じ）、深度、指定で描画リストへの登録（XMFLOAT.ver）
		 *	@param[in]	argTexture		!<	描画したいテクスチャ
		 *	@param[in]	argPosition		!<	描画する座標
		 *	@param[in]	argColor		!<	テクスチャの色
		 *	@param[in]	argRotation		!<	描画角度
		 *	@param[in]	argOrigin		!<	テクスチャの中心座標
		 *	@param[in]	argScale		!<	テクスチャの大きさ
		 *	@param[in]	argLayerDepth	!<	レイヤーの深度
		 */
		void Entry(const std::weak_ptr<Texture> &argTexture, DirectX::XMFLOAT2 const& argPosition, RECT const* argUVRect, DirectX::FXMVECTOR argColor = DirectX::Colors::White, float argRotation = 0, DirectX::XMFLOAT2 const& argOrigin = DirectX::XMFLOAT2(0, 0), float argScale = 1, float argLayerDepth = 0);
		/**
		 *	@fn			Entry
		 *	@brief		座標、色、角度、中心座標、大きさ（縦横別）、深度、指定で描画リストへの登録（XMFLOAT.ver）
		 *	@param[in]	argTexture		!<	描画したいテクスチャ
		 *	@param[in]	argPosition		!<	描画する座標
		 *	@param[in]	argColor		!<	テクスチャの色
		 *	@param[in]	argRotation		!<	描画角度
		 *	@param[in]	argOrigin		!<	テクスチャの中心座標
		 *	@param[in]	argScale		!<	テクスチャの大きさ
		 *	@param[in]	argLayerDepth	!<	レイヤーの深度
		 */
		void Entry(const std::weak_ptr<Texture> &argTexture, DirectX::XMFLOAT2 const& argPosition, RECT const* argUVRect, DirectX::FXMVECTOR argColor, float argRotation, DirectX::XMFLOAT2 const& argOrigin, DirectX::XMFLOAT2 const& argScale, float argLayerDepth = 0);
		/**
		 *	@fn			Entry
		 *	@brief		座標、色、指定で描画リストへの登録（XMVECTOR.ver）
		 *	@param[in]	argTexture	!<	描画したいテクスチャ
		 *	@param[in]	argPosition	!<	描画する座標
		 *	@param[in]	argColor	!<	テクスチャの色
		 */
		void Entry(const std::weak_ptr<Texture> &argTexture, DirectX::FXMVECTOR argPosition, DirectX::FXMVECTOR argColor = DirectX::Colors::White);
		/**
		 *	@fn			Entry
		 *	@brief		座標、色、角度、中心座標、大きさ（縦横同じ）、深度、指定で描画リストへの登録（XMVECTOR.ver）
		 * 	@param[in]	argTexture		!<	描画したいテクスチャ
		 *	@param[in]	argPosition		!<	描画する座標
		 *	@param[in]	argColor		!<	テクスチャの色
		 *	@param[in]	argRotation		!<	描画角度
		 *	@param[in]	argOrigin		!<	テクスチャの中心座標
		 *	@param[in]	argScale		!<	テクスチャの大きさ
		 *	@param[in]	argLayerDepth	!<	レイヤーの深度
		 */
		void Entry(const std::weak_ptr<Texture> &argTexture, DirectX::FXMVECTOR argPosition, RECT const* argUVRect, DirectX::FXMVECTOR argColor = DirectX::Colors::White, float argRotation = 0, DirectX::FXMVECTOR argOrigin = DirectX::g_XMZero, float argScale = 1, float argLayerDepth = 0);
		/**
		 *	@fn			Entry
		 *	@brief		座標、色、角度、中心座標、大きさ（縦横別）、深度、指定で描画リストへの登録（XMVECTOR.ver）
		 *	@param[in]	argTexture		!<	描画したいテクスチャ
		 *	@param[in]	argPosition		!<	描画する座標
		 *	@param[in]	argColor		!<	テクスチャの色
		 *	@param[in]	argRotation		!<	描画角度
		 *	@param[in]	argOrigin		!<	テクスチャの中心座標
		 *	@param[in]	argScale		!<	テクスチャの大きさ
		 *	@param[in]	argLayerDepth	!<	レイヤーの深度
		 */
		void Entry(const std::weak_ptr<Texture> &argTexture, DirectX::FXMVECTOR argPosition, RECT const* argUVRect, DirectX::FXMVECTOR argColor, float argRotation, DirectX::FXMVECTOR argOrigin, DirectX::GXMVECTOR scalefloat, float argLayerDepth = 0);

	};
};