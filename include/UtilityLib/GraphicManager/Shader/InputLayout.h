/**
 *	@file	InputLayout.h
 *	@date	2017 / 10 / 06
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <wrl/client.h>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11InputLayout;

namespace Utility
{
	class InputLayout final
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout_;
	public:
		InputLayout(ID3D11Device *argDevice, const char *argVertexShaderData, size_t argVertexShaderSize);
		InputLayout() = default;
		~InputLayout() = default;
	public:
		void SetInputLayout(ID3D11DeviceContext *argContext);
	public:
		/**
		 *	@fn			Create
		 *	@brief		インプットレイアウトの生成
		 *	@param[in]	argInputLayout		!<	生成したインプットレイアウトを格納するバッファのポインタ
		 *	@param[in]	argVertexShaderData	!<	インプットレイアウトを適用する頂点シェーダーのデータ
		 *	@param[in]	argVertexShaderSize	!<	インプットレイアウトを適用する頂点シェーダーのサイズ
		 *	@retval		S_OK				!<	生成に成功
		 *	@retval		E_FAIL				!<	生成に失敗
		 *	@retval		FWP_E_NOT_FOUND		!<	入力構造体のポインタの取得に失敗
		 *	@attention	CreateInputeLayoutは頂点シェーダのコンパイル結果が必要
		 */
		static HRESULT Create(ID3D11Device *argDevice, ID3D11InputLayout **argInputLayout, const char *argVertexShaderData, size_t argVertexShaderSize);
	public:
		inline Microsoft::WRL::ComPtr<ID3D11InputLayout> GetInputLayout()const { return inputLayout_; }

	public:
		/**
		 *	@fn			operator&
		 *	@brief		読み取り用operator
		 *	@return		バッファ
		 */
		const ID3D11InputLayout* const* operator&()const;
		/**
		 *	@fn			operator&
		 *	@brief		書き込み用operator
		 *	@return		バッファ
		 */
		ID3D11InputLayout** operator&();
	};
}