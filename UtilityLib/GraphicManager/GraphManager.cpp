/**
 *	@file   GraphManager.cpp
 *	@date   2017 / 08 / 31
 *	@author Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */

#include "GraphManager.h"

Utility::GraphManager::GraphManager() = default;
Utility::GraphManager::~GraphManager() = default;

void Utility::GraphManager::Initialize(ID3D11DeviceContext *argContext, IDXGISwapChain *argSwapChain)
{
	graphic3d_ = std::make_unique<Graphic3DBase>();
	auto spriteBatch = std::make_unique<SpriteBatch>(argContext);
	graphic2d_ = std::make_unique<Graphic2DBase>(argContext, argSwapChain, std::move(spriteBatch));
}

void Utility::GraphManager::DrawAndWriteText(const std::wstring argStr, DirectX::XMFLOAT2 argPos)const
{
	graphic2d_->DrawAndWriteText(argStr, argPos);
}

void Utility::GraphManager::ConfigTextColor(D2D1::ColorF argColor)
{
	graphic2d_->ConfigTextColor(argColor);
}

void Utility::GraphManager::ConfigTextSize(float argStrSize)
{
	graphic2d_->ConfigTextSize(argStrSize);
}

Utility::Graphic2DBase *Utility::GraphManager::AddAndLoadImage(const std::wstring argTextureFilename)
{
	return graphic2d_->AddAndLoadImage(argTextureFilename);
}

Utility::Graphic3DBase *Utility::GraphManager::AddAndLoadModel(const std::string argModelFilename)
{	
	return graphic3d_->AddAndLoadModel(argModelFilename);
}

#pragma region Accessor

std::shared_ptr<Utility::Texture> Utility::GraphManager::FindIndexTexture(const std::wstring argKey)const
{
	return graphic2d_->FindIndexTexture(argKey);
}
ID2D1RenderTarget *Utility::GraphManager::RenderTarget()const
{
	return graphic2d_->RenderTarget2d();
}
void Utility::GraphManager::ChangeRt2dDepth(eRtDepth argType)
{
	graphic2d_->ChangeRt2dDepth(argType);
}
void Utility::GraphManager::ChangeRt2dDepth(int argType)
{
	graphic2d_->ChangeRt2dDepth(static_cast<eRtDepth>(argType));
}
Utility::SpriteBatch *Utility::GraphManager::GetSpriteBatch() const
{
	return graphic2d_->GetSpriteBatch();
}

Utility::Graphic2DBase * Utility::GraphManager::Graphic2D() const
{
	return graphic2d_.get();
}

Utility::Graphic3DBase * Utility::GraphManager::Graphic3D() const
{
	return graphic3d_.get();
}
void Utility::GraphManager::CanUseArchive(bool argCanUseArchive)
{
	graphic2d_->CanUseArchive(argCanUseArchive);
}

Utility::DWriteBase *Utility::GraphManager::DirectWrite()const
{
	return graphic2d_->DirectWrite();
}

#pragma endregion Accessor
