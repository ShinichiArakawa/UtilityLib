/**
 *	@file	InputLayout.cpp
 *	@date	2017 / 10 / 06
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "InputLayout.h"
#include "../../Debug/Debug.h"
#include "../../String/String.h"

#include <assert.h>
#include <d3d11.h>
#include <vector>

Utility::InputLayout::InputLayout(ID3D11Device *argDevice, const char * argVertexShaderData, size_t argVertexShaderSize)
{
	Create(argDevice, &inputLayout_, argVertexShaderData, argVertexShaderSize);
}

void Utility::InputLayout::SetInputLayout(ID3D11DeviceContext *argContext)
{
	argContext->IASetInputLayout(inputLayout_.Get());
}

HRESULT Utility::InputLayout::Create(ID3D11Device * argDevice, ID3D11InputLayout ** argInputLayout, const char * argVertexShaderData, size_t argVertexShaderSize)
{
	//入力構造体の情報ある位置へのポインタを取得
	unsigned char *structPtr = nullptr; // !<	入力構造体の情報がある位置へのポインタ
	for (unsigned long i = 0L; i < argVertexShaderSize - 4; i++)
	{
		if (memcmp(argVertexShaderData + i, "ISGN", 4) == NULL)
		{
			structPtr = reinterpret_cast<unsigned char*>(const_cast<char*>(argVertexShaderData) + i);
			break;
		}
	}
	if (!structPtr)
	{
		OutputDebugString(L"Not Found Input Struct...\n");
		return FWP_E_NOT_FOUND;
	}
	// 入力構造体の情報を取得
	int variableCnt = structPtr[8];					//	!<	変数の数
	int variablesystemCnt = 0;						//	!<	システム値セマンティクスが割り当てられた変数の数
	std::vector<char*> semanticsName(variableCnt);	//	!<	セマンティクス名
	std::vector<unsigned int> semanticsIndex(variableCnt);	//	!<	セマンティクスインデックス
	std::vector<DXGI_FORMAT> format(variableCnt);	//	!<	変数の型
	const unsigned char *const Str = &structPtr[16];
	for (int i = 0; i < variableCnt; i++)
	{
		semanticsName[i] = reinterpret_cast<char*>(Str[i * 24] + structPtr + 8);
		if (strcmp(semanticsName[i], "SV_InstanceID") == 0)
		{//システム値セマンティクスが付いている変数は無視
			variablesystemCnt++;
			continue;
		}

		semanticsIndex[i] = Str[i * 24 + 4];
		switch (Str[i * 24 + 20])
		{
		case '\x0f':// 4次元
			switch (Str[i * 24 + 12])
			{
			case D3D_REGISTER_COMPONENT_FLOAT32:
				format[i] = DXGI_FORMAT_R32G32B32A32_FLOAT;
				break;
			case D3D_REGISTER_COMPONENT_UINT32:
				format[i] = DXGI_FORMAT_R32G32B32A32_UINT;
				break;
			default:
				format[i] = DXGI_FORMAT_UNKNOWN;
				break;
			}// switch (Str[i * 24 + 12])
			break;// case '\x0f'
		case '\x07':// 3次元
			switch (Str[i * 24 + 12])
			{
			case D3D_REGISTER_COMPONENT_FLOAT32:
				format[i] = DXGI_FORMAT_R32G32B32_FLOAT;
				break;
			default:
				format[i] = DXGI_FORMAT_UNKNOWN;
				break;
			}// switch (Str[i * 24 + 12])
			break;// case '\x07'
		case '\x03':// 2次元
			switch (Str[i * 24 + 12])
			{
			case D3D_REGISTER_COMPONENT_FLOAT32:
				format[i] = DXGI_FORMAT_R32G32_FLOAT;
				break;
			default:
				format[i] = DXGI_FORMAT_UNKNOWN;
				break;
			}// switch (Str[i * 24 + 12])
			break;// case '\x03'
		case '\x01':// 1次元
			switch (Str[i * 24 + 12])
			{
			case D3D_REGISTER_COMPONENT_FLOAT32:
				format[i] = DXGI_FORMAT_R32_FLOAT;
				break;
			case D3D_REGISTER_COMPONENT_UINT32:
				format[i] = DXGI_FORMAT_R32_UINT;
				break;
			default:
				format[i] = DXGI_FORMAT_UNKNOWN;
				break;
			}// switch (Str[i * 24 + 12])
			break;// case '\x01'
		default:
			format[i] = DXGI_FORMAT_UNKNOWN;
			break;
		}// switch (Str[i * 24 + 20])
	}//	foreach(variableCnt)
	variableCnt -= variablesystemCnt; // !<	変数の数からシステム値セマンティクスが付いている変数の数を引いて無かったものとする

	std::vector<D3D11_INPUT_ELEMENT_DESC> elementDesc(variableCnt);
	for (int i = 0; i < variableCnt; i++)
	{// 入力レイアウトオブジェクトを作成
		elementDesc[i] = { semanticsName[i], semanticsIndex[i], format[i],
			0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
#ifdef _DEBUG
		Utility::Debug::OutputDebugStringFormat(L"\nname:" + String::ToWstring(elementDesc[i].SemanticName) + L"\nindex:%u\nformat%d\nslot:%u\noffset:%u\nclass:%d\nstep:%u\n\n",
			elementDesc[i].SemanticIndex, elementDesc[i].Format, elementDesc[i].InputSlot,
			elementDesc[i].AlignedByteOffset, elementDesc[i].InputSlotClass, elementDesc[i].InstanceDataStepRate);
#endif // _DEBUG

	}

	// 入力レイアウトを作成
	HRESULT hr = argDevice->CreateInputLayout(elementDesc.data(), variableCnt, argVertexShaderData, argVertexShaderSize, argInputLayout);
	if (FAILED(hr))
	{
		OutputDebugString(L"Create Input Layout Failed...\n");
		return hr;
	}

	return hr;
}
const ID3D11InputLayout* const* Utility::InputLayout::operator&() const
{
	assert(inputLayout_.GetAddressOf());
	return inputLayout_.GetAddressOf();
}
ID3D11InputLayout ** Utility::InputLayout::operator&()
{
	assert(inputLayout_.GetAddressOf());
	return inputLayout_.GetAddressOf();
}
// CreateInputLayout
