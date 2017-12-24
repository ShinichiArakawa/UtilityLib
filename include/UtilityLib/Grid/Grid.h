/**
 *	@file	Grid.h
 *	@date	2017 / 8 / 21
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once
#include <memory>
#include <DirectXMath.h>

struct ID3D11DeviceContext;

namespace Utility
{
	class Grid
	{
	private:
		class Impl;
		std::unique_ptr<Impl> pImpl;
	public:
		Grid(ID3D11DeviceContext *argContext);
		virtual ~Grid();

		/**
		 *	@fn			Draw
		 *	@brief		描画
		 *	@param[in]	argView	!<	ビュー行列
		 *	@param[in]	argProj	!<	透視射影変換行列
		 */
		void Draw(DirectX::XMFLOAT4X4 argView, DirectX::XMFLOAT4X4 argProj);
		/**
		 *	@fn			IsVisible
		 *	@brief		描画するか
		 *	@param[in]	argIVisble	!<	描画するか指定
		 */
		void IsVisible(bool argIVisble);

	};
};