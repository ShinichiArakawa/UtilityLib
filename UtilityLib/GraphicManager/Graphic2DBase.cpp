/**
*	@file   Graphic2DBase.cpp
*	@date   2017 / 08 / 31
*	@author Katsumi Takei
*	Copyright (c) Kastumi Takei. All rights reserved.
*/
#include "Graphic2DBase.h"
#include "../DeviceResources/DeviceResources.h"
#include "../Window/Window.h"
#include "../Function.h"

#include "../Archive/Archive.h"
#include "../String/String.h"
#include "GraphManager.h"
#include "../Singleton/Singleton.h"

#include <map>
#include <wrl/client.h>


class Utility::Graphic2DBase::Impl
{
public:

	Impl(ID3D11DeviceContext *argContext, IDXGISwapChain *argSwapChain, std::unique_ptr<SpriteBatch> &&argSpriteBatch)
		:nextLoadTextureName_(L""), spriteBatch_(std::move(argSpriteBatch)), rt2dDepth_(eRtDepth::Front), canUseArchive_(false),
		context_(argContext)
	{
		InitializeColorBrush(argSwapChain);
		InitializeTextFormat();
	}

	~Impl() = default;
private:

	/**
	*	@fn		InitializeColorBrush
	*	@brief	Direct2Dのテキスト色の設定
	*/
	void InitializeColorBrush(IDXGISwapChain *argSwapChain)
	{
		direct2D_ = std::make_unique<Direct2DBase>(argSwapChain);

		const auto BrushColor = D2D1::ColorF(1, 1, 1, 1);

		/**
		*	@fn		CreateSolidColorBrush
		*	@brief	文字色のためのブラシ作成
		*	@param[in]	!<	色（r, b, g, a）
		*	@param[in]	!<	設定したいブラシ
		*/
		//	HACK	:	ブラシを2本にするかも
		ID2D1RenderTarget *front = direct2D_->RenderTarget(eRtDepth::Front);
		front->CreateSolidColorBrush(BrushColor, brushWhite_.GetAddressOf());
	}
	/**
	*	@fn		InitializeTextFormat
	*	@brief	テキスト形式の設定
	*/
	void InitializeTextFormat()
	{
		directWrite_ = std::make_unique<DWriteBase>();
		IDWriteFactory *directWriteFactory = directWrite_->ImageFactory();

		const auto StrSize = 20.f;

		/**
		*	@fn		CreateTextFormat
		*	@brief	フォントの作成
		*	@param[in]	!<	フォント名   ""の場合は、デフォルトのフォントが選ばれる
		*	@param[in]	!<	フォントファミリー  nullptrの場合は、システムフォントファミリーが使われる
		*	@param[in]	!<	文字の太さ
		*	@param[in]	!<	文字のスタイル
		*	@param[in]	!<	文字の伸縮
		*	@param[in]	!<	文字のサイズ
		*	@param[in]	!<	ロケール名
		*	@param[in]	!<	テキストフォーマット
		*/
		directWriteFactory->CreateTextFormat(
			L"",
			nullptr,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			StrSize,
			L"japanese",
			textFormat_.GetAddressOf());
	}

