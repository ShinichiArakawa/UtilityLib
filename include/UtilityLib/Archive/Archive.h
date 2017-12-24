/**
 *	@file	SetArchive.h
 *	@date	2017 / 10 / 04
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <map>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

struct ID3D11Device;

namespace Utility
{
	class Texture;

	class Archive
	{
	private:

		/**
		 *  @struct	Entry
		 *  @brief	エントリー
		 */
		struct Entry
		{
			int Position;
			int Size;
		};
		std::unique_ptr<std::ifstream> stream_;
		std::map<std::string, Entry> entries_;
		std::vector<char*> deleteList_;
	public:

		/**
		 *  @constructor	SetArchive
		 *  @brief			アーカイブ
		 *	@param[in]		argArchiveName	!<	アーカイブのパス
		 */
		Archive(const char* argArchiveName);
		Archive();
		~Archive();
	public:
		/**
		 *  @fn			Import
		 *  @brief		インポート
		 *	@param[in]	argArchiveName	!<	アーカイブのパス
		 */
		void Import(const char* argArchiveName);
		/**
		 *  @fn			Export
		 *  @brief		エクスポート
		 *	@param[in]	argDirectoryName	!<	書き出すディレクトリ
		 *	@param[in]	argArchiveName		!<	書き出すアーカイブのネーム
		 */
		static void Export(const std::string &argDirectoryName, const char* argArchiveName = "Archive.dat");
		/**
		 *  @fn			IsImported
		 *  @brief		インポートしていたか
		 *	@retval		true	!<	インポート済み
		 *	@retval		false	!<	インポートしていない
		 */
		bool IsImported()const;
	public:
		/**
		 *  @fn			Read
		 *  @brief		データの読み込み
		 *  @param[in]	argFileName	!<	ファイルのパス
		 *  @param[in]	argFileData	!<	データを格納する変数
		 *  @param[in]	argFileSize	!<	データのサイズを格納する変数
		 */
		void Read(const char *argFileName, char** argFileData, int *argFileSize);

	public:
		/**
		 *  @fn			LoadText
		 *  @brief		テキストの読み込み
		 *	@param[in]	argFileName	!<	ファイルのパス
		 *	@return		テキストデータ
		 */
		const char *LoadText(const char *argFileName);

	};
};