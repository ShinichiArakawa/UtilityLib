/**
 *	@file	SpriteBatch.cpp
 *	@brief	テクスチャの描画クラス
 *	@date	2017 / 8 / 27
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 *	@note	http://marupeke296.com/NGDV_No3_Sprite.html
 */
#include "SpriteBatch.h"

#include "../Shader/ShaderSummary.h"
#include "../../ConstantBuffer.h"
#include "../TextureImage/TextureImage.h"
#include "../../Math/Math.h"
#include "../../DeviceResources/DeviceResources.h"
#include "../BlendStateManager/BlendStateManager.h"
#include "../../Window/Window.h"
#include "../../String/String.h"
#include "../../Singleton/Singleton.h"
#include "../BufferObjectStructure.h"
#include "../../Resource/SpriteBatchVS.h"
#include "../../Resource/SpriteBatchPS.h"

#include <wrl/client.h>

class Utility::SpriteEffect::Impl
{
public:
	Impl(Sprite *argSprite)
		:sprite_(argSprite)
	{
		life_ = 240.0f;

		(useAlpha_) ? state_ = std::make_unique<FadeIn>() : state_ = std::make_unique<Idle>();
	}
	~Impl() = default;

public:
	class SpriteState
	{
	public: virtual bool Update(Impl* task) = 0;
	};

public:
#if 1
	class FadeIn : public SpriteState
	{
		bool isUpdated_;
		float target_, cur_;
	public:
		FadeIn()
			: isUpdated_(false), target_(0.f), cur_(0.f)
		{
		}
		bool Update(Impl* task)
		{
			bool result = false;
			if (!isUpdated_)
			{
				target_ = task->sprite_->Color.w;
				task->sprite_->Color.w = 0.f;
				isUpdated_ = true;
			}
			cur_ += 0.01f;

			if (cur_ >= target_)
				cur_ = target_;

			task->sprite_->Color.w = cur_;

			if (cur_ >= target_)
			{
				delete this;
				task->state_ = std::make_unique<Idle>();
			}
			return result;
		}
	};//	class FadeIn : public SpriteState

	class Idle : public SpriteState
	{
	public:
		bool Update(Impl* task)
		{
			bool result = false;
			task->life_ -= 1.0f;
			if (task->life_ <= 0.0f)
			{
				delete this;
				(task->useAlpha_) ? task->state_.reset(new FadeOut) : result = true;
			}

			return result;
		}
	};//	class Idle : public SpriteState

	class FadeOut : public SpriteState
	{
	public:
		bool Update(Impl* task)
		{
			float alpha = task->sprite_->Color.w;
			alpha -= 0.01f;
			if (alpha <= 0.0f)
			{
				delete this;
				return true;
			}
			task->sprite_->Color.w = alpha;
			return false;
		}
	};//	class FadeOut : public SpriteState

#endif
private:

	float life_;
	bool useAlpha_;
	std::unique_ptr<SpriteState> state_;
	Sprite *sprite_;

};//	class SpriteEffect::Impl

Utility::SpriteEffect::SpriteEffect(Sprite *argSprite)
	:pImpl(std::make_unique<Impl>(argSprite))
{
}

Utility::SpriteEffect::~SpriteEffect() = default;

class Utility::SpriteBatch::Impl
{
#pragma region	Structures
private:

