/**
 *	@file	PaintBrush.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../../DirectX/Direct2DBase.h"

namespace Utility
{
	class PaintBrush
	{
	private:
		class Impl;
		std::unique_ptr<Impl> pImpl;

	protected:
		virtual void Draw() = 0;
		ID2D1Factory* GetFactory();
		ID2D1RenderTarget* GetRenderTarget();
		// 影カラー
		void SetShadowColor(const D2D1_COLOR_F argColor);

		// 線幅
		float LineWidth() const;
		void LineWidth(float argWidth);

		// ラウンド
		void Round(float argWidth, float argHeight);
		void Round(const D2D1_SIZE_F& argSize);
		const D2D1_SIZE_F& Round() const;

		// カラー
		void MainColor(const D2D1_COLOR_F& argColor);
		const D2D1_COLOR_F& MainColor() const;

		// サブカラー
		void SubColor(const D2D1_COLOR_F& argColor);
		const D2D1_COLOR_F& SubColor() const;

		// ドロップシャドウの有効
		void ShadowEnable(bool argIsShadow);
		bool ShadowEnable() const;

		// ドロップシャドウのオフセット
		void ShadowOffset(float argPosX, float argPosY);
		const D2D_SIZE_F& ShadowOffset() const;

		// 円弧の開始角度
		void ArcStartAngle(float argAngle);
		float ArcStartAngle() const;

		/**
		 *	@fn			ToTransform
		 *	@brief		回転移動行列
		 *	@param[in]	argAngle	!<	角度
		 *	@param[in]	argCenterX	!<	円座標x
		 *	@param[in]	argCenterY	!<	円座標y
		 *	@param[in]	argPosX		!<	座標x
		 *	@param[in]	argPosY		!<	座標y
		 */
		D2D1_MATRIX_3X2_F ToTransform(float argAngle, float argCenterX, float argCenterY, float argPosX, float argPosY);
		/**
		 *	@fn			DrawArc
		 *	@brief		円弧描画（センター指定）
		 *	@param[in]	argCirlceX		!<	中心座標x
		 *	@param[in]	argCirlceY		!<	中心座標y
		 *	@param[in]	argRadius		!<	半径
		 *	@param[in]	argStartAngle	!<	開始角度
		 *	@param[in]	argEndAngle		!<	終了角度角度
		 */
		void DrawArc(float argCircleX, float argCircleY, float argRadius, float argStartAngle, float argEndAngle);
		/**
		 *	@fn			DrawArc
		 *	@brief		円弧描画（サイズ指定）
		 *	@param[in]	argPosX			!<	座標x
		 *	@param[in]	argPosY			!<	座標y
		 *	@param[in]	argWidth		!<	幅
		 *	@param[in]	argHeight		!<	高さ
		 *	@param[in]	argStartAngle	!<	開始角度
		 *	@param[in]	argEndAngle		!<	終了角度角度
		 */
		void DrawArc(float argPosX, float argPosY, float argWidth, float argHeight, float argStartAngle, float argEndAngle);
		/**
		 *	@fn			FillArc
		 *	@brief		円弧塗りつぶし（センター指定）
		 *	@param[in]	argCirlceX		!<	中心座標x
		 *	@param[in]	argCirlceY		!<	中心座標y
		 *	@param[in]	argRadius		!<	半径
		 *	@param[in]	argStartAngle	!<	開始角度
		 *	@param[in]	argEndAngle		!<	終了角度角度
		 */
		void FillArc(float argCirlceX, float argCirlceY, float argRadius, float argStartAngle, float argEndAngle);
		/**
		 *	@fn			FillArc
		 *	@brief		円弧塗りつぶし（サイズ指定）
		 *	@param[in]	argPosX			!<	座標x
		 *	@param[in]	argPosY			!<	座標y
		 *	@param[in]	argWidth		!<	幅
		 *	@param[in]	argHeight		!<	高さ
		 *	@param[in]	argStartAngle	!<	開始角度
		 *	@param[in]	argEndAngle		!<	終了角度角度
		 */
		void FillArc(float argPosX, float argPosY, float argWidth, float argHeight, float argStartAngle, float argEndAngle);
		/**
		 *	@fn			DrawChecker
		 *	@brief		チェッカー描画
		 *	@param[in]	argLeft		!<	座標x
		 *	@param[in]	argTop		!<	座標y
		 *	@param[in]	argNumX		!<	
		 *	@param[in]	argNumY		!<	
		 *	@param[in]	argWidth	!<	幅
		 *	@param[in]	argHeight	!<	高さ
		 */
		void DrawChecker(float argLeft, float argTop, int argNumX, int argNumY, float argWidth, float argHeight);
		/**
		 *	@fn			DrawRoundRect
		 *	@brief		角丸矩形描画
		 *	@param[in]	argLeft		!<	座標x
		 *	@param[in]	argTop		!<	座標y
		 *	@param[in]	argWidth	!<	幅
		 *	@param[in]	argHeight	!<	高さ
		 */
		void DrawRoundRect(float argLeft, float argTop, float argWidth, float argHeight);
		/**
		 *	@fn			FillRoundRect
		 *	@brief		角丸矩形塗りつぶし
		 *	@param[in]	argLeft		!<	座標x
		 *	@param[in]	argTop		!<	座標y
		 *	@param[in]	argWidth	!<	幅
		 *	@param[in]	argHeight	!<	高さ
		 */
		void FillRoundRect(float argLeft, float argTop, float argWidth, float argHeight);
		/**
		 *	@fn			DrawRect
		 *	@brief		矩形描画
		 *	@param[in]	argLeft		!<	座標x
		 *	@param[in]	argTop		!<	座標y
		 *	@param[in]	argWidth	!<	幅
		 *	@param[in]	argHeight	!<	高さ
		 */
		void DrawRect(float argLeft, float argTop, float argWidth, float argHeight);
		/**
		 *	@fn			FillRect
		 *	@brief		矩形塗りつぶし
		 *	@param[in]	argLeft		!<	座標x
		 *	@param[in]	argTop		!<	座標y
		 *	@param[in]	argWidth	!<	幅
		 *	@param[in]	argHeight	!<	高さ
		 */
		void FillRect(float argLeft, float argTop, float argWidth, float argHeight);
		/**
		 *	@fn			DrawCircle
		 *	@brief		円描画
		 *	@param[in]	argPoints	!<	塗りつぶす頂点の座標
		 *	@param[in]	argMax		!<	頂点の数
		 */
		// 
		void DrawCircle(float argCenterX, float argCenterY, float argRadius);
		/**
		 *	@fn			DrawEllipse
		 *	@brief		楕円描画
		 *	@param[in]	argLeft		!<	座標x
		 *	@param[in]	argTop		!<	座標y
		 *	@param[in]	argWidth	!<	幅
		 *	@param[in]	argHeight	!<	高さ
		 */
		void DrawEllipse(float argLeft, float argTop, float argWidth, float argHeight);
		/**
		 *	@fn			FillCircle
		 *	@brief		円塗りつぶし
		 *	@param[in]	argCenterX	!<	中心座標x
		 *	@param[in]	argCenterY	!<	中心座標y
		 *	@param[in]	argRadius	!<	半径
		 */
		void FillCircle(float argCenterX, float argCenterY, float argRadius);
		/**
		 *	@fn			FillEllipse
		 *	@brief		楕円塗りつぶし
		 *	@param[in]	argLeft		!<	座標x
		 *	@param[in]	argTop		!<	座標y
		 *	@param[in]	argWidth	!<	幅
		 *	@param[in]	argHeight	!<	高さ
		 */
		void FillEllipse(float argLeft, float argTop, float argWidth, float argHeight);
		/**
		 *	@fn			DrawLine
		 *	@brief		直線描画
		 *	@param[in]	argStartX	!<	開始頂点座標x
		 *	@param[in]	argStartY	!<	開始頂点座標y
		 *	@param[in]	argEndX		!<	終了頂点座標x
		 *	@param[in]	argEndY		!<	終了頂点座標y
		 */
		void DrawLine(float argStartX, float argStartY, float argEndX, float argEndY);
		/**
		 *	@fn			DrawLinesOpen
		 *	@brief		開いた連続ライン
		 *	@param[in]	argPoints	!<	塗りつぶす頂点の座標
		 *	@param[in]	argMax		!<	頂点の数
		 */
		void DrawLinesOpen(D2D1_POINT_2F *argPoints, int argMax);
		/**
		 *	@fn			DrawLinesClosed
		 *	@brief		閉じた連続ライン
		 *	@param[in]	argPoints	!<	塗りつぶす頂点の座標
		 *	@param[in]	argMax		!<	頂点の数
		 */
		void DrawLinesClosed(D2D1_POINT_2F *argPoints, int argMax);
		/**
		 *	@fn			FillLinesClosed
		 *	@brief		閉じた連続ライン塗りつぶし
		 *	@param[in]	argPoints	!<	塗りつぶす頂点の座標
		 *	@param[in]	argMax		!<	頂点の数
		 */
		void FillLinesClosed(D2D1_POINT_2F *argPoints, int argMax);
		/**
		 *	@fn			DrawCurve
		 *	@brief		カーブライン
		 *	@param[in]	argPoints	!<	塗りつぶす頂点の座標
		 *	@param[in]	argMax		!<	頂点の数
		 */
		void DrawCurve(D2D1_POINT_2F *argPoints, int argMax);
		/**
		 *	@fn			FillCurve
		 *	@brief		カーブ塗りつぶし
		 *	@param[in]	argPoints	!<	塗りつぶす頂点の座標
		 *	@param[in]	argMax		!<	頂点の数
		 */
		void FillCurve(D2D1_POINT_2F *argPoints, int argMax);

	public:
		PaintBrush();
		virtual ~PaintBrush();

	public:
		void Draw(ID2D1RenderTarget* argRt);

	};
};