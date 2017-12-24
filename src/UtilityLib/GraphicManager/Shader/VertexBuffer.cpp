/**
 *	@file	VertexBuffer.cpp
 *	@date	2017 / 12 / 20
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */

#include "VertexBuffer.h"
#include <assert.h>

template<typename T>
Utility::VertexBuffer<T>::VertexBuffer(ID3D11Device *argDevice, uint32_t argVertexCount)
{
	vertices_.resize(argVertexCount);
	HRESULT hr = Create(argDevice, &buffer_, vertices_.size(), vertices_.data());
	assert(SUCCEEDED(hr) && "Create Vertex Buffer Failed...");
}

template<typename T>
static HRESULT Utility::VertexBuffer<T>::Create(ID3D11Device *argDevice, ID3D11Buffer **argBuffer, uint32_t argVertexCount, T *argVertices)
{
	if (argVertexCount == 0)
	{
		OutputDebugString(L"Stride or VertexCount Search Failed...\n");
		return E_FAIL;
	}

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(T) * argVertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));

	InitData.pSysMem = argVertices;

	HRESULT hr = argDevice->CreateBuffer(&bd, &InitData, argBuffer);
	if (FAILED(hr))
	{
		OutputDebugString(L"Create Vertex Constant Buffer Failed...\n");
		return hr;
	}
	return hr;
}// CreateVertexBuffer

template<typename T>
void Utility::VertexBuffer<T>::Create(ID3D11Device *argDevice)
{
	assert(vertices_.size() > 0 && "There is no vertices...");
	HRESULT hr = Create(argDevice, &buffer_, vertices_.size(), vertices_.data());
	assert(SUCCEEDED(hr) && "Create Vertex Buffer Failed...");
}

template<typename T> 
void Utility::VertexBuffer<T>::Update(ID3D11DeviceContext *argContext)
{
	argContext->UpdateSubresource(buffer_.Get(), 0, nullptr, vertices_.data(), 0, 0);
}

template<typename T> 
void  Utility::VertexBuffer<T>::SetVertexBuffers(ID3D11DeviceContext *argContext)
{
	const unsigned int Stride = sizeof(T);
	const unsigned int Center = 0;
	argContext->IASetVertexBuffers(0, 1, buffer_.GetAddressOf(), &Stride, &Center);
}

template<typename T>
std::vector<T> Utility::VertexBuffer<T>::Vertices() const
{
	return vertices_;
}

template<typename T>
void Utility::VertexBuffer<T>::Vertices(std::vector<T> argVertices)
{
	vertices_ = argVertices;
}

template<typename T>
const T * Utility::VertexBuffer<T>::operator[](long i) const
{
	assert(i >= 0L && i < static_cast<long>(vertices_.size()));
	return &vertices_[i];
}

template<typename T>
T * Utility::VertexBuffer<T>::operator[](long i)
{
	assert(i >= 0L && i < static_cast<long>(vertices_.size()));
	return &vertices_[i];
}

template<typename T>
const ID3D11Buffer * const * Utility::VertexBuffer<T>::operator&() const
{
	assert(buffer_.GetAddressOf());
	return buffer_.GetAddressOf();
}

template<typename T>
ID3D11Buffer ** Utility::VertexBuffer<T>::operator&()
{
	assert(buffer_.GetAddressOf());
	return buffer_.GetAddressOf();
}
