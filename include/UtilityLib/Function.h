/**
 *	@file	Function.h
 *	@date	2017 / 10 / 04
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <string>
#include <windows.h>
#include <DirectXMath.h>

namespace Utility
{
	/**
	 *	@fn			IsInArea
	 *	@brief		座標が指定のエリアに入っているか
	 *	@param[in]	argPos	!<	座標
	 *	@param[in]	argArea	!<	エリア
	 *	@retval		true	!<	座標がエリア内に存在する
	 *	@retval		false	!<	座標がエリア内に存在しない
	 */
	inline bool IsInArea(DirectX::XMFLOAT2 argPos, RECT argArea)
	{
		if(argPos.x < argArea.left ||
			argPos.x > argArea.right || 
			argPos.y < argArea.top || 
			argPos.y > argArea.bottom)
		{
			return false;
		}
		return true;
	}

	/**
	 *	@fn			IsInRange
	 *	@brief		数値が指定の範囲に収まっているか
	 *	@param[in]	argValue	!<	数値
	 *	@param[in]	argRangeMin	!<	最小範囲
	 *	@param[in]	argRangeMax	!<	最大範囲
	 *	@retval		true	!<	数値が範囲内に存在する
	 *	@retval		false	!<	数値が範囲内に存在しない
	 */
	template<typename T>
	inline bool IsInRange(T argValue, T argRangeMin, T argRangeMax)
	{
		if (argRangeMin <= argValue && argValue <= argRangeMax)
		{
			return true;
		}
		return false;
	}

	/**
	 *	@fn			SafeDelete
	 *	@brief		引数のポインタを削除してnullperを代入する
	 *	@param[in]	argPinter	!<	安全に削除したいポインタ
	 */
	template <typename T>
	inline void SafeDelete(T*& p)
	{
		// 前方宣言しかない場合等に負の要素数となる配列を
		// 宣言することでコンパイラにエラーを出すことを強制する
		typedef char type_must_be_complete[(sizeof(T)) ? 1 : -1];
		(void)sizeof(type_must_be_complete);

		delete p;
		p = nullptr;

	}
	/**
	 *	@fn			SafeDeleteArray
	 *	@brief		引数のポインタを削除してnullperを代入する
	 *	@param[in]	argPinter	!<	安全に削除したい配列のポインタ
	 */
	template <typename T>
	inline void SafeDeleteArray(T*& p)
	{
		typedef char type_must_be_complete[(sizeof(T)) ? 1 : -1];
		(void)sizeof(type_must_be_complete);

		delete[]p;
		p = nullptr;
	}
	/**
	 *	@fn			SafeDeleteRelese
	 *	@brief		引数のポインタを削除してnullperを代入する
	 *	@param[in]	argPinter	!<	安全に削除したい標準オブジェクトのポインタ
	 */
	template <typename T>
	inline void SafeRelease(T*& p)
	{
		typedef char type_must_be_complete[(sizeof(T)) ? 1 : -1];
		(void)sizeof(type_must_be_complete);

		p->Release();
		p = nullptr;
	}

	namespace Error
	{
		/**
		 *	@fn			AssertMessageRelease
		 *	@brief		リリース版で強制終了させたいときに使う
		 *	@param[in]	argMessage	!<	ユーザーに報告して貰えるようなメッセージ
		 */
		inline void AssertMessageRelease(std::wstring argMessage)
		{
			MessageBox(nullptr, argMessage.c_str(), L"ErrorLog", MB_ICONERROR);
			abort();
		}
	}
}