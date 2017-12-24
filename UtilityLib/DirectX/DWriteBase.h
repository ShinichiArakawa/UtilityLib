/**
 *	@file   DWriteBase.h
 *	@date   2017 / 07 / 15
 *	@author Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <memory>
#include <dwrite.h>

namespace Utility
{
	class DWriteBase
	{
	private:
		class Impl;
		std::unique_ptr<Impl> pImpl;
	public:
		DWriteBase();
		~DWriteBase();

		/**
		 *	@fn		ImageFactory
		 *	@brief	IDWriteFactory型のポインタの取得
		 *	@return	IDWriteFactoryのポインタ
		 */
		IDWriteFactory* ImageFactory();
	};
};