	std::unique_ptr<Texture> LoadTexture(const char * argFileName, ID3D11Device *argDevice)
	{
		char* binData = nullptr;
		int length = 0;
		Singleton<Archive>::Get()->Read(argFileName, &binData, &length);

		auto memoryData = reinterpret_cast<unsigned char *>(binData);
		return std::move(std::make_unique<Texture>(memoryData, length, argDevice));
	}

#pragma region		Extension
public:
	void Png()
	{
		assert(nextLoadTextureName_ != L"" && "拡張子をつけ忘れています");
		const std::wstring TexPath = imagePath_ + nextLoadTextureName_ + L".png";
		auto device = GetD3Ddevice(context_.Get()).Get();
		std::shared_ptr<Texture> texture = (canUseArchive_) ? LoadTexture(String::ToString(TexPath).c_str(), device) : std::make_shared<Texture>(TexPath, device);

		textures_.insert(std::map<const std::wstring, std::shared_ptr<Texture>>::value_type(nextLoadTextureName_, texture));
		nextLoadTextureName_ = L"";
	}
	void Tex()
	{
		assert(nextLoadTextureName_ != L"" && "拡張子をつけ忘れています");
		const std::wstring TexPath = nextLoadTextureName_;
		auto device = GetD3Ddevice(context_.Get()).Get();
		std::shared_ptr<Texture> texture = (canUseArchive_) ? LoadTexture(String::ToString(TexPath).c_str(), device) : std::make_shared<Texture>(TexPath, device);

		textures_.insert(std::map<const std::wstring, std::shared_ptr<Texture>>::value_type(nextLoadTextureName_, texture));
		nextLoadTextureName_ = L"";
	}

#pragma endregion	Extension
private:
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context_;
public:
	std::map<const std::wstring,
	std::shared_ptr<Texture>>						textures_;				//	!<	テクスチャキャッシュ
	std::wstring									nextLoadTextureName_;	//	!<	次にロードするテクスチャ名
	std::unique_ptr<Direct2DBase>					direct2D_;				//	!<	2Dグラフィックス用リソース
	std::unique_ptr<DWriteBase>						directWrite_;			//	!<	テキストの描画用リソース
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>	brushWhite_;			//	!<	テキストの色
	Microsoft::WRL::ComPtr<IDWriteTextFormat>		textFormat_;			//	!<	テキスト形式
	std::unique_ptr<SpriteBatch>					spriteBatch_;			//	!<	テクスチャ描画バッチ
	std::wstring									imagePath_;
	eRtDepth										rt2dDepth_;
	bool											canUseArchive_;

};//	Impl

Utility::Graphic2DBase::Graphic2DBase(ID3D11DeviceContext *argContext, IDXGISwapChain *argSwapChain, std::unique_ptr<SpriteBatch> &&argSpriteBatch)
	:pImpl(std::make_unique<Impl>(argContext, argSwapChain, std::move(argSpriteBatch)))
{
}

Utility::Graphic2DBase::~Graphic2DBase() = default;

void Utility::Graphic2DBase::CanUseArchive(bool argCanUseArchive)
{
	pImpl->canUseArchive_ = argCanUseArchive;
}

Utility::Graphic2DBase *Utility::Graphic2DBase::AddAndLoadImage(const std::wstring argTextureFilename)
{
	assert(pImpl->nextLoadTextureName_ == L"" && "拡張子をつけ忘れています");
	pImpl->nextLoadTextureName_ = argTextureFilename;

	return this;
}

std::shared_ptr<Utility::Texture> Utility::Graphic2DBase::FindIndexTexture(const std::wstring argKey)const
{
	assert(pImpl->nextLoadTextureName_ == L"" && "拡張子をつけ忘れています");
	return (pImpl->textures_.count(argKey) != 0) ? pImpl->textures_.at(argKey) : nullptr;
}
/**
*	@fn			ConfigTextColor
*	@brief		テキストブラシの色設定
*	@param[in]	argColor	!<	変更させたいテキストの色
*/
void Utility::Graphic2DBase::ConfigTextColor(D2D1::ColorF argColor)
{
	pImpl->brushWhite_->SetColor(argColor);
}
/**
*	@fn			ConfigTextSize
*	@brief		テキストのサイズ設定
*	@param[in]	argColor	!<	変更させたいテキストのサイズ
*/
void Utility::Graphic2DBase::ConfigTextSize(float argStrSize)
{
	pImpl->directWrite_->ImageFactory()->CreateTextFormat(
		L"",
		nullptr,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		argStrSize,
		L"japanese",
		pImpl->textFormat_.GetAddressOf());
}
/**
*	@fn			DrawAndWriteText
*	@brief		テキストの描画
*	@param[in]	argStr	!<	書き込むテキスト
*	@param[in]	argPos	!<	テキストを描画する座標
*/
void Utility::Graphic2DBase::DrawAndWriteText(const std::wstring argStr, DirectX::XMFLOAT2 argPos)const
{
	pImpl->direct2D_->RenderTarget(pImpl->rt2dDepth_)->DrawTextW(
		argStr.c_str(),
		argStr.size(),
		pImpl->textFormat_.Get(),
		D2D1::RectF(argPos.x, argPos.y, static_cast<FLOAT>(Window::Get()->WindowRect().right - Window::Get()->WindowRect().left), static_cast<FLOAT>(Window::Get()->WindowRect().bottom - Window::Get()->WindowRect().top)),
		pImpl->brushWhite_.Get(),
		D2D1_DRAW_TEXT_OPTIONS_CLIP);
}
/**
*	@fn			DrawAndWriteText
*	@brief		テキストの描画
*	@param[in]	argStr			!<	書き込むテキスト
*	@param[in]	argPos			!<	テキストを描画する座標
*	@param[in]	argScreenSize	!<	スクリーンサイズ
*/
void Utility::Graphic2DBase::DrawAndWriteText(const std::wstring argStr, DirectX::XMFLOAT2 argPos, DirectX::XMFLOAT2 argScreenSize)const
{
	pImpl->direct2D_->RenderTarget(pImpl->rt2dDepth_)->DrawTextW(
		argStr.c_str(),
		argStr.size(),
		pImpl->textFormat_.Get(),
		D2D1::RectF(argPos.x, argPos.y, argScreenSize.x, argScreenSize.y),
		pImpl->brushWhite_.Get(),
		D2D1_DRAW_TEXT_OPTIONS_CLIP);
}

