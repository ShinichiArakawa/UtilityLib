/**
 *	@file	Shader.h
 *	@date	2017 / 8 / 27
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <string>
#include <functional>

namespace Utility
{
	class CShader abstract
	{
	public:
		virtual ~CShader() = default;
	public:
		static std::string csoPath_;
		static bool canUseArchive_;
	public:
		/**
		 *  @fn			CsoPath
		 *  @brief		シェーダーのパスを指定する
		 *  @param[in]	argPath	!<	パス
		 */
		static void CsoPath(std::string argPath);
		/**
		 *  @fn			CsoPath
		 *  @brief		シェーダーのパスを取得する
		 */
		static std::string CsoPath();

		static void CanUseArchive(bool argCanUseArchive);
	};

	template<typename T>
	class Shader abstract : CShader
	{
	protected:

		Microsoft::WRL::ComPtr<T> shader_;
		std::function<HRESULT(ID3D11DeviceContext*, const char*, const size_t)> create_;
	public:

		Shader()
			: create_([=](ID3D11DeviceContext * argContext, const char * argMemory, const size_t argSize) { return Create(argContext, argMemory, argSize); })
		{
		}
		virtual ~Shader() = default;
	protected:

		virtual void SetShader(ID3D11DeviceContext *argContext) = 0;
		virtual HRESULT Create(ID3D11DeviceContext *argContext, const char * argMemory, const size_t argSize) = 0;
		HRESULT Create(ID3D11DeviceContext * argContext, const char * argFilename);
	
	};

};