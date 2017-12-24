/**
 *	@file	RippleFade.cpp
 *	@date	2017 / 10 / 16
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "RippleFade.h"
#include "../DeviceResources/DeviceResources.h"
#include "../ConstantBuffer.h"

#include "../Resource/RippleFadeVS.h"
#include "../Resource/RippleFadePS.h"

class Utility::RippleFade::Impl
{
private:

	struct CbFaed
	{
		DirectX::XMFLOAT2 Center;
		float Speed;
		float MoveUvPower;

		float WaveNumPower;
		float Alpha;
		float AlphaPower;
		int padding;
	};


public:
	Impl(ID3D11DeviceContext * argContext, eFadeType argFadeType, Math::Vector2 argWindowSize, RippleFade *argFade)
		:maxTime_(180), time_(0), fade_(argFade)
	{
		auto device = GetD3Ddevice(argContext).Get();
		cbFaed_ = std::make_unique<ConstantBuffer<CbFaed>>(device, eShaderType::Pixel, 0);

		Math::Vector2 center = Math::Vector2(0.5f, 0.5f);
		Math::Vector2 pos = Math::Vector2(0, 0);
		pos.x = (center.x >= 0.5f) ? center.x : 1.f - center.x;
		pos.y = (center.y >= 0.5f) ? center.y : 1.f - center.y;

		const float AlphaPower = 10.f, MoveUvPower = 150.f;
		maxAlpha_ = (sqrt(pos.x * pos.x + pos.y * pos.y) * AlphaPower + 1.f);
		maxMoveUvPower_ = MoveUvPower * (1.f / argWindowSize.x);

		cbFaed_->WaveNumPower = 20.f;
		cbFaed_->AlphaPower = AlphaPower;
		cbFaed_->Center = center;

		Reset(argFadeType);
		
		const Math::Vector2 AddTex = Math::Vector2(0.5f / argWindowSize.x, 0.5f / argWindowSize.y);

		auto vertices = fade_->vertices_.Vertices();
		vertices.resize(fade_->VertexMax);
		vertices[0] = TextureVertex(Math::Vector3(0.f, 0.f, 1.f), AddTex + Math::Vector2(0.f , 0.f));
		vertices[1] = TextureVertex(Math::Vector3(argWindowSize.x, 0.f, 1.f), AddTex + Math::Vector2(1.f, 0.f));
		vertices[2] = TextureVertex(Math::Vector3(0.f, argWindowSize.y, 1.f), AddTex + Math::Vector2(0.f, 1.f));
		vertices[3] = TextureVertex(Math::Vector3(argWindowSize.x, argWindowSize.y, 1.f), AddTex + Math::Vector2(1.f, 1.f));
		fade_->vertices_.Vertices(vertices);
		fade_->vertices_.Create(device);

		fade_->vertexShader_ = VertexShader(argContext, &fade_->inputLayout_, reinterpret_cast<char*>(BinDataRippleFadeVS), BinSizeRippleFadeVS);
		fade_->pixelShader_ = PixelShader(argContext, reinterpret_cast<char*>(BinDataRippleFadePS), BinSizeRippleFadePS);

	};
	
	~Impl() = default;
public:
	void Reset(eFadeType argFadeType)
	{
		time_ = 0;
		cbFaed_->Speed = 0.15f / Math::PI;
		if (argFadeType == eFadeType::In)
		{
			cbFaed_->Alpha = 0.f;
			addAlpha_ = maxAlpha_ / static_cast<float>(maxTime_);

			cbFaed_->MoveUvPower = maxMoveUvPower_;
			addMoveUvPower_ = -maxMoveUvPower_ / static_cast<float>(maxTime_);
		}
		else
		{
			cbFaed_->Alpha = maxAlpha_;
			addAlpha_ = -maxAlpha_ / static_cast<float>(maxTime_);

			cbFaed_->MoveUvPower = 0.f;
			addMoveUvPower_ = maxMoveUvPower_ / static_cast<float>(maxTime_);
		}
	}

	bool Update() 
	{
		bool res = true;

		cbFaed_->Speed *= time_;

		cbFaed_->MoveUvPower += addMoveUvPower_;
		if (cbFaed_->MoveUvPower < 0.f)
			cbFaed_->MoveUvPower = 0.f;
		if (cbFaed_->MoveUvPower > maxMoveUvPower_)
			cbFaed_->MoveUvPower = maxMoveUvPower_;

		cbFaed_->Alpha += addAlpha_;
		if (cbFaed_->Alpha < 0.f)
			cbFaed_->Alpha = 0.f;
		if (cbFaed_->Alpha > maxAlpha_)
			cbFaed_->Alpha = maxAlpha_;

		if (++time_ >= maxTime_)
			res = false;
		
		return res;
	}

	void Draw(ID3D11DeviceContext * argContext)
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
	RippleFade *fade_;
	std::unique_ptr<ConstantBuffer<CbFaed>> cbFaed_;
	float addMoveUvPower_;
	float maxMoveUvPower_;
	float addAlpha_;
	float maxAlpha_;
	int time_, maxTime_;
};

Utility::RippleFade::RippleFade(ID3D11DeviceContext * argContext, eFadeType argFadeType, Math::Vector2 argWindowSize, std::shared_ptr<Texture> argTexture)
	:CFade(argContext, argWindowSize, argFadeType, argTexture)
{
	pImpl = std::make_unique<Impl>(argContext, argFadeType, argWindowSize, this);
}

Utility::RippleFade::~RippleFade() = default;

bool Utility::RippleFade::CrossFade()
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

void Utility::RippleFade::Reset(eFadeType argFadeType)
{
	return pImpl->Reset(argFadeType);
}

bool Utility::RippleFade::Update()
{
	return pImpl->Update();
}

void Utility::RippleFade::Draw(ID3D11DeviceContext * argContext)
{
	return pImpl->Draw(argContext);
}