void Utility::Graphic2DBase::Png()
{
	pImpl->Png();
}
void Utility::Graphic2DBase::Tex()
{
	pImpl->Tex();
}

Utility::SpriteBatch *Utility::Graphic2DBase::GetSpriteBatch()const
{
	return pImpl->spriteBatch_.get();
}
Utility::DWriteBase *Utility::Graphic2DBase::DirectWrite()const
{
	return pImpl->directWrite_.get();
}
ID2D1RenderTarget *Utility::Graphic2DBase::RenderTarget2d(eRtDepth argType)const
{
	return pImpl->direct2D_->RenderTarget(argType);
}

ID2D1RenderTarget * Utility::Graphic2DBase::RenderTarget2d() const
{
	return pImpl->direct2D_->RenderTarget(pImpl->rt2dDepth_);
}

void Utility::Graphic2DBase::ImagePath(std::wstring argPath) const
{
	pImpl->imagePath_ = argPath;
}

std::wstring Utility::Graphic2DBase::ImagePath()
{
	return pImpl->imagePath_;
}

void Utility::Graphic2DBase::ChangeRt2dDepth(eRtDepth argType)
{
	pImpl->rt2dDepth_ = argType;
}

#pragma region SpriteBatchDraw

namespace
{
	const DirectX::XMVECTOR White = DirectX::XMVectorSet(1.f, 1.f, 1.f, 1.f);
	bool CanDrawTexture(const std::weak_ptr<Utility::Texture> &argTexture, DirectX::XMFLOAT2 argSize)
	{
		if (argTexture.expired())
			return false;

		DirectX::XMFLOAT2 texSize = argTexture.lock()->ImageSize();
		assert(texSize.x >= argSize.x && "Arg sizeX over Texture size...");
		assert(texSize.y >= argSize.y && "Arg sizeY over Texture size...");

		return true;
	}
}
/**
*	@fn			DrawNum
*	@brief		数字のテクスチャ描画
*	@param[in]	描画位置
*	@param[in]	描画サイズ
*	@param[in]	描画したい数字
*	@param[in]	扱うテクスチャ
*	@param[in]	最大桁数
*	@param[in]	テクスチャの色
*/
void Utility::DrawNum(DirectX::XMFLOAT2 argPos, DirectX::XMFLOAT2 argSize, const float argNum, const std::weak_ptr<Texture> &argTexture, const int argDigitMax, DirectX::XMFLOAT4 argColor)
{
	float w = argNum;
	int i = 0;	//	!<	作業用の変数

	if (static_cast<int>(w) == 0)
	{
		for (i = 0; i < argDigitMax; ++i)
		{
			DrawRectGraph(DirectX::XMFLOAT2(argPos.x - i * argSize.x, argPos.y), DirectX::XMFLOAT2(0, 0), argSize, argTexture, argColor);
		}
	}
	else
	{
		while (w)
		{
			DrawRectGraph(DirectX::XMFLOAT2(argPos.x - i * argSize.x, argPos.y), DirectX::XMFLOAT2(((static_cast<int>(w) % 10) * argSize.x), 0), argSize, argTexture, argColor);
			w *= 0.1f;
			++i;
			if (i >= argDigitMax)
				break;
		}
	}
}
/**
*	@fn			DrawNum
*	@brief		数字のテクスチャ描画
*	@param[in]	描画位置
*	@param[in]	描画サイズ
*	@param[in]	画像の大きさ
*	@param[in]	描画したい数字
*	@param[in]	扱うテクスチャ
*	@param[in]	最大桁数
*	@param[in]	テクスチャの色
*/
void Utility::DrawNum(DirectX::XMFLOAT2 argPos, DirectX::XMFLOAT2 argSize, float argExRate, const float argNum, const std::weak_ptr<Texture> &argTexture, const int argDigitMax, DirectX::XMFLOAT4 argColor)
{
	float w = argNum;
	int i = 0;	//	!<	作業用の変数

	if (static_cast<int>(w) == 0)
	{
		for (i = 0; i < argDigitMax; ++i)
		{
			DrawRectRotaGraph(DirectX::XMFLOAT2(argPos.x - i * argSize.x, argPos.y), DirectX::XMFLOAT2(0, 0), argSize, argExRate, 0.f, argTexture, argColor);
		}
	}
	else
	{
		while (w)
		{
			DrawRectRotaGraph(DirectX::XMFLOAT2(argPos.x - i * argSize.x, argPos.y), DirectX::XMFLOAT2(((static_cast<int>(w) % 10) * argSize.x), 0), argSize, argExRate, 0.f, argTexture, argColor);
			w *= 0.1f;
			++i;
			if (i >= argDigitMax)
				break;
		}
	}
}
/**
*	@fn			DrawGraph（描画サイズ、深度指定なし）
*	@brief		指定位置、原寸、でテクスチャを描画
*	@param[in]	argPos		!<	描画座標
*	@param[in]	argTexture	!<	描画するテクスチャ
*/
void Utility::DrawGraph(DirectX::XMFLOAT2 argPos, const std::weak_ptr<Texture> &argTexture)
{
	if (argTexture.expired())
		return;
	Singleton<GraphManager>::Get()->GetSpriteBatch()->Entry(argTexture, argPos);
}
/**
*	@fn			DrawGraph（描画サイズ、深度指定あり）
*	@brief		指定位置、指定サイズ、指定の奥行、でテクスチャを描画
*	@param[in]	argPos		!<	描画座標
*	@param[in]	argSize		!<	描画サイズ
*	@param[in]	argDepth	!<	z軸の描画座標
*	@param[in]	argTexture	!<	描画するテクスチャ
*/
void Utility::DrawGraph(DirectX::XMFLOAT2 argPos, DirectX::XMFLOAT2 argSize, float argDepth, const std::weak_ptr<Texture> &argTexture)
{
	if (!CanDrawTexture(argTexture, argSize))
		return;
	const RECT Rect = { 0, 0, static_cast<LONG>(argSize.x), static_cast<LONG>(argSize.y) };
	Singleton<GraphManager>::Get()->GetSpriteBatch()->Entry(argTexture, argPos,
		&Rect, White, 0.f, DirectX::XMFLOAT2(0, 0), 1.f, argDepth);
}
/**
*	@fn			DrawGraph（深度指定なし、色指定あり）
*	@brief		指定位置、指定サイズ、指定の奥行、でテクスチャを描画
*	@param[in]	argPos		!<	描画座標
*	@param[in]	argSize		!<	描画サイズ
*	@param[in]	argTexture	!<	描画するテクスチャ
*	@param[in]	argColor	!<	テクスチャの色
*/
void Utility::DrawGraph(DirectX::XMFLOAT2 argPos, const std::weak_ptr<Texture> &argTexture, DirectX::XMVECTOR argColor)
{
	const DirectX::XMFLOAT2 Size = argTexture._Get()->ImageSize();
	if (!CanDrawTexture(argTexture, Size))
		return;
	const RECT Rect = { 0, 0, static_cast<LONG>(Size.x), static_cast<LONG>(Size.y) };
	Singleton<GraphManager>::Get()->GetSpriteBatch()->Entry(argTexture, argPos,
		&Rect, argColor, 0.f, DirectX::XMFLOAT2(0, 0), 1.f, 1.f);
}
/**
*	@fn			DrawRectGraph（色、アルファ指定無し）
*	@brief		指定位置、指定サイズ、切り抜き、でテクスチャを描画
*	@param[in]	argPos		!<	描画座標
*	@param[in]	argCutPos	!<	描画するテクスチャ上の矩形
*	@param[in]	argSize		!<	描画サイズ
*	@param[in]	argTexture	!<	描画するテクスチャ
*/
void Utility::DrawRectGraph(DirectX::XMFLOAT2 argPos, DirectX::XMFLOAT2 argCutPos, DirectX::XMFLOAT2 argSize, const std::weak_ptr<Texture> &argTexture)
{
	if (!CanDrawTexture(argTexture, argSize))
		return;
	const RECT Rect = { static_cast<LONG>(argCutPos.x), static_cast<LONG>(argCutPos.y), static_cast<LONG>(argCutPos.x + argSize.x), static_cast<LONG>(argCutPos.y + argSize.y) };
	Singleton<GraphManager>::Get()->GetSpriteBatch()->Entry(argTexture, argPos,
		&Rect, White, 0.f, DirectX::XMFLOAT2(0, 0), 1.f);
}
/**
*	@fn			DrawRectGraph（色、アルファ指定あり）
*	@brief		指定位置、指定サイズ、切り抜き、指定の色、でテクスチャを描画
*	@param[in]	argPos		!<	描画座標
*	@param[in]	argCutPos	!<	描画するテクスチャ上の矩形
*	@param[in]	argSize		!<	描画サイズ
*	@param[in]	argTexture	!<	描画するテクスチャ
*	@param[in]	argColor	!<	描画するテクスチャの色とアルファ
*/
void Utility::DrawRectGraph(DirectX::XMFLOAT2 argPos, DirectX::XMFLOAT2 argCutPos, DirectX::XMFLOAT2 argSize, const std::weak_ptr<Texture> &argTexture, DirectX::XMFLOAT4 argColor)
{
	if (!CanDrawTexture(argTexture, argSize))
		return;
	const RECT Rect = { static_cast<LONG>(argCutPos.x), static_cast<LONG>(argCutPos.y), static_cast<LONG>(argCutPos.x + argSize.x), static_cast<LONG>(argCutPos.y + argSize.y) };
	Singleton<GraphManager>::Get()->GetSpriteBatch()->Entry(argTexture, argPos,
		&Rect, DirectX::XMLoadFloat4(&argColor), 0.f, DirectX::XMFLOAT2(0, 0), 1.f);
}
/**
*	@fn			DrawRotaGraph（回転中心無し）
*	@brief		指定位置、指定サイズ、指定拡大率、指定角度、でテクスチャを描画
*	@param[in]	argPos		!<	描画座標
*	@param[in]	argSize		!<	描画サイズ
*	@param[in]	argExRate	!<	拡大率
*	@param[in]	argAngle	!<	描画角度
*	@param[in]	argTexture	!<	描画するテクスチャ
*/
void Utility::DrawRotaGraph(DirectX::XMFLOAT2 argPos, DirectX::XMFLOAT2 argSize, float argExRate, float argAngle, const std::weak_ptr<Texture> &argTexture)
{
	if (!CanDrawTexture(argTexture, argSize))
		return;
	const RECT Rect = { 0, 0, static_cast<LONG>(argSize.x), static_cast<LONG>(argSize.y) };
	Singleton<GraphManager>::Get()->GetSpriteBatch()->Entry(argTexture, argPos,
		&Rect, White, argAngle, DirectX::XMFLOAT2(argSize.x * 0.5f, argSize.y * 0.5f), argExRate);
}
/**
*	@fn			DrawRotaGraph（回転中心あり）
*	@brief		指定位置、指定サイズ、指定中心座標、指定拡大率、指定角度、でテクスチャを描画
*	@param[in]	argPos		!<	描画座標
*	@param[in]	argSize		!<	描画サイズ
*	@param[in]	argCenter	!<	描画の中心座標
*	@param[in]	argExRate	!<	拡大率
*	@param[in]	argAngle	!<	描画角度
*	@param[in]	argTexture	!<	描画するテクスチャ
*/
void Utility::DrawRotaGraph(DirectX::XMFLOAT2 argPos, DirectX::XMFLOAT2 argSize, DirectX::XMFLOAT2 argCenter, float argExRate, float argAngle, const std::weak_ptr<Texture> &argTexture)
{
	if (!CanDrawTexture(argTexture, argSize))
		return;
	const RECT Rect = { 0, 0, static_cast<LONG>(argSize.x), static_cast<LONG>(argSize.y) };
	Singleton<GraphManager>::Get()->GetSpriteBatch()->Entry(argTexture, argPos,
		&Rect, White, argAngle, argCenter, argExRate);
}
/**
*	@fn			DrawRotaGraph（回転中心、縦横別の拡縮アリ）
*	@brief		指定位置、指定サイズ、指定中心座標、指定拡大率、指定角度、でテクスチャを描画
*	@param[in]	argPos		!<	描画座標
*	@param[in]	argSize		!<	描画サイズ
*	@param[in]	argCenter	!<	描画の中心座標
*	@param[in]	argExRate	!<	拡大率
*	@param[in]	argAngle	!<	描画角度
*	@param[in]	argTexture	!<	描画するテクスチャ
*/
void Utility::DrawRotaGraph(DirectX::XMFLOAT2 argPos, DirectX::XMFLOAT2 argSize, DirectX::XMFLOAT2 argCenter, DirectX::XMFLOAT2 argExRate, float argAngle, const std::weak_ptr<Texture> &argTexture)
{
	if (!CanDrawTexture(argTexture, argSize))
		return;
	const RECT Rect = { 0, 0, static_cast<LONG>(argSize.x), static_cast<LONG>(argSize.y) };
	Singleton<GraphManager>::Get()->GetSpriteBatch()->Entry(argTexture, argPos,
		&Rect, White, argAngle, argCenter, argExRate);
}
/**
*	@fn			DrawRotaGraph（回転中心、色、アルファ指定アリ）
*	@brief		指定位置、指定サイズ、指定中心座標、指定拡大率、指定角度、指定の色、でテクスチャを描画
*	@param[in]	argPos		!<	描画座標
*	@param[in]	argSize		!<	描画サイズ
*	@param[in]	argCenter	!<	描画の中心座標
*	@param[in]	argExRate	!<	拡大率
*	@param[in]	argAngle	!<	描画角度
*	@param[in]	argTexture	!<	描画するテクスチャ
*	@param[in]	argColor	!<	描画するテクスチャの色とアルファ
*/
void Utility::DrawRotaGraph(DirectX::XMFLOAT2 argPos, DirectX::XMFLOAT2 argSize, DirectX::XMFLOAT2 argCenter, DirectX::XMFLOAT2 argExRate, float argAngle, const std::weak_ptr<Texture> &argTexture, DirectX::XMFLOAT4 argColor)
{
	if (!CanDrawTexture(argTexture, argSize))
		return;
	const RECT Rect = { 0, 0, static_cast<LONG>(argSize.x), static_cast<LONG>(argSize.y) };
	Singleton<GraphManager>::Get()->GetSpriteBatch()->Entry(argTexture, argPos,
		&Rect, DirectX::XMLoadFloat4(&argColor), argAngle, argCenter, argExRate);
}
/**
*	@fn			DrawRotaGraph（回転中心、色、アルファ指定アリ）
*	@brief		指定位置、指定サイズ、指定中心座標、指定拡大率、指定角度、指定の色、でテクスチャを描画
*	@param[in]	argPos		!<	描画座標
*	@param[in]	argSize		!<	描画サイズ
*	@param[in]	argCenter	!<	描画の中心座標
*	@param[in]	argExRate	!<	拡大率
*	@param[in]	argAngle	!<	描画角度
*	@param[in]	argTexture	!<	描画するテクスチャ
*	@param[in]	argColor	!<	描画するテクスチャの色とアルファ
*/
void Utility::DrawRotaGraph(DirectX::XMFLOAT2 argPos, DirectX::XMFLOAT2 argSize, DirectX::XMFLOAT2 argCenter, float argExRate, float argAngle, const std::weak_ptr<Texture> &argTexture, DirectX::XMFLOAT4 argColor)
{
	if (!CanDrawTexture(argTexture, argSize))
		return;
	const RECT Rect = { 0, 0, static_cast<LONG>(argSize.x), static_cast<LONG>(argSize.y) };
	Singleton<GraphManager>::Get()->GetSpriteBatch()->Entry(argTexture, argPos,
		&Rect, DirectX::XMLoadFloat4(&argColor), argAngle, argCenter, argExRate);
}
/**
*	@fn			DrawRectRotaGraph（色とアルファの指定なし）
*	@brief		指定位置、指定サイズ、切り抜き、指定拡大率、指定角度、でテクスチャを描画
*	@param[in]	argPos		!<	描画座標
*	@param[in]	argCutPos	!<	描画するテクスチャ上の矩形
*	@param[in]	argSize		!<	描画サイズ
*	@param[in]	argExRate	!<	拡大率
*	@param[in]	argAngle	!<	描画角度
*	@param[in]	argTexture	!<	描画するテクスチャ
*/
void Utility::DrawRectRotaGraph(DirectX::XMFLOAT2 argPos, DirectX::XMFLOAT2 argCutPos, DirectX::XMFLOAT2 argSize, float argExRate, float argAngle, const std::weak_ptr<Texture> &argTexture)
{
	if (!CanDrawTexture(argTexture, argSize))
		return;
	const RECT Rect = { static_cast<LONG>(argCutPos.x), static_cast<LONG>(argCutPos.y), static_cast<LONG>(argCutPos.x + argSize.x), static_cast<LONG>(argCutPos.y + argSize.y) };
	Singleton<GraphManager>::Get()->GetSpriteBatch()->Entry(argTexture, argPos,
		&Rect, White, argAngle, DirectX::XMFLOAT2(argSize.x * 0.5f, argSize.y * 0.5f), argExRate);
}
/**
*	@fn			DrawRectRotaGraph（色とアルファの指定あり）
*	@brief		指定位置、指定サイズ、切り抜き、指定拡大率、指定角度、でテクスチャを描画
*	@param[in]	argPos		!<	描画座標
*	@param[in]	argCutPos	!<	描画するテクスチャ上の矩形
*	@param[in]	argSize		!<	描画サイズ
*	@param[in]	argExRate	!<	拡大率
*	@param[in]	argAngle	!<	描画角度
*	@param[in]	argTexture	!<	描画するテクスチャ
*	@param[in]	argColor	!<	描画するテクスチャの色とアルファ
*/
void Utility::DrawRectRotaGraph(DirectX::XMFLOAT2 argPos, DirectX::XMFLOAT2 argCutPos, DirectX::XMFLOAT2 argSize, float argExRate, float argAngle, const std::weak_ptr<Texture> &argTexture, DirectX::XMFLOAT4 argColor)
{
	if (!CanDrawTexture(argTexture, argSize))
		return;
	const RECT Rect = { static_cast<LONG>(argCutPos.x), static_cast<LONG>(argCutPos.y), static_cast<LONG>(argCutPos.x + argSize.x), static_cast<LONG>(argCutPos.y + argSize.y) };
	Singleton<GraphManager>::Get()->GetSpriteBatch()->Entry(argTexture, argPos,
		&Rect, DirectX::XMLoadFloat4(&argColor), argAngle, DirectX::XMFLOAT2(argSize.x * 0.5f, argSize.y * 0.5f), argExRate);
}
#pragma endregion
