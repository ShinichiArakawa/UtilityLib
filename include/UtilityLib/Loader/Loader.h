/**
 *	@file	Loader.h
 *	@date	2017 / 10 / 09
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <vector>

namespace Utility
{
	class File;

	class Loader final
	{
	private:

		std::vector<File*> files_;	//	!<	読み込んだファイル
		std::vector<char> buffer_;	//	!<	読み込んだ一時データ

		/**
		 *	@fn			FileSize
		 *	@brief		ファイルサイズの取得
		 *	@param[in]	argStream	!<	ファイルを読み込んだストリーム
		 *	@return		ファイルのサイズ
		 */
		size_t FileSize(std::istream* argStream)const;
		/**
		 *	@fn			Load
		 *	@brief		一時データのロード
		 *	@param[in]	argStream	!<	ファイルを読み込んだストリーム
		 *	@param[in]	argSize		!<	ファイルサイズ
		 */
		bool Load(std::istream *argStream, size_t argSize);
	public:
		Loader();
		~Loader();

		/**
		 *	@fn			SetFile
		 *	@brief		読み込みたいファイルのセット
		 *	@param[in]	argFile		!<	ファイル
		 *	@param[in]	argFileName	!<	ファイルのパス
		 */
		void SetFile(File** argFile, const char *argFileName);
		/**
		 *	@fn			DestroyFile
		 *	@brief		ファイルの消去
		 *	@param[in]	argFile	!<	ファイル
		 */
		void DestroyFile(File** argFile);
		/**
		 *	@fn		LoadAtThread
		 *	@brief	セットしたファイルの読み込み
		 */
		void LoadAtThread();
		/**
		 *	@fn		LoadAtThreadUTF
		 *	@brief	セットしたファイルの読み込み
		 */
		void LoadAtThreadUTF();
		/**
		 *	@fn			Load
		 *	@brief		一時データの読み込み
		 *	@param[in]	argFileName	!<	ファイルパス
		 *	@retval		true		!<	成功
		 *	@retval		false		!<	失敗
		 */
		bool Load(const char* argFileName);
		/**
		 *	@fn			Load
		 *	@brief		一時データの読み込み
		 *	@param[in]	argFileName	!<	ファイルパス
		 *	@retval		true	!<	成功
		 *	@retval		false	!<	失敗
		 */
		bool Load(const wchar_t* argFileName);
		/**
		 *	@fn		Data
		 *	@brief	一時データの取得
		 *	@return	一時データ
		 */
		const char* Data()const;
		/**
		 *	@fn		Size
		 *	@brief	一時データのサイズの取得
		 *	@return	一時データのサイズ
		 */
		size_t Size()const;
		/**
		 *	@fn			Getval
		 *	@brief		読込み可能かチェックしながら値を読込む
		 *	@param[in]	pifs	ファイルストリームへのポインタ
		 *　@param[in]	eofPos	ファイルストリームの終了位置
		 *　@param[in]	v　		読込むデータタイプ
		 *	@return		成功ならtrue、失敗したら例外で読込みサイズエラー
		 */
		template <typename T>
		inline bool GetVal(std::ifstream * pifs, std::streamoff eofPos, T& v)
		{
			std::streamoff size = sizeof(T);
			std::streamoff pos = pifs->tellg();
			if (eofPos > size + pos)
			{
				pifs->read((char*)&v, size);
				return true;
			}
			throw TEXT("読込みサイズエラー");
			return false;
		}
		/**
		 *	@fn			Setval
		 *	@brief		書込みを行う
		 *	@param[in]	pofs	ファイルストリームへのポインタ
		 *　@param[in]	v　		書込むデータタイプ
		 *	@return		成功ならtrue、失敗したら例外で書込みエラー
		 */
		template <typename T>
		inline bool SetVal(std::ofstream * pofs, T& v)
		{
			try
			{
				pofs->write((char*)&v, sizeof(T));
			}
			catch (char *)
			{
				throw TEXT("ファイル書込みエラー");
				return false;
			}
			return true;
		}
	};

};