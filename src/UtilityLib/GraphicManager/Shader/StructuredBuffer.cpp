/**
 *	@file	StructuredBuffer.cpp
 *	@date	2017 / 12 / 20
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */

#include "StructuredBuffer.h"
#include <assert.h>

template<typename T>
Utility::StructuredBuffer<T>::StructuredBuffer(ID3D11Device *argDevice, int argIndexCount, T* argBufferData)
{
	Create(argDevice, argIndexCount, argBufferData);
	assert(SUCCEEDED(hr) && "Create Structured Buffer Failed...");
}

template<typename T>
HRESULT Utility::StructuredBuffer<T>::Create(ID3D11Device * argDevice, int argIndexCount, T * argBufferData)
{
	const size_t Size = sizeof(T);

	D3D11_BUFFER_DESC bufDesc;
	ZeroMemory(&bufferDesc, sizeof(bufDesc));
	bufDesc.ByteWidth = argIndexCount * Size;
	bufDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	bufDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufDesc.StructureByteStride = Size;

	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = argBufferData;

	HRESULT hr = argDevice->CreateBuffer(&bufDesc, &subResource, &sendBuf_);
	assert(SUCCEEDED(hr) && "Create Send Send Buffer Failed...");

	hr = argDevice->CreateBuffer(&bufDesc, nullptr, &receiveBuf_);
	assert(SUCCEEDED(hr) && "Create Receive Receive Buffer Failed...");

	ZeroMemory(&bufDesc, sizeof(bufDesc));
	sendBuf_->GetDesc(&bufDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.FirstElement = 0;
	srvDesc.BufferEx.NumElements = bufDesc.ByteWidth / bufDesc.StructureByteStride;

	hr = argDevice->CreateShaderResourceView(&sendBuf_, &srvDesc, &srv_);
	assert(SUCCEEDED(hr) && "Create ShaderResourceView Failed...");

	ZeroMemory(&bufDesc, sizeof(bufDesc));
	receiveBuf_->GetDesc(&bufDesc);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(uavDesc));
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = bufDesc.ByteWidth / bufDesc.StructureByteStride;

	hr = argDevice->CreateUnorderedAccessView(&receiveBuf_, &uavDesc, &uav_);
	assert(SUCCEEDED(hr) && "Create UnorderedAccessView Failed...");

	ZeroMemory(&bufDesc, sizeof(bufDesc));
	resultBuffer->GetDesc(&bufDesc);
	bufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	bufDesc.Usage = D3D11_USAGE_STAGING;
	bufDesc.BindFlags = 0;
	bufDesc.MiscFlags = 0;

	hr = device->CreateBuffer(&bufDesc, nullptr, &pullBuf_);
	assert(SUCCEEDED(hr) && "Create PullBuffer Failed...");

	return hr;

}

template<typename T>
void Utility::StructuredBuffer<T>::SetStructuredBuffer(ID3D11DeviceContext * argContext, unsigned int argThreadX, unsigned int argThreadY, unsigned int argThreadZ)
{
	argContext->CSSetShaderResources(0, 1, &srv_);
	argContext->CSSetUnorderedAccessViews(0, 1, &uav_, nullptr);
	argContext->Dispatch(argThreadX, argThreadY, argThreadZ);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	argContext->Map(pullBuf_.Get(), 0, D3D11_MAP_READ, 0, &mappedResource);
	resData_ = static_cast<T*>(mappedResource.pData);
	argContext->Unmap(pullBuf_.Get(), 0);
}

template<typename T>
const T * const * Utility::StructuredBuffer<T>::GetData()
{
	return resData_;
}

template<typename T>
const T * const * Utility::StructuredBuffer<T>::operator()() const
{
	return resData_;
}