	struct TextureInfo
	{
	public:
		DirectX::XMFLOAT2 UVPos;
		DirectX::XMFLOAT2 UVSize;
		DirectX::XMFLOAT4 Color;
	public:
		TextureInfo()
			:UVPos(DirectX::XMFLOAT2()), UVSize(DirectX::XMFLOAT2()), Color(DirectX::XMFLOAT4())
		{
			static_assert((sizeof(TextureInfo) % 16) == 0, "CB must be 16-byte aligned");
		}
	};//	struct TextureInfo
#pragma endregion	Structures

public:
	Impl(ID3D11DeviceContext *argImmediateContext, DirectX::XMFLOAT2 argScreenSize)
		:context_(argImmediateContext), currentBlned_(eBlendMode::NonPremultiplied), screenSize_(argScreenSize),
		blendStateManager_(Singleton<BlendStateManager>::Get())
	{
		blendStateManager_->Initialize(argImmediateContext);
		context_->OMGetRenderTargets(1, rt_.GetAddressOf(), ds_.GetAddressOf());
	
		vertexShader_ = VertexShader(argImmediateContext, &inputLayout_, reinterpret_cast<char*>(BinDataSpriteBatchVS), BinSizeSpriteBatchVS);
		pixelShader_ = PixelShader(argImmediateContext, reinterpret_cast<char*>(BinDataSpriteBatchPS), BinSizeSpriteBatchPS);

		ID3D11Device* device = Utility::GetD3Ddevice(context_.Get()).Get();
		CreateTexturepolygon(device);

	}//	Impl
	~Impl() = default;

private:
	//	テクスチャ用ポリゴンの作成
	void CreateTexturepolygon(ID3D11Device *argDevice)
	{
		cbTextureMatrix_ = std::make_unique<ConstantBuffer<TextureMatrix>>(argDevice, eShaderType::Vertex, 0);
		//	ポリゴンの原点をウィンドウの左上に指定する
		DirectX::XMMATRIX proj = DirectX::XMMatrixScaling(2 / screenSize_.x, -2 / screenSize_.y, 1.f);
		proj = proj * DirectX::XMMatrixTranslation(-1.f, 1.f, 0.f);

		DirectX::XMStoreFloat4x4(&cbTextureMatrix_->Proj, proj);

		vbTextureVertices_ = VertexBuffer<TextureVertex>(argDevice, VertexMax);
		
		vbTextureVertices_[0]->Pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		vbTextureVertices_[1]->Pos = DirectX::XMFLOAT3(screenSize_.x, 0.f, 0.f);
		vbTextureVertices_[2]->Pos = DirectX::XMFLOAT3(0.f, screenSize_.y, 0.f);
		vbTextureVertices_[3]->Pos = DirectX::XMFLOAT3(screenSize_.x, screenSize_.y, 0.f);

		vbTextureVertices_[0]->UV = DirectX::XMFLOAT2(0.f, 0.f);
		vbTextureVertices_[1]->UV = DirectX::XMFLOAT2(1.f, 0.f);
		vbTextureVertices_[2]->UV = DirectX::XMFLOAT2(0.f, 1.f);
		vbTextureVertices_[3]->UV = DirectX::XMFLOAT2(1.f, 1.f);

		cbTextureInfo_ = std::make_unique<ConstantBuffer<TextureInfo>>(argDevice, eShaderType::Vertex, 1);
	}//	void CreateTexturepolygon

public:

	// Adds a single sprite to the queue.
	void Entry(const std::weak_ptr<Texture> &argTexture, DirectX::FXMVECTOR argPos, RECT const* argUVRect, DirectX::FXMVECTOR argColor,
		float argRotation, DirectX::FXMVECTOR argOrigin, DirectX::GXMVECTOR argScale)
	{
		if (argTexture.expired())
			return;

		std::shared_ptr<Texture> tex = argTexture.lock();
		Sprite sprite(tex);

		DirectX::XMFLOAT4 pos;
		DirectX::XMStoreFloat4(&pos, argPos);
		DirectX::XMFLOAT4 pivotPos;
		DirectX::XMStoreFloat4(&pivotPos, argOrigin);

		DirectX::XMFLOAT4 scale;
		DirectX::XMStoreFloat4(&scale, argScale);
		
		sprite.PolygonSize = DirectX::XMFLOAT2(static_cast<float>(argUVRect->right - argUVRect->left) / screenSize_.x, static_cast<float>(argUVRect->bottom - argUVRect->top) / screenSize_.y);

		sprite.Pos = DirectX::XMFLOAT3(pos.x, pos.y, pos.z);
		sprite.PivotPos = DirectX::XMFLOAT2(pivotPos.x, pivotPos.y);
		sprite.UvPos = DirectX::XMFLOAT2(static_cast<float>(argUVRect->left), static_cast<float>(argUVRect->top));
		sprite.UvSize = DirectX::XMFLOAT2(static_cast<float>(argUVRect->right), static_cast<float>(argUVRect->bottom));
		sprite.Scale = DirectX::XMFLOAT2(scale.x, scale.y);
		sprite.Radian = argRotation;

		DirectX::XMStoreFloat4(&sprite.Color, argColor);

		drawList_[currentBlned_].push_back(std::move(sprite));
	}//	void Entry

