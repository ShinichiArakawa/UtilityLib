/**
 *	@file   Icon.h
 *	@date   2017 / 10 / 25
 *	@author Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <Windows.h>
#include <memory> 

namespace Utility
{
	class Archive;

	struct IconByteInfo
	{
	public:
		char *Memory = nullptr;
		unsigned int ByteSize = 0;
	};

	class Icon
	{
	public:
		/**
		 *	@fn			GetSmallIcon
		 *	@brief		小さいサイズのアイコン取得
		 *	@param[in]	argHwnd	!<	ウィンドウハンドル
		 *	@return		小さいサイズのアイコン
		 */
		static HICON GetSmallIcon(HWND argHwnd);
		/**
		 *	@fn			GetLargeIcon
		 *	@brief		大きいサイズのアイコン取得
		 *	@param[in]	argHwnd	!<	ウィンドウハンドル
		 *	@return		大きいサイズのアイコン
		 */
		static HICON GetLargeIcon(HWND argHwnd);
		/**
		 *	@fn			Load
		 *	@brief		アイコンをリソースIDから取得
		 *	@param[in]	argIconId	!<	アイコンのId
		 *	@return		IDに紐づけされたアイコン
		 */
		static HICON Load(unsigned short argIconId);
		/**
		 *	@fn			Load
		 *	@brief		アイコンをアーカイブから取得
		 *	@param[in]	argArchive	!<	アーカイブ
		 *	@param[in]	argFileName	!<	ファイルパス
		 *	@return		アイコン
		 */
		static HICON Load(Archive *argArchive, const char* argFileName);
		/**
		 *	@fn			Load
		 *	@brief		アイコンをアイコンのデータから取得
		 *	@param[in]	argIconDescription	!<	アイコンのデータ
		 *	@return		アイコン
		 */
		static HICON Load(const char* argIconDescription);
		
		static IconByteInfo SaveIconData(HICON argIcon);
		static void Export(const char*argExportPath, unsigned short argIconId);
	};
}