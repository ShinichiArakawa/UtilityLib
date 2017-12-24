/**
 *	@file	ConstantBuffer.h
 *	@date	2017 / 10 / 16
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <wrl/client.h>
#include <d3d11.h>
#include <assert.h>

#pragma warning(push)
#pragma warning(disable : 4324)	//	!<	アライメントのパック
namespace Utility
{
	/**
	 *	@enum	eShaderType
	 *	@brief	GPUと連携するシェーダーの種類
	 */
	enum class eShaderType : unsigned char
	{
		Vertex,
		Pixel,
		Geometry,
		Compute,
		Hull,
	};
	/**
	 *	@class	ConstantBuffer
	 *	@brief	コンスタントバッファ
	 */
	template<typename T>
	class __declspec(align(16)) ConstantBuffer : public T
	{
	private:
		ConstantBuffer(ConstantBuffer const&) = delete;
		ConstantBuffer& operator= (ConstantBuffer const&) = delete;
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer_;
		int slotCnt = 0;
		eShaderType type_;
	public:

		ConstantBuffer() = default;
		/**
		 *	@constructor	ConstantBuffer
		 *	@brief			定数バッファ
		 *	@param[in]		argDevice		!<	デバイス
		 *	@param[in]		argShaderType	!<	セットするシェーダーの種類
		 *	@param[in]		argSlotNum		!<	定数バッファの番号
		 */
		explicit ConstantBuffer(ID3D11Device *argDevice, eShaderType argShaderType, int argSlotNum)
		{
			slotCnt = argSlotNum;
			Create(argDevice, argShaderType);
		}
		/**
		 *	@constructor	ConstantBuffer
		 *	@brief			定数バッファ
		 *	@param[in]		argDevice		!<	デバイス
		 *	@param[in]		argShaderType	!<	セットするシェーダーの種類
		 */
		explicit ConstantBuffer(ID3D11Device *argDevice, eShaderType argShaderType)
		{
			Create(argDevice, argShaderType);
		}
		~ConstantBuffer() = default;
	public:
		/**
		 *	@fn			Create
		 *	@brief		定数バッファの生成
		 *	@param[in]	argDevice		!<	デバイス
		 *	@param[in]	argShaderType	!<	セットするシェーダーの種類
		 */
		void Create(ID3D11Device *argDevice, eShaderType argShaderType)
		{
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.ByteWidth = sizeof(T);
			bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			HRESULT hr = argDevice->CreateBuffer(&bd, nullptr, buffer_.ReleaseAndGetAddressOf());
			assert(SUCCEEDED(hr) && "Create Constant Buffer Failed...");

			type_ = argShaderType;
		}
		/**
		 *	@fn			Update
		 *	@brief		定数バッファの更新
		 *	@param[in]	argContext	!<	デバイスコンテキスト
		 *	@note		定数バッファは毎フレーム更新するのでMapを使う
		 */
		void Update(ID3D11DeviceContext *argImmediateContext)
		{
			SetData(argImmediateContext);

			switch (type_)
			{
			case eShaderType::Vertex:
				argImmediateContext->VSSetConstantBuffers(slotCnt, 1, buffer_.GetAddressOf());
				break;
			case eShaderType::Pixel:
				argImmediateContext->PSSetConstantBuffers(slotCnt, 1, buffer_.GetAddressOf());
				break;
			case eShaderType::Geometry:
				argImmediateContext->GSSetConstantBuffers(slotCnt, 1, buffer_.GetAddressOf());
				break;
			case eShaderType::Compute:
				argImmediateContext->CSSetConstantBuffers(slotCnt, 1, buffer_.GetAddressOf());

				break;
			case eShaderType::Hull:
				argImmediateContext->HSSetConstantBuffers(slotCnt, 1, buffer_.GetAddressOf());
				break;
			}
		}
		/**
		 *	@fn			SetData
		 *	@brief		定数バッファのデータのセット
		 *	@param[in]	argContext	!<	デバイスコンテキスト
		 *	@note		Updateとは違いデータの更新を配列でまとめて可能
		 */
		void SetData(ID3D11DeviceContext *argImmediateContext)
		{
			assert(buffer_.Get() && "Not Yet Create Buffer");

			D3D11_MAPPED_SUBRESOURCE mappedResource;

			argImmediateContext->Map(buffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

			*static_cast<T*>(mappedResource.pData) = *static_cast<T*>(this);

			argImmediateContext->Unmap(buffer_.Get(), 0);

		}

	public:
		/**
		 *	@fn			operator new
		 *	@brief		8の累乗byte区切りのアライメント用のメモリ確保
		 *	@param[in]	argSize	!<	確保するメモリのサイズ
		 *	@attention	定数バッファはSIMD命令に最適化しているので、8の累乗byte区切りのアライメント
		 */
		static void* operator new (size_t argSize)
		{
			const size_t Alignment = __alignof(ConstantBuffer<T>);

			static_assert(Alignment > 8, "AlignedNew is only useful for types with > 8 byte alignment. Did you forget a __declspec(align) on TDerived?");

			void* ptr = _aligned_malloc(argSize, Alignment);

			if (!ptr)
				throw std::bad_alloc();

			return ptr;
		}
		/**
		 *	@fn			operator delete
		 *	@brief		メモリ解放
		 */
		static void operator delete (void* argPtr)
		{
			_aligned_free(argPtr);
		}

	};
};
#pragma warning(pop)