	void Draw()
	{
		auto context = context_.Get();

		context->OMSetRenderTargets(1, rt_.GetAddressOf(), nullptr);
		vertexShader_.SetShader(context);
		pixelShader_.SetShader(context);


		for (int lBlendIndex = 0; lBlendIndex < eBlendMode::Max; lBlendIndex++)
		{
			if (drawList_[lBlendIndex].empty())
				continue;

			blendStateManager_->ChangeState(context, lBlendIndex);
			
			for (const auto &sprite : drawList_[lBlendIndex])
			{
				DirectX::XMMATRIX world = DirectX::XMMatrixScaling(sprite.PolygonSize.x, sprite.PolygonSize.y, 1.f);
				DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(sprite.Scale.x, sprite.Scale.y, 1.f);
				DirectX::XMMATRIX rot = DirectX::XMMatrixRotationZ(sprite.Radian);

				world = world * DirectX::XMMatrixTranslation(-sprite.PivotPos.x, -sprite.PivotPos.y, 0.f);
				world = world * scale * rot;
				world = world * DirectX::XMMatrixTranslation(sprite.Pos.x, sprite.Pos.y, 0.f);

				DirectX::XMStoreFloat4x4(&cbTextureMatrix_->World, world);

				const Math::Vector2 Size = sprite.Tex->ImageSize();

				cbTextureInfo_->UVPos.x = (sprite.UvPos.x > 0.f) ? sprite.UvPos.x / Size.x : 1.f;
				cbTextureInfo_->UVPos.y = (sprite.UvPos.y > 0.f) ? sprite.UvPos.y / Size.y : 1.f;

				cbTextureInfo_->UVSize = DirectX::XMFLOAT2((sprite.UvSize.x - sprite.UvPos.x) / Size.x, (sprite.UvSize.y - sprite.UvPos.y) / Size.y);
				cbTextureInfo_->Color = sprite.Color;

				cbTextureMatrix_->Update(context);
				cbTextureInfo_->Update(context);
				vbTextureVertices_.Update(context);

				context_->PSSetShaderResources(0, 1, sprite.Tex->Srv().GetAddressOf());
				context_->PSSetSamplers(0, 1, sprite.Tex->Sampler().GetAddressOf());

				inputLayout_.SetInputLayout(context);
				vbTextureVertices_.SetVertexBuffers(context);
				context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
				context_->Draw(VertexMax, 0);

			}//	foreach(drawList_)

			drawList_[lBlendIndex].clear();
		}//	foreach(eBlendMode::Max)

		blendStateManager_->ChangeState(context, eBlendMode::NonPremultiplied);
		context_->OMSetRenderTargets(1, rt_.GetAddressOf(), ds_.Get());

	}//	void Draw()

private:
	const int VertexMax = 4;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		context_;
	std::vector<Sprite>								drawList_[eBlendMode::Max];	//	!<	描画対象リスト
	VertexShader									vertexShader_;
	PixelShader										pixelShader_;
	InputLayout										inputLayout_;
	VertexBuffer<TextureVertex>						vbTextureVertices_;
	std::unique_ptr<ConstantBuffer<TextureMatrix>>	cbTextureMatrix_;
	std::unique_ptr<ConstantBuffer<TextureInfo>>	cbTextureInfo_;
	DirectX::XMFLOAT2								screenSize_;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rt_;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> ds_;
	BlendStateManager* blendStateManager_;
public:
	eBlendMode										currentBlned_;

};//	class SpriteBatch::Impl

Utility::SpriteBatch::SpriteBatch(ID3D11DeviceContext * argImmediateContext)
{
	assert(Window::Get() && "Not Create Window...");
	RECT rect = {};
	const BOOL Result = GetClientRect(Window::Get()->WindowHundle(), &rect);
	assert(Result && "GetClientRect Failed...");

	const auto ScreenSize = DirectX::XMFLOAT2(static_cast<float>(rect.right - rect.left), static_cast<float>(rect.bottom - rect.top));
	pImpl = std::make_unique<Impl>(argImmediateContext, ScreenSize);
}

Utility::SpriteBatch::SpriteBatch(ID3D11DeviceContext *argImmediateContext, DirectX::XMFLOAT2 argScreenSize)
	:pImpl(std::make_unique<Impl>(argImmediateContext, argScreenSize))
{
}

Utility::SpriteBatch::~SpriteBatch() = default;

void Utility::SpriteBatch::ChangeBlendMode(Utility::eBlendMode argBlendMode)
{
	pImpl->currentBlned_ = argBlendMode;
}

