/**
 *	@file	CFade.cpp
 *	@date	2017 / 10 / 16
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "CFade.h"

#include "../DeviceResources/DeviceResources.h"
#include "../ConstantBuffer.h"
#include "../Singleton/Singleton.h"
#include "../String/String.h"

Utility::CFade::CFade(ID3D11DeviceContext * argContext, Math::Vector2 argWindowSize, eFadeType argFadeType, std::shared_ptr<Texture> argTexture)
	:blendStateManager_(Singleton<BlendStateManager>::Get()), srcTex_(argTexture), fadeType_(argFadeType), shouldFade_(true)
{
	auto device = GetD3Ddevice(argContext).Get();

	cbTextureMatrix_ = std::make_unique<ConstantBuffer<TextureMatrix>>(device, eShaderType::Vertex, 0);
	//	ポリゴンの原点をウィンドウの左上に指定する
	DirectX::XMMATRIX proj = DirectX::XMMatrixScaling(2 / argWindowSize.x, -2 / argWindowSize.y, 1.f);
	proj = proj * DirectX::XMMatrixTranslation(-1.f, 1.f, 0.f);

	DirectX::XMStoreFloat4x4(&cbTextureMatrix_->World, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&cbTextureMatrix_->Proj, proj);

	blendStateManager_->Initialize(argContext);
	argContext->OMGetRenderTargets(1, rt_.GetAddressOf(), ds_.GetAddressOf());

}

Utility::CFade::~CFade() = default;