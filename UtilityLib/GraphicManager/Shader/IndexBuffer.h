/**
 *	@file	IndexBuffer.h
 *	@date	2017 / 10 / 06
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <vector>
#include <wrl/client.h>

enum DXGI_FORMAT;
struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;

namespace Utility
{
	class IndexBuffer final
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer_;
		std::vector<unsigned int>indices_;
		DXGI_FORMAT bitCount_;
	public:
		IndexBuffer(ID3D11Device *argDevice, std::vector<unsigned int> argIndices);
		IndexBuffer() = default;
		~IndexBuffer() = default;
	public:
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
		 static HRESULT Create(ID3D11Device *argDevice, ID3D11Buffer **argBuffer, uint32_t argIndexCount, unsigned int *argIndices);

	public:
		/**
		 *  @fn			Create
		 *  @brief		頂点インデックスバッファの生成
		 *  @param[in]	argDevice	!<	デバイス
		 */
		void Create(ID3D11Device *argDevice);
		/**
		 *  @fn			SetIndexBuffer
		 *  @brief		インデックスバッファの設定
		 *  @param[in]	argContext	!<	コンテキスト
		 */
		void SetIndexBuffer(ID3D11DeviceContext *argContext);
		inline std::vector<unsigned int> Indices() const { return indices_; }
		inline void Indices(std::vector<unsigned int> argIndices) { indices_ = argIndices; }
	public:
		/**
		 *	@fn			operator&
		 *	@brief		読み取り用operator
		 *	@return		バッファ
		 */
		const ID3D11Buffer* const* operator&()const;
		/**
		 *	@fn			operator&
		 *	@brief		書き込み用operator
		 *	@return		バッファ
		 */
		ID3D11Buffer** operator&();
	};
}