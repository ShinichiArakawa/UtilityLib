/**
 *	@file   Graphic3DBase.h
 *	@date   2017 / 08 / 31
 *	@author Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <memory>
#include "Shader/ShaderSummary.h"

namespace Utility
{
	class Graphic3DBase
	{
	private:
		class Impl;
		std::unique_ptr<Impl> pImpl;

	public:
		Graphic3DBase();
		~Graphic3DBase();

	public:
		/**
		 *	@fn			AddAndLoadModel
		 *	@brief		モデルキャッシュにモデルを登録する
		 *	@param[in]	argModelFilename	!<	モデルのパス
		 *	@return		3Dグラフィックスのマネージャー
		 */
		Graphic3DBase *AddAndLoadModel(const std::string argModelFilename);

	public:

#pragma region		Extension
	public:
		/**
		 *	@fn		Pmd
		 *	@brief	Pmdをモデルキャッシュに登録
		 */
		void Pmd();
		/**
		 *	@fn		Fbx
		 *	@brief	Fbxをモデルキャッシュに登録
		 */
		void Fbx();
#pragma endregion	Extension

	};//	class Graphic3DBase
};