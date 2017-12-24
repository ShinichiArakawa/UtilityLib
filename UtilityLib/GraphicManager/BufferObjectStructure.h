/**
 *	@file	BufferObjectStructure.h
 *	@date	2017 / 10 / 16
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <DirectXMath.h>

namespace Utility
{
	/**
	 *	@struct	SimpleVertex
	 *	@brief	簡易的な描画に扱う頂点構造体
	 */
	struct SimpleVertex
	{
		DirectX::XMFLOAT4 Pos;
		SimpleVertex()
			: Pos(DirectX::XMFLOAT4())
		{
			static_assert((sizeof(SimpleVertex) % 16) == 0, "CB must be 16-byte aligned");
		}
	};
	/**
	 *	@struct	SimpleMaterial
	 *	@brief	簡易的な描画に扱うマテリアル構造体
	 */
	struct SimpleMaterial
	{
		DirectX::XMFLOAT3	Emissive;
		DirectX::XMFLOAT3	Ambient;
		DirectX::XMFLOAT3	Diffuse;
		DirectX::XMFLOAT3	Specular;

		SimpleMaterial()
			: Emissive(DirectX::XMFLOAT3()), Ambient(DirectX::XMFLOAT3()), Diffuse(DirectX::XMFLOAT3()), Specular(DirectX::XMFLOAT3())
		{
			static_assert((sizeof(SimpleMaterial) % 16) == 0, "CB must be 16-byte aligned");
		}
	};
	/**
	 *	@struct	SimpleMatrix
	 *	@brief	簡易的な描画に扱う行列構造体
	 */
	struct SimpleMatrix
	{
		DirectX::XMFLOAT4X4 World;
		DirectX::XMFLOAT4X4 View;
		DirectX::XMFLOAT4X4 Proj;
		SimpleMatrix()
			:World(DirectX::XMFLOAT4X4()), View(DirectX::XMFLOAT4X4()), Proj(DirectX::XMFLOAT4X4())
		{
			static_assert((sizeof(SimpleMatrix) % 16) == 0, "CB must be 16-byte aligned");
		}
		SimpleMatrix(DirectX::XMFLOAT4X4 argWorld, DirectX::XMFLOAT4X4 argView, DirectX::XMFLOAT4X4 argProj)
			:World(argWorld), View(argView), Proj(argProj)
		{
			static_assert((sizeof(SimpleMatrix) % 16) == 0, "CB must be 16-byte aligned");
		}
	};

	/**
	 *	@struct TextureVertex
	 *	@brief	テクスチャの頂点バッファ
	 */
	struct TextureVertex
	{
	public:
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT2 UV;
	public:
		TextureVertex()
			:Pos(DirectX::XMFLOAT3()), UV(DirectX::XMFLOAT2())
		{
		}
		TextureVertex(DirectX::XMFLOAT3 argUvPos, DirectX::XMFLOAT2 argUv)
			:Pos(argUvPos), UV(argUv)
		{
		}
	};//	struct TextureVertex
	/**
	 *	@struct TextureMatrix
	 *	@brief	テクスチャの行列
	 */
	struct TextureMatrix
	{
	public:
		DirectX::XMFLOAT4X4 World;
		DirectX::XMFLOAT4X4 Proj;

	public:
		TextureMatrix()
			:World(DirectX::XMFLOAT4X4()), Proj(DirectX::XMFLOAT4X4())
		{
			static_assert((sizeof(TextureMatrix) % 16) == 0, "CB must be 16-byte aligned");
		}

	};//	struct TextureMatrix


}