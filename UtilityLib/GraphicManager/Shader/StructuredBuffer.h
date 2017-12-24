/**
 *	@file	StructuredBuffer.h
 *	@date	2017 / 12 / 20
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <d3d11.h>
#include <assert.h>
#include <wrl/client.h>

struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11ShaderResourceView;

namespace Utility
{
	template<typename T>
	class StructuredBuffer final
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> sendBuf_;
		Microsoft::WRL::ComPtr<ID3D11Buffer> receiveBuf_;
		Microsoft::WRL::ComPtr<ID3D11Buffer> pullBuf_;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv_;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav_;
		T* resData_;
	public:
		StructuredBuffer(ID3D11Device *argDevice, int argIndexCount, T* argBufferData)
		{
			Create(argDevice, argIndexCount, argBufferData);
			assert(SUCCEEDED(hr) && "Create Structured Buffer Failed...");
		}
		StructuredBuffer() = default;
		~StructuredBuffer() = default;
	public:
		/**
		 *	@fn			Create
		 *	@brief		頂点インデックスバッファの生成
		 *	@param[in]	argDevice		!<	デバイス
		 *	@param[in]	argBuffer		!<	生成した頂点インデックスバッファを格納するバッファのポインタ
		 *	@param[in]	argIndexCount	!<	頂点インデックス数
		 *	@param[in]	argBufferData	!<	バッファのデータ
		 *	@retval		S_OK			!<	生成に成功
		 *	@retval		E_FAIL			!<	生成に失敗または頂点インデックスが存在しない
		 */
		HRESULT Create(ID3D11Device *argDevice, int argIndexCount, T* argBufferData)
		{
			const size_t Size = sizeof(T);

			D3D11_BUFFER_DESC bufDesc;
			ZeroMemory(&bufDesc, sizeof(bufDesc));
			bufDesc.ByteWidth = argIndexCount * Size;
			bufDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
			bufDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
			bufDesc.StructureByteStride = Size;

			D3D11_SUBRESOURCE_DATA subResource;
			subResource.pSysMem = argBufferData;
			//	シェーダーに情報を出力するバッファ作成
			HRESULT hr = argDevice->CreateBuffer(&bufDesc, &subResource, &sendBuf_);
			assert(SUCCEEDED(hr) && "Create Send Send Buffer Failed...");

			//	シェーダーから情報を受け取るバッファ作成
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

			//	バッファをシェーダーリソースに変換してシェーダーに出力する
			//	StructuredBuffer<T>(t0)
			hr = argDevice->CreateShaderResourceView(sendBuf_.Get(), &srvDesc, &srv_);
			assert(SUCCEEDED(hr) && "Create ShaderResourceView Failed...");

			ZeroMemory(&bufDesc, sizeof(bufDesc));
			receiveBuf_->GetDesc(&bufDesc);

			D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
			ZeroMemory(&uavDesc, sizeof(uavDesc));
			uavDesc.Format = DXGI_FORMAT_UNKNOWN;
			uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
			uavDesc.Buffer.FirstElement = 0;
			uavDesc.Buffer.NumElements = bufDesc.ByteWidth / bufDesc.StructureByteStride;

			//	バッファをアクセスビューに変換してシェーダーから情報を受け取る
			//	RWStructuredBuffer<T>(u0)
			hr = argDevice->CreateUnorderedAccessView(receiveBuf_.Get(), &uavDesc, &uav_);
			assert(SUCCEEDED(hr) && "Create UnorderedAccessView Failed...");

			ZeroMemory(&bufDesc, sizeof(bufDesc));
			receiveBuf_->GetDesc(&bufDesc);
			bufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			bufDesc.Usage = D3D11_USAGE_STAGING;
			bufDesc.BindFlags = 0;
			bufDesc.MiscFlags = 0;

			//	情報の取り出し用バッファ作成
			hr = argDevice->CreateBuffer(&bufDesc, nullptr, &pullBuf_);
			assert(SUCCEEDED(hr) && "Create PullBuffer Failed...");

			return hr;

		}

		/**
		 *  @fn			SetStructuredBuffer
		 *  @brief		インデックスバッファの設定
		 *  @param[in]	argContext	!<	コンテキスト
		 */
		inline void SetStructuredBuffer(ID3D11DeviceContext *argContext, unsigned int argThreadX, unsigned int argThreadY, unsigned int argThreadZ)
		{
			argContext->CSSetShaderResources(0, 1, srv_.GetAddressOf());
			argContext->CSSetUnorderedAccessViews(0, 1, uav_.GetAddressOf(), nullptr);
			//	[numthreads(argThreadX, argThreadY, argThreadZ)]
			argContext->Dispatch(argThreadX, argThreadY, argThreadZ);

			argContext->CopyResource(pullBuf_.Get(), receiveBuf_.Get());

			D3D11_MAPPED_SUBRESOURCE mappedResource;
			HRESULT hr = argContext->Map(pullBuf_.Get(), 0, D3D11_MAP_READ, 0, &mappedResource);
			assert(SUCCEEDED(hr) && "StructuredBuffer Map Failed...");

			resData_ = static_cast<T*>(mappedResource.pData);
			argContext->Unmap(pullBuf_.Get(), 0);
		}
		inline const T* const GetData()const
		{
			return resData_;
		}

	public:
		/**
		 *	@fn			operator()
		 *	@brief		読み取り用operator
		 *	@return		バッファ
		 */
		inline T* operator()()const
		{
			return resData_;
		}
	};
}