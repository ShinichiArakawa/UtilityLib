/**
 *	@file	Grid.cpp
 *	@date	2017 / 8 / 21
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "Grid.h"
#include "../GraphicManager/GraphManager.h"
#include "../Singleton/Singleton.h"
#include "../ConstantBuffer.h"
#include "../DeviceResources/DeviceResources.h"

#include "../Resource/GridVS.h"
#include "../Resource/GridPS.h"

class Utility::Grid::Impl
{
public:

	Impl(ID3D11DeviceContext* argContext)
		: context_(argContext), isVisible_(false)
	{
		vertexShader_ = VertexShader(argContext, &inputLayout_, reinterpret_cast<char*>(BinDataGridVS), BinSizeGridVS);
		pixelShader_ = PixelShader(argContext, reinterpret_cast<char*>(BinDataGridPS), BinSizeGridPS);

		ID3D11Device* device = GetD3Ddevice(context_.Get()).Get();
		cbGridMatrix_ = std::make_unique<ConstantBuffer<SimpleMatrix>>(device, eShaderType::Vertex, 0);
		vertexBuffer_ = VertexBuffer<SimpleVertex>(device, VertexNum);
	}

	~Impl() = default;

private:
	void DrawGrid(size_t argXdivs, size_t argYdivs)
	{
		const float Space = argXdivs * 0.05f;
		const float Origin = -Space * (argXdivs * 0.5f);

		for (size_t i = 0; i < argXdivs; i += 2)
		{
			const DirectX::XMFLOAT4 Up = DirectX::XMFLOAT4(Origin + Space * i, 0.f, Origin, 1.f);
			const DirectX::XMFLOAT4 Under = DirectX::XMFLOAT4(Origin + Space * i, 0.f, -Origin, 1.f);
			vertexBuffer_[i]->Pos = Up;			//	上
			vertexBuffer_[i + 1]->Pos = Under;	//	下
		}

		for (size_t i = 0; i < argYdivs; i += 2)
		{
			const DirectX::XMFLOAT4 Left = DirectX::XMFLOAT4(Origin, 0.f, Origin + Space * i, 1.f);
			const DirectX::XMFLOAT4 Right = DirectX::XMFLOAT4(-Origin, 0.f, Origin + Space * i, 1.f);
			vertexBuffer_[i + argXdivs]->Pos = Left;			//	左
			vertexBuffer_[i + argXdivs + 1]->Pos = Right;	//	右
		}

		vertexShader_.SetShader(context_.Get());
		pixelShader_.SetShader(context_.Get());

		cbGridMatrix_->Update(context_.Get());
		vertexBuffer_.Update(context_.Get());

		inputLayout_.SetInputLayout(context_.Get());
		vertexBuffer_.SetVertexBuffers(context_.Get());
		context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		context_->Draw(VertexNum, 0);
	}
public:

	void Draw(DirectX::XMFLOAT4X4 argView, DirectX::XMFLOAT4X4 argProj)
	{
		if (isVisible_)
			return;
		
		DirectX::XMStoreFloat4x4(&cbGridMatrix_->World, DirectX::XMMatrixIdentity());
		cbGridMatrix_->View = argView;
		cbGridMatrix_->Proj = argProj;
		
		DrawGrid(static_cast<size_t>(VertexDiv.x), static_cast<size_t>(VertexDiv.y));

	}


private:
	const int							GridVertices = 8;			//	!<	グリッドの格子1つ分の頂点
	const DirectX::XMFLOAT2						GridNum = DirectX::XMFLOAT2(12.f,12.f);	//	!<	描画したい格子の行と列の数
	const DirectX::XMFLOAT2						VertexDiv = DirectX::XMFLOAT2(GridVertices * GridNum.x, GridVertices * GridNum.y);	//	!<	1行と1列の頂点数
	const uint32_t						VertexNum = static_cast<uint32_t>(VertexDiv.x + VertexDiv.y) * 2;	//	!<	総頂点数
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>	context_;
	VertexShader								vertexShader_;
	PixelShader									pixelShader_;
	std::unique_ptr<ConstantBuffer<SimpleMatrix>>	cbGridMatrix_;
	VertexBuffer<SimpleVertex>	vertexBuffer_;
	InputLayout				inputLayout_;
public:
	bool								isVisible_;
};

Utility::Grid::Grid(ID3D11DeviceContext *argContext)
	: pImpl(std::make_unique<Impl>(argContext))
{
}

Utility::Grid::~Grid() = default;

void Utility::Grid::Draw(DirectX::XMFLOAT4X4 argView, DirectX::XMFLOAT4X4 argProj)
{
	pImpl->Draw(argView, argProj);
}

void Utility::Grid::IsVisible(bool argIsVisble)
{
	pImpl->isVisible_ = argIsVisble;
}
