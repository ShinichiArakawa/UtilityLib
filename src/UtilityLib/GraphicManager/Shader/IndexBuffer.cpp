/**
 *	@file	IndexBuffer.cpp
 *	@date	2017 / 10 / 06
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */

#include "IndexBuffer.h"
#include "../../DeviceResources/DeviceResources.h"

#include <d3d11.h>
#include <assert.h>

Utility::IndexBuffer::IndexBuffer(ID3D11Device *argDevice, std::vector<unsigned int> argIndices)
	:bitCount_(DXGI_FORMAT_R32_UINT)
{
	HRESULT hr = Create(argDevice, &buffer_, argIndices.size(), argIndices.data());
	assert(SUCCEEDED(hr) && "Create Index Buffer Failed...");
}

/**
 *	@fn			Create
 *	@brief		頂点インデックスバッファの生成
 *	@param[in]	argDevice		!<	デバイス
 *	@param[in]	argBuffer		!<	生成した頂点インデックスバッファを格納するバッファのポインタ
 *	@param[in]	argIndexCount	!<	頂点インデックス数
 *	@param[in]	argIndices		!<	頂点インデックスデータ
 *	@retval		S_OK			!<	生成に成功
 *	@retval		E_FAIL			!<	生成に失敗または頂点インデックスが存在しない
 */
HRESULT Utility::IndexBuffer::Create(ID3D11Device *argDevice, ID3D11Buffer **argBuffer, uint32_t argIndexCount, unsigned int *argIndices)
{
	if (argIndexCount == 0)
	{
		OutputDebugString(L"IndexCount Search Failed...\n");
		return E_FAIL;
	}

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(uint32_t) * argIndexCount;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));

	InitData.pSysMem = argIndices;

	HRESULT hr = argDevice->CreateBuffer(&bd, &InitData, argBuffer);
	if (FAILED(hr))
	{
		OutputDebugString(L"Create Index Buffer Failed...\n");
		return hr;
	}
	return hr;
}// CreateIndexBuffer

void Utility::IndexBuffer::Create(ID3D11Device * argDevice)
{
	assert(indices_.size() && "There is no indices...");
	bitCount_ = DXGI_FORMAT_R32_UINT;
	HRESULT hr = Create(argDevice, &buffer_, indices_.size(), indices_.data());
	assert(SUCCEEDED(hr) && "Create Index Buffer Failed...");
}

void Utility::IndexBuffer::SetIndexBuffer(ID3D11DeviceContext *argContext)
{
	argContext->IASetIndexBuffer(buffer_.Get(), bitCount_, 0);
}
const ID3D11Buffer* const* Utility::IndexBuffer::operator&() const
{
	assert(buffer_.GetAddressOf());
	return buffer_.GetAddressOf();
}
ID3D11Buffer ** Utility::IndexBuffer::operator&()
{
	assert(buffer_.GetAddressOf());
	return buffer_.GetAddressOf();
}