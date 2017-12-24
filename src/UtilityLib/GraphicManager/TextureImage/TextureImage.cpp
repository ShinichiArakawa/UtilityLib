/**
 *	@file	TextureImage.cpp
 *	@brief	WICの読み込み
 *	@date	2017 / 8 / 27
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 *	@note	http://qiita.com/ousttrue/items/a730cc44bafbc34b2ff5
 *	@note	https://github.com/ousttrue/MinTriangle
 */
#include "TextureImage.h"
#include <wincodec.h>
#include <comdef.h>

#include "../../String/String.h"

#pragma region		ImageFactory

Utility::ImageFactory::ImageFactory()
{
	HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory_));
	_com_util::CheckError(hr);
	assert(SUCCEEDED(hr) && "Create IWICImagingFactory Failed...\n");
}

Utility::ImageFactory::~ImageFactory() = default;

std::unique_ptr<Utility::Image> Utility::ImageFactory::Load(const std::wstring &argPath)
{
	Microsoft::WRL::ComPtr<IWICBitmapDecoder> bitmapDecoder;
	HRESULT hr = factory_->CreateDecoderFromFilename(argPath.c_str(), 0, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &bitmapDecoder);

	std::wstring ws = L"Filename : " + argPath + L" CreateDecoderFromFilename Failed...\n";
	assert(SUCCEEDED(hr) && String::ToString(ws).c_str());

	return std::move(Load(bitmapDecoder));
}

std::unique_ptr<Utility::Image> Utility::ImageFactory::Load(unsigned char *argMemory, const unsigned long argFileLength)
{
	Microsoft::WRL::ComPtr<IWICStream> stream;
	HRESULT hr = factory_->CreateStream(&stream);
	assert(SUCCEEDED(hr) && "Create Stream Failed...");

	hr = stream->InitializeFromMemory(argMemory, argFileLength);
	assert(SUCCEEDED(hr) && "Initialize Memory Failed...");

	Microsoft::WRL::ComPtr<IWICBitmapDecoder> bitmapDecoder;
	hr = factory_->CreateDecoderFromStream(stream.Get(), nullptr, WICDecodeMetadataCacheOnLoad, &bitmapDecoder);
	assert(SUCCEEDED(hr) && "Create Decode Failed...");

	return std::move(Load(bitmapDecoder));
}

std::unique_ptr<Utility::Image> Utility::ImageFactory::Load(Microsoft::WRL::ComPtr<IWICBitmapDecoder> argBitmapDecoder)
{
	Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frameDecoder;
	HRESULT hr = argBitmapDecoder->GetFrame(0, &frameDecoder);
	assert(SUCCEEDED(hr) && "GetFrame Failed...");

	unsigned int width, height;
	hr = frameDecoder->GetSize(&width, &height);
	assert(SUCCEEDED(hr) && "GetSize Failed...");

	assert(width > 0 && height > 0 && "ImageSize 0 or less...");

	WICPixelFormatGUID pixelFormat;
	hr = frameDecoder->GetPixelFormat(&pixelFormat);
	assert(SUCCEEDED(hr) && "GetPixelFormat Failed...");

	std::unique_ptr<Utility::Image> resurt = nullptr;

	if (pixelFormat != GUID_WICPixelFormat32bppRGBA)
	{
		// 変換する
		Microsoft::WRL::ComPtr<IWICFormatConverter> formatConverter;
		hr = factory_->CreateFormatConverter(&formatConverter);
		assert(SUCCEEDED(hr) && "CreateFormatConverter Failed...");

		hr = formatConverter->Initialize(frameDecoder.Get(), GUID_WICPixelFormat32bppRGBA,
			WICBitmapDitherTypeErrorDiffusion, 0, 0, WICBitmapPaletteTypeCustom);
		assert(SUCCEEDED(hr) && "IWICFormatConverter Initialize Failed...");

		// コピー
		resurt = std::make_unique<Utility::Image>(DirectX::XMFLOAT2(static_cast<float>(width), static_cast<float>(height)), 4);
		hr = formatConverter->CopyPixels(0, resurt->Stride(), resurt->UVSize(), resurt->Ptr());
		assert(SUCCEEDED(hr) && "CopyPixels Failed... ");
	}
	else
	{
		// コピー
		resurt = std::make_unique<Utility::Image>(DirectX::XMFLOAT2(static_cast<float>(width), static_cast<float>(height)), 4);
		hr = frameDecoder->CopyPixels(0, resurt->Stride(), resurt->UVSize(), resurt->Ptr());
		assert(SUCCEEDED(hr) && "CopyPixels Failed... ");
	}
	return std::move(resurt);
}

#pragma endregion	ImageFactory

Utility::Texture::Texture(const std::wstring &argPath, ID3D11Device *argDevice)
{
	std::unique_ptr<ImageFactory> factory = std::make_unique<ImageFactory>();
	image_ = factory->Load(argPath);
	assert(image_ && "Create Image Failed...");
	Create(argDevice);
}

Utility::Texture::Texture(unsigned char *argMemory, const unsigned long argFileLength, ID3D11Device *argDevice)
{
	std::unique_ptr<ImageFactory> factory = std::make_unique<ImageFactory>();
	image_ = factory->Load(argMemory, argFileLength);
	assert(image_ && "Create Image Failed...");
	Create(argDevice);
}

Utility::Texture::Texture(std::unique_ptr<Utility::Image> &&argImage, ID3D11Device *argDevice)
{
	image_ = std::move(argImage);
	Create(argDevice);
}

Utility::Texture::Texture(ID3D11Texture2D * argTexture, ID3D11Device * argDevice)
	:texture_(argTexture)
{
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = 1;

	HRESULT hr = argDevice->CreateShaderResourceView(texture_.Get(), &SRVDesc, &srv_);
	assert(SUCCEEDED(hr) && "CreateShaderResourceView Failed...");

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = argDevice->CreateSamplerState(&samplerDesc, &sampler_);
	assert(SUCCEEDED(hr) && "CreateSamplerState Failed...");
}

Utility::Texture::~Texture() = default;

void Utility::Texture::Create(ID3D11Device *argDevice)
{
	D3D11_TEXTURE2D_DESC desc;
	desc.Width = static_cast<unsigned int>(image_->RectSize().x);
	desc.Height = static_cast<unsigned int>(image_->RectSize().y);
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = image_->Ptr();
	initData.SysMemPitch = image_->Stride();
	initData.SysMemSlicePitch = image_->UVSize();

	auto hr = argDevice->CreateTexture2D(&desc, &initData, &texture_);
	assert(SUCCEEDED(hr) && "CreateTexture2D Failed...");

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = 1;

	hr = argDevice->CreateShaderResourceView(texture_.Get(), &SRVDesc, &srv_);
	assert(SUCCEEDED(hr) && "CreateShaderResourceView Failed...");

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = argDevice->CreateSamplerState(&samplerDesc, &sampler_);
	assert(SUCCEEDED(hr) && "CreateSamplerState Failed...");
}