/**
 *	@file	InputManager.h
 *	@date	2017 / 8 / 31
 *	@author	Katsumi Takei
 *	Copyright (argTarget) Kastumi Takei. All rights reserved.
 *	@note	http://www.slis.tsukuba.ac.jp/~fujisawa.makoto.fu/cgi-bin/wiki/index.php?string%A4%CB%A4%E8%A4%EB%CA%B8%BB%FA%CE%F3%BD%E8%CD%FD#y2302683
 */
#pragma once

#include <string>
#include <vector>
#include <windows.h>
#include <sstream>

namespace Utility
{
	class String
	{
	public:
		/**
		 *	@fn			IsInteger
		 *  @brief		文字列が整数値を表しているかを調べる
		 *  @param[in]	argSrc		!<	 文字列
		 *  @return		整数値ならtrue
		 */
		inline static bool IsInteger(const std::string &argStr)
		{
			if (argStr.find_first_not_of("-0123456789 \t") != std::string::npos)
				return false;

			return true;
		}

		/**
		 *	@fn			IsNumeric
		 *  @brief		文字列が実数値を表しているかを調べる
		 *  @param[in]	argSrc		!<	 文字列
		 *  @return		実数値ならtrue
		 */
		inline static bool IsNumeric(const std::string &argStr)
		{
			if (argStr.find_first_not_of("-0123456789. Ee\t") != std::string::npos)
				return false;

			return true;
		}
		/**
		 *	@fn			DeleteSpace
		 *  @brief		空白(スペース，タブ)を削除
		 *  @param[in]	buf		!<	 処理文字列
		 *	@return		変換後の文字列
		 */
		inline static std::string DeleteSpace(const std::string &argBuf)
		{
			size_t pos;
			std::string buf = argBuf;
			while ((pos = buf.find_first_of(" 　\t")) != std::string::npos)
				buf.erase(pos, 1);
			
			return buf;
		}
		/**
		 *	@fn			GetFileName
		 *  @brief		パスからファイル名のみ取り出す
		 *  @param[in]	argPath	!<	  パス
		 *	@return		ファイル名
		 */
		inline static std::string GetFileName(const std::string &argPath)
		{
			size_t pos = argPath.rfind('\\');
			if (pos != std::string::npos) 
				return argPath.substr(pos + 1, argPath.size() - pos - 1);
			
			pos = argPath.rfind('/');
			if (pos != std::string::npos) 
				return argPath.substr(pos + 1, argPath.size() - pos - 1);

			return argPath;
		}
		/**
		 *	@fn			GetFolderPath
		 *  @brief		パスからファイル名を取り除いたパスを抽出
		 *  @param[in]	argPath	!<	 パス
		 *	@return		フォルダパス
		 */
		inline static std::string GetFolderPath(const std::string &argPath)
		{
			size_t pos = argPath.rfind('\\');
			if (pos != std::string::npos) 
				return argPath.substr(0, pos + 1);


			pos = argPath.rfind('/');
			if (pos != std::string::npos) 
				return argPath.substr(0, pos + 1);
			
			return "";
		}
		/**
		 *	@fn			GetParentFolderName
		 *  @brief		パスからファイルの親フォルダ名を取り出す
		 *  @param[in]	argPath	!<	 パス
		 *	@return		親フォルダ名
		 */
		inline static std::string GetParentFolderName(const std::string &argPath)
		{
			std::string::size_type pos1, pos0;
			pos1 = argPath.find_last_of("\\/");
			pos0 = argPath.find_last_of("\\/", pos1 - 1);

			if (pos0 != std::string::npos && pos1 != std::string::npos) 
			{
				return argPath.substr(pos0 + 1, pos1 - pos0 - 1);
			}
			else 
			{
				return "";
			}
		}
		/**
		 *	@fn			GetExtension
		 *  @brief		パスから拡張子を小文字にして取り出す
		 *  @param[in]	argPath	!<	 ファイルパス
		 *	@return		(小文字化した)拡張子
		 */
		inline static std::string GetExtension(const std::string &argPath)
		{
			std::string ext;
			size_t pos1 = argPath.rfind('.');
			if (pos1 != std::string::npos) 
			{
				ext = argPath.substr(pos1 + 1, argPath.size() - pos1);
				std::string::iterator itr = ext.begin();
				while (itr != ext.end()) 
				{
					*itr = static_cast<char>(tolower(*itr));
					itr++;
				}
				itr = ext.end() - 1;
				while (itr != ext.begin()) 
				{    // パスの最後に\0やスペースがあったときの対策
					if (*itr == 0 || *itr == 32) 
					{
						ext.erase(itr--);
					}
					else 
					{
						itr--;
					}
				}
			}

			return ext;
		}
		/**
		 *	@fn			ExtractPathWithoutExt
		 *  @brief		ファイル名から拡張子を削除
		 *  @param[in]	argFn	!<	 ファイル名(フルパス or 相対パス)
		 *	@return		フォルダパス
		 */
		inline static std::string ExtractPathWithoutExt(const std::string &argFn)
		{
			std::string::size_type pos;
			if ((pos = argFn.find_last_of(".")) == std::string::npos) 
				return argFn;

			return argFn.substr(0, pos);
		}
		/**
		 *	@fn			ExtractFileName
		 *  @brief		ファイル名を抽出(拡張子を除くフラグ付き)
		 *  @param[in]	argPath				!<	 パス
		 *  @param[in]	argWithoutExtension	!<	 拡張子を除くフラグ
		 *	@return		ファイル名
		 */
		inline static std::string ExtractFileName(const std::string &argPath, bool argWithoutExtension = true)
		{
			std::string fn;
			std::string::size_type fpos;
			if ((fpos = argPath.find_last_of("/")) != std::string::npos) 
			{
				fn = argPath.substr(fpos + 1);
			}
			else if ((fpos = argPath.find_last_of("\\")) != std::string::npos)
			{
				fn = argPath.substr(fpos + 1);
			}
			else 
			{
				fn = argPath;
			}

			if (argWithoutExtension && (fpos = fn.find_last_of(".")) != std::string::npos) 
				fn = fn.substr(0, fpos);

			return fn;
		}

