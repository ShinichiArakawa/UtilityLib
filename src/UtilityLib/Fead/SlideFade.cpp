/**
 *	@file	SlideFade.cpp
 *	@date	2017 / 10 / 16
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "SlideFade.h"
#include "../DeviceResources/DeviceResources.h"
#include "../ConstantBuffer.h"

#include "../Resource/SlideFadeVS.h"
#include "../Resource/SlideFadePS.h"

class Utility::SlideFade::Impl
{
private:

	struct CbFaed
	{
		DirectX::XMFLOAT2 DrawRegion;
		int padding[2];
	};

public:
	Impl(ID3D11DeviceContext * argContext, eFadeType argFadeType, Math::Vector2 argWindowSize, SlideFade *argFade)
		:fade_(argFade), windowSize_(argWindowSize)
	{
		auto device = GetD3Ddevice(argContext).Get();
		cbFaed_ = std::make_unique<ConstantBuffer<CbFaed>>(device, eShaderType::Pixel, 0);

		Reset(argFadeType);

		auto vertices = fade_->vertices_.Vertices();
		vertices.resize(fade_->VertexMax);
		vertices[0] = TextureVertex(Math::Vector3(0.f, 0.f, 0.f), Math::Vector2(0.f , 0.f));
		vertices[1] = TextureVertex(Math::Vector3(argWindowSize.x, 0.f, 0.f), Math::Vector2(1.f, 0.f));
		vertices[2] = TextureVertex(Math::Vector3(0.f, argWindowSize.y, 0.f), Math::Vector2(0.f, 1.f));
		vertices[3] = TextureVertex(Math::Vector3(argWindowSize.x, argWindowSize.y, 0.f), Math::Vector2(1.f, 1.f));
		fade_->vertices_.Vertices(vertices);
		fade_->vertices_.Create(device);

		fade_->vertexShader_ = VertexShader(argContext, &fade_->inputLayout_, reinterpret_cast<char*>(BinDataSlideFadeVS), BinSizeSlideFadeVS);
		fade_->pixelShader_ = PixelShader(argContext, reinterpret_cast<char*>(BinDataSlideFadePS), BinSizeSlideFadePS);

	};
	
	~Impl() = default;
public:

	void Reset(eFadeType argFadeType)
	{
		fade_->fadeType_ = argFadeType;
		fade_->shouldFade_ = true;
		if (fade_->fadeType_ == eFadeType::In)
		{
			cbFaed_->DrawRegion = Math::Vector2(0, 0);
			addValue_ = Math::Vector2(1.f / 60.f, 1.f / 60.f);
		}
		else
		{
			cbFaed_->DrawRegion = Math::Vector2(1, 1);
			addValue_ = Math::Vector2(-1.f / 60.f, -1.f / 60.f);
		}
	}

	bool Update() 
	{
		bool res = true;
		if (fade_->fadeType_ == eFadeType::In)
		{
			if (cbFaed_->DrawRegion.x < 1.f)
				cbFaed_->DrawRegion.x += addValue_.x;

			if (cbFaed_->DrawRegion.y < 1.f)
				cbFaed_->DrawRegion.y += addValue_.y;

			if (cbFaed_->DrawRegion.x >= 1.f &&
				cbFaed_->DrawRegion.y >= 1.f)
			{
				res = false;
			}
		}
		else
		{
			if (cbFaed_->DrawRegion.x > 0.f)
				cbFaed_->DrawRegion.x += addValue_.x;

			if (cbFaed_->DrawRegion.y > 0.f)
				cbFaed_->DrawRegion.y += addValue_.y;

			if (cbFaed_->DrawRegion.x <= 0.f &&
				cbFaed_->DrawRegion.y <= 0.f)
			{
				res = false;
			}
		}

		return res;
	}

	void Draw(ID3D11DeviceContext *argContext)
	{
		fade_->blendStateManager_->ChangeState(argContext, eBlendMode::NonPremultiplied);
		argContext->OMSetRenderTargets(1, fade_->rt_.GetAddressOf(), nullptr);
		fade_->vertexShader_.SetShader(argContext);
		fade_->pixelShader_.SetShader(argContext);

		fade_->cbTextureMatrix_->Update(argContext);
		cbFaed_->Update(argContext);
		fade_->vertices_.Update(argContext);
		argContext->PSSetSamplers(0, 1, fade_->srcTex_->Sampler().GetAddressOf());
		argContext->PSSetShaderResources(0, 1, fade_->srcTex_->Srv().GetAddressOf());

		fade_->inputLayout_.SetInputLayout(argContext);
		fade_->vertices_.SetVertexBuffers(argContext);
		argContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		argContext->Draw(fade_->VertexMax, 0);
		argContext->OMSetRenderTargets(1, fade_->rt_.GetAddressOf(), fade_->ds_.Get());
	}

private:
	std::unique_ptr<ConstantBuffer<CbFaed>> cbFaed_;
	Math::Vector2 windowSize_;
	Math::Vector2 addValue_;
	SlideFade *fade_;
};

Utility::SlideFade::SlideFade(ID3D11DeviceContext * argContext, eFadeType argFadeType, Math::Vector2 argWindowSize, std::shared_ptr<Texture> argTexture)
	:CFade(argContext, argWindowSize, argFadeType, argTexture)
{
	pImpl = std::make_unique<Impl>(argContext, argFadeType, argWindowSize, this);
}

Utility::SlideFade::~SlideFade() = default;

bool Utility::SlideFade::CrossFade()
{
	bool res = Update();
	if (!res && shouldFade_)
	{
		eFadeType fadeT = (fadeType_ == eFadeType::In) ? eFadeType::Out : eFadeType::In;
		Reset(fadeT);
		shouldFade_ = false;
		res = true;
	}
	return res;
}

void Utility::SlideFade::Reset(eFadeType argFadeType)
{
	pImpl->Reset(argFadeType);
}

bool Utility::SlideFade::Update()
{
	return pImpl->Update();
}

void Utility::SlideFade::Draw(ID3D11DeviceContext * argContext)
{
	pImpl->Draw(argContext);
}
