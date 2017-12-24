/**
*	@file	VertexBuffer.h
*	@date	2017 / 10 / 06
*	@author	Katsumi Takei
*	Copyright (c) Kastumi Takei. All rights reserved.
*/
#pragma once

#include <vector>
#include <wrl/client.h>
#include <d3d11.h>
#include <assert.h>

namespace Utility
{
	template<typename T>
	class VertexBuffer final
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer_;
		std::vector<T> vertices_;
	public:
		/**
		 *	@constructor	VertexBuffer
		 *	@brief			頂点バッファの生成
		 *	@param[in]		argDevice		!<	デバイス
		 *	@param[in]		argVertexCount	!<	頂点数
		 */
		VertexBuffer(ID3D11Device *argDevice, uint32_t argVertexCount)
		{
			vertices_.resize(argVertexCount);
			HRESULT hr = Create(argDevice, &buffer_, vertices_.size(), vertices_.data());
			assert(SUCCEEDED(hr) && "Create Vertex Buffer Failed...");
		}
		VertexBuffer() = default;
		~VertexBuffer() = default;
	public:
		/**
		 *	@fn			Create
		 *	@brief		頂点バッファの生成
		 *	@param[in]	argDevice		!<	デバイス
		 *	@param[in]	argBuffer		!<	生成した頂点バッファを格納するバッファのポインタ
		 *	@param[in]	argVertexCount	!<	頂点数
		 *	@param[in]	argVertices		!<	頂点データ
		 *	@retval		S_OK			!<	生成に成功
		 *	@retval		E_FAIL			!<	生成に失敗または頂点が存在しない
		 */
		static HRESULT Create(ID3D11Device *argDevice, ID3D11Buffer **argBuffer, uint32_t argVertexCount, T *argVertices)
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
	public:
		/**
		 *	@fn			Create
		 *	@brief		頂点バッファの生成
		 *	@param[in]	argDevice		!<	デバイス
		 */
		inline void Create(ID3D11Device *argDevice)
		{
			assert(vertices_.size() > 0 && "There is no vertices...");
			HRESULT hr = Create(argDevice, &buffer_, vertices_.size(), vertices_.data());
			assert(SUCCEEDED(hr) && "Create Vertex Buffer Failed...");
		}
		/**
		 *	@fn			Update
		 *	@brief		頂点バッファの更新
		 *	@param[in]	argContext		!<	コンテキスト
		 */
		inline void Update(ID3D11DeviceContext *argContext)
		{
			argContext->UpdateSubresource(buffer_.Get(), 0, nullptr, vertices_.data(), 0, 0);
		}
		/**
		 *	@fn			SetVertexBuffers
		 *	@brief		頂点バッファの設定
		 *	@param[in]	argContext		!<	コンテキスト
		 */
		inline void SetVertexBuffers(ID3D11DeviceContext *argContext)
		{
			const unsigned int Stride = sizeof(T);
			const unsigned int Center = 0;
			argContext->IASetVertexBuffers(0, 1, buffer_.GetAddressOf(), &Stride, &Center);
		}
	public:
		inline std::vector<T> Vertices()const { return vertices_; }
		inline void Vertices(std::vector<T> argVertices){ vertices_ = argVertices; }

#pragma region		Operator
	public:
		/**
		 *	@fn			operator[]
		 *	@brief		頂点読み取り用operator
		 *	@param[in]	i	!<	インデックス
		 *	@return		頂点のインデックス
		 */
		inline const T* operator[](long i) const
		{
			assert(i >= 0L && i < static_cast<long>(vertices_.size()));
			return &vertices_[i];
		}
		/**
		 *	@fn			operator[]
		 *	@brief		書き込みoperator
		 *	@param[in]	i	!<	インデックス
		 *	@return		頂点のインデックス
		 */
		inline T* operator[](long i)
		{
			assert(i >= 0L && i < static_cast<long>(vertices_.size()));
			return &vertices_[i];
		}
		/**
		 *	@fn			operator&
		 *	@brief		読み取りoperator
		 *	@return		バッファ
		 */
		inline const ID3D11Buffer* const* operator&() const
		{
			assert(buffer_.GetAddressOf());
			return buffer_.GetAddressOf();
		}
		/**
		 *	@fn			operator&
		 *	@brief		書き込み用operator
		 *	@return		バッファ
		 */
		inline ID3D11Buffer ** operator&()
		{
			assert(buffer_.GetAddressOf());
			return buffer_.GetAddressOf();
		}
#pragma endregion	Operator
	};
	
}
