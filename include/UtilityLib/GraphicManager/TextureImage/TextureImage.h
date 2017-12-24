/**
 *	@file	TextureImage.h
 *	@brief	WICの読み込み
 *	@date	2017 / 8 / 27
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 *	@note	http://qiita.com/ousttrue/items/a730cc44bafbc34b2ff5
 *	@note	https://github.com/ousttrue/MinTriangle
 */

#pragma once

#include <wrl/client.h>
#include <memory>
#include <vector>
#include <d3d11.h>
#include <d3d11_1.h>
#include <DirectXMath.h>

struct IWICImagingFactory;
struct IWICBitmapDecoder;

namespace Utility
{
	class Image
	{
	private:
		std::vector<unsigned char>	buffer_;
		DirectX::XMFLOAT2	size_;
		int					pixelBytes_;

	public:
		/**
		 *	@constructor	eImage
		 *	@brief			画像
		 *	@param[in]		argRectSize		!<	画像の大きさ
		 *	@param[in]		argPixelBytes	!<	ピクセルのバイト数
		 */
		Image::Image(DirectX::XMFLOAT2 argRectSize, int argPixelBytes)
			: size_(argRectSize), pixelBytes_(argPixelBytes)
			, buffer_(static_cast<unsigned int>(argRectSize.x) * static_cast<unsigned int>(argRectSize.y) * argPixelBytes)
		{
		}

		Image::~Image() = default;
	public:

		inline int Stride()const { return static_cast<int>(size_.x) * pixelBytes_; }
		inline unsigned char* Ptr() { return (buffer_.empty()) ? nullptr : buffer_.data(); }
		inline size_t UVSize()const { return buffer_.size(); }
		inline DirectX::XMFLOAT2 RectSize()const { return size_; }
	};

	class ImageFactory
	{
	private:
		Microsoft::WRL::ComPtr<IWICImagingFactory> factory_;
	private:
		/**
		 *	@fn			Load
		 *	@brief		画像の読み込み
		 *	@param[in]	argBitmapDecoder	!<	画像の復号化オブジェクト
		 *	@return		復号化画像データ
		 */
		std::unique_ptr<Image> Load(Microsoft::WRL::ComPtr<IWICBitmapDecoder> argBitmapDecoder);

	public:
		ImageFactory();
		~ImageFactory();
	public:
		/**
		 *	@fn			Load
		 *	@brief		画像の読み込み
		 *	@param[in]	argPath	!<	画像のパス
		 *	@return		復号化画像データ
		 */
		std::unique_ptr<Image> Load(const std::wstring &argPath);
		/**
		 *	@fn			Load
		 *	@brief		画像の読み込み
		 *	@param[in]	argMemory		!<	画像のバイナリデータ
		 *	@param[in]	argFileLength	!<	バイナリサイズ
		 *	@return		復号化画像データ
		 */
		std::unique_ptr<Image> Load(unsigned char *argMemory, const unsigned long argFileLength);

	};

	class Texture
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11Texture2D>			texture_;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	srv_;
		Microsoft::WRL::ComPtr<ID3D11SamplerState>		sampler_;
		std::unique_ptr<Image>						image_;
		/**
		 *	@fn			Create
		 *	@brief		テクスチャの作成
		 *	@param[in]	argDevice	!<	デバイス
		 */
		void Create(ID3D11Device *argDevice);
	public:
		/**
		 *	@constructor	Texture
		 *	@brief			描画用テクスチャ
		 *	@param[in]		argPath		!<	画像のパス
		 *	@param[in]		argDevice	!<	デバイス
		 */
		Texture(const std::wstring &argPath, ID3D11Device *argDevice);
		/**
		 *	@constructor	Texture
		 *	@brief			描画用テクスチャ
		 *	@param[in]		argMemory		!<	画像のバイナリデータ
		 *	@param[in]		argFileLength	!<	バイナリサイズ
		 */
		Texture(unsigned char *argMemory, const unsigned long argFileLength, ID3D11Device *argDevice);
		/**
		 *	@constructor	Texture
		 *	@brief			描画用テクスチャ
		 *	@param[in]		argImage	!<	画像クラス
		 *	@param[in]		argDevice	!<	デバイス
		 */
		Texture(std::unique_ptr<Image> &&argImage, ID3D11Device *argDevice);
		/**
		 *	@constructor	Texture
		 *	@brief			描画用テクスチャ
		 *	@param[in]		argTexture	!<	画像クラス
		 *	@param[in]		argDevice	!<	デバイス
		 */
		Texture(ID3D11Texture2D *argTexture, ID3D11Device *argDevice);
		
		~Texture();
	public:

		inline Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture2D()const { return texture_; }
		inline Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Srv()const { return srv_; }
		inline Microsoft::WRL::ComPtr<ID3D11SamplerState> Sampler()const { return sampler_; }
		inline DirectX::XMFLOAT2 ImageSize()const { return image_->RectSize(); }
	};
};