		/**
		 *	@fn			CountString
		 *  @brief		文字列に含まれる指定された文字列の数を返す
		 *  @param[in]	argSrc		!<	 元の文字列
		 *  @param[in]	argTarget	!<	 検索文字列
		 *	@return		含まれる数
		 */
		inline static int CountString(const std::string &argSrc, int offset, std::string argTarget)
		{
			int count = 0;
			std::string src = argSrc;
			size_t pos0 = offset, pos = 0;
			int n = static_cast<int>(argTarget.size());

			while ((pos = src.find(argTarget, pos0)) != std::string::npos)
			{
				if (pos != pos0)
				{
					count++;
				}
				else 
				{
					src.erase(src.begin() + pos);
				}
				pos0 = pos + n;
			}

			// 最後の文字列除去
			if (src.rfind(argTarget) == src.size() - n) 
				count--;
			
			return count;
		}

		/**
		 *	@fn			RxToString
		 *	@brief		様々な型のstringへの変換(stringstreamを使用)
		 *  @param[in]	arg	!<	入力
		 *  @return		string型に変換したもの
		 */
		template<typename T>
		inline static std::string RxToString(const T &arg)
		{
			std::stringstream ss;
			ss << arg;
			return ss.str();
		}
	    /**
		 *	@fn			ToWstring
		 *	@brief		stringからwstringへの変換
		 *	@param[in]	argSrc				!<	変換したい文字列
		 * 	@return		変換したワイド文字列
		 */
		inline static std::wstring ToWstring(const std::string argStr)
		{
			size_t size;
			if (0 != mbstowcs_s(&size, nullptr, 0, argStr.c_str(), 0))
				return nullptr;

			std::vector<wchar_t> wch(size);
			if (0 != mbstowcs_s(&size, wch.data(), wch.size(), argStr.c_str(), argStr.length()))
				return nullptr;

			return wch.data();
		}
		/**
		 *	@fn			ToString
		 *	@brief		wstringからstringへの変換
		 *	@param[in]	wstr			!<	変換したいワイド文字列
		 * 	@return		変換した文字列
		 */
		inline static std::string ToString(const std::wstring argWstr)
		{
			// wstring → SJIS
			int size = WideCharToMultiByte(CP_OEMCP, 0, argWstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
			char* buffer = new char[size];

			WideCharToMultiByte(CP_OEMCP, 0, argWstr.c_str(), -1, buffer, size, nullptr, nullptr);

			std::string str(buffer, buffer + size - 1);
			delete []buffer;

			return str;
		}
		/**
		 *	@fn			DeleteSubStr
		 *  @brief		文字列の後ろから指定の文字までの削除
		 *  @param[in]	argSrc	!<	 文字列
		 *  @param[in]	argChar	!<	 指定の文字
		 *	@return		文字列
		 */
		inline static std::string DeleteSubStr(const std::string &argSrc, const char* argChar)
		{
			std::string::size_type pos;
			if ((pos = argSrc.find_last_of(argChar)) == (std::string::npos - 1))
				return argSrc;

			return argSrc.substr(0, pos + 1);
		}
		/**
		 *	@fn			Replace
		 *	@brief		文字列中から文字列を検索して別の文字列に置換する
		 *	@param[in]	argSrc  !< 置換対象の文字列
		 *	@param[in]	argFrom !< 検索文字列
		 *	@param[in]	argTo   !< 置換後の文字列
		 * 	@return		変換した文字列
		 */
		inline static std::string Replace(const std::string& argSrc, const std::string& argFrom, const std::string& argTo)
		{
			std::string src = argSrc;
			std::string::size_type pos = 0;
			while (pos = src.find(argFrom, pos), pos != std::string::npos)
			{
				src.replace(pos, argFrom.length(), argTo);
				pos += argTo.length();
			}
			return src;
		}
		/**
		 *	@fn			StrToInt
		 *	@brief		文字列を整数型に変換する
		 *	@param[in]	argStr	!<	整数型に変換する文字列
		 *	@return		整数
		 */
		inline static const unsigned int StrToInt(const char *argStr)
		{
			return *reinterpret_cast<const unsigned int *>(argStr);
		}
		/**
		 *	@fn			CmpStr
		 *	@brief		文字列の結合
		 *	@param[in]	argLeft		!<	結合する文字列
		 *	@param[in]	argRight	!<	結合する文字列
		 *	@return		結合した文字列
		 */
		inline static int CmpStr(const char *argLeft, const char *argRight)
		{
			return std::strcmp(argLeft, argRight);
		}
		/**
		 *	@fn			CmpStr
		 *	@brief		文字列の結合
		 *	@param[in]	argLeft		!<	結合する文字列
		 *	@param[in]	argRight	!<	結合する文字列
		 *	@return		結合した文字列
		 */
		inline static int CmpStr(const wchar_t *argLeft, const wchar_t *argRight)
		{
			return std::wcscmp(argLeft, argRight);
		}
	};
}