void Utility::SpriteBatch::Draw()
{
	pImpl->Draw();
}
/**
 *	@fn			Entry
 *	@brief		座標、色、指定で描画リストへの登録（XMFLOAT.ver）
 *	@param[in]	argTexture	!<	描画したいテクスチャ
 *	@param[in]	argPosition	!<	描画する座標
 *	@param[in]	argColor	!<	テクスチャの色
 */
void Utility::SpriteBatch::Entry(const std::weak_ptr<Texture> &argTexture, DirectX::XMFLOAT2 const& argPosition, DirectX::FXMVECTOR argColor)
{
	RECT UvRect = { 0, 0, static_cast<long>(argTexture._Get()->ImageSize().x), static_cast<long>(argTexture._Get()->ImageSize().y) };
	pImpl->Entry(argTexture, DirectX::XMVectorSet(argPosition.x, argPosition.y, 0.f, 0.f), &UvRect, argColor, 0.f, DirectX::g_XMZero, DirectX::g_XMOne);
}
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
void Utility::SpriteBatch::Entry(const std::weak_ptr<Texture> &argTexture, DirectX::XMFLOAT2 const& argPosition, RECT const* argUVRect, DirectX::FXMVECTOR argColor,
	float argRotation, DirectX::XMFLOAT2 const& argOrigin, float argScale, float argLayerDepth)
{
	pImpl->Entry(argTexture, DirectX::XMVectorSet(argPosition.x, argPosition.y, 0.f, 0.f), argUVRect, argColor, argRotation, DirectX::XMLoadFloat2(&argOrigin), DirectX::XMVectorSet(argScale, argScale, 0.f, 0.f));
}
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
void Utility::SpriteBatch::Entry(const std::weak_ptr<Texture> &argTexture, DirectX::XMFLOAT2 const& argPosition, RECT const* argUVRect, DirectX::FXMVECTOR argColor,
	float argRotation, DirectX::XMFLOAT2 const& argOrigin, DirectX::XMFLOAT2 const& argScale, float argLayerDepth)
{
	pImpl->Entry(argTexture, DirectX::XMVectorSet(argPosition.x, argPosition.y, 0.f, 0.f), argUVRect, argColor, argRotation, DirectX::XMLoadFloat2(&argOrigin), DirectX::XMLoadFloat2(&argScale));
}
/**
 *	@fn			Entry
 *	@brief		座標、色、指定で描画リストへの登録（XMVECTOR.ver）
 *	@param[in]	argTexture	!<	描画したいテクスチャ
 *	@param[in]	argPosition	!<	描画する座標
 *	@param[in]	argColor	!<	テクスチャの色
 */
void Utility::SpriteBatch::Entry(const std::weak_ptr<Texture> &argTexture, DirectX::FXMVECTOR argPosition, DirectX::FXMVECTOR argColor)
{
	pImpl->Entry(argTexture, argPosition, nullptr, argColor, 0.f, DirectX::g_XMZero, DirectX::g_XMOne);
}
/**
 *	@fn			Entry
 *	@brief		座標、色、角度、中心座標、大きさ（縦横同じ）、深度、指定で描画リストへの登録（XMVECTOR.ver）
 *	@param[in]	argTexture		!<	描画したいテクスチャ
 *	@param[in]	argPosition		!<	描画する座標
 *	@param[in]	argColor		!<	テクスチャの色
 *	@param[in]	argRotation		!<	描画角度
 *	@param[in]	argOrigin		!<	テクスチャの中心座標
 *	@param[in]	argScale		!<	テクスチャの大きさ
 *	@param[in]	argLayerDepth	!<	レイヤーの深度
 */
void Utility::SpriteBatch::Entry(const std::weak_ptr<Texture> &argTexture, DirectX::FXMVECTOR argPosition, RECT const* argUVRect, DirectX::FXMVECTOR argColor,
	float argRotation, DirectX::FXMVECTOR argOrigin, float argScale, float argLayerDepth)
{
	pImpl->Entry(argTexture, argPosition, argUVRect, argColor, argRotation, argOrigin, DirectX::XMVectorSet(argScale, argScale, 0.f, 0.f));
}
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
void Utility::SpriteBatch::Entry(const std::weak_ptr<Texture> &argTexture, DirectX::FXMVECTOR argPosition, RECT const* argUVRect, DirectX::FXMVECTOR argColor,
	float argRotation, DirectX::FXMVECTOR argOrigin, DirectX::GXMVECTOR argScale, float argLayerDepth)
{
	pImpl->Entry(argTexture, argPosition, argUVRect, argColor, argRotation, argOrigin, argScale);
}
