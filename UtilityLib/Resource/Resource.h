/**
 *	@file	Resource.h
 *	@date	2017 / 10 / 21
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */

#pragma once

#include <string>

namespace Utility
{
	class Resource
	{
	public:
		/**
		 *	@fn			ExportByteData
		 *	@brief		バイナリを文字列にして出力する
		 *	@param[in]	argPath			!<	変換元のファイルのパス
		 *	@param[in]	argFilename		!<	変換元のファイルの名前
		 *	@param[in]	argExtention	!<	変換元のファイルの拡張子
		 */
		static void ExportByteData(std::string argPath, std::string argFilename, std::string argExtention);
		/**
		 *	@fn			ExportBinary
		 *	@brief		データをバイナリで出力する
		 *	@param[in]	argPath	!<	パスとファイルの名前
		 *	@param[in]	argSrc	!<	データ
		 */
		static void ExportBinary(std::string argExportPath, std::wstring argSrc);
		
	};
}