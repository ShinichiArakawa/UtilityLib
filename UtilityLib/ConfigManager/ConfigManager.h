/**
 *	@file	ConfigManager.h
 *	@date	2017 / 10 / 09
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>

namespace Utility
{
	class ConfigManager final
	{
	private:

		/**
		 *  @class		Constant
		 *  @brief		コンスタント
		 */
		class Constant
		{
		public:
			char name_[256];
			unsigned char data_[256];
		public:

			/**
			 *  @fn		GetIntData
			 *  @brief	ロードしたデータを整数で取得
			 *  @return	ロードした整数のデータ
			 */
			const int GetIntData()
			{
				return std::stoi(reinterpret_cast<char(&)[sizeof(data_)]>(data_));
			}
		};
	private:
		static std::string dataPath_;
		bool canUseArchive_ = false;
		std::vector<Constant> data_;
		int dataIndex_ = 0;
		std::map<std::string, int> checker_;
		static const int Key_ = 12;
	private:
		/**
		 *  @fn			LoadArchive
		 *  @brief		アーカイブからのコンフィグファイルのロード
		 *  @param[in]	argFilename	!<	ファイルのパス
		 *  @param[in]	argIsDecode	!<	復号化するか
		 */
		void LoadArchive(const char * argFileName, bool argIsDecode);

	public:
		ConfigManager() = default;
		virtual ~ConfigManager() = default;
	public:

		/**
		 *  @fn			Import
		 *  @brief		コンフィグファイルのインポート
		 *  @param[in]	argFilename	!<	ファイルのパス
		 *  @param[in]	argIsDecode	!<	復号化するか
		 */
		void Import(const char *argFilename, bool argIsDecode);
		/**
		 *  @fn			Export
		 *  @brief		コンフィグファイルのエクスポート
		 *  @param[in]	argFilename	!<	ファイルのパス
		 *  @param[in]	argIsDecode	!<	暗号化するか
		 */
		void Export(const char *argFilename, bool argIsEncode);
		
		/**
		 *	@fn			CanUseArchive
		 *	@brief		アーカイブの設定
		 *	@param[in]	argCanUseArchive	!<	アーカイブを使用するか
		 */
		void CanUseArchive(bool argCanUseArchive);
		/**
		 *  @fn			CanGetData
		 *  @brief		データが取得できるか
		 *  @param[in]	argIndex	!<	データのインデックス
		 *  @param[in]	argKey		!<	データのキー
		 *	@retval		true		!<	取得可能
		 *	@retval		false		!<	取得不能
		 */
		inline bool CanGetData(int argIndex, std::string argKey) const { return (Name(argIndex) == argKey) ? true : false; }
		/**
		 *  @fn			IntData
		 *  @brief		整数のデータの取得
		 *  @param[in]	argIndex	!<	データのインデックス
		 *  @return		整数のデータ
		 */
		inline const int IntData(int argIndex) { return data_[argIndex].GetIntData(); }

	public:
		/**
		 *  @fn			IntData
		 *  @brief		整数のデータの取得
		 *  @param[in]	argIndex	!<	データのインデックス
		 *  @return		整数のデータ
		 */
		const int IntData(std::string argKey);
		/**
		 *  @fn			Name
		 *  @brief		データの名前の取得
		 *  @param[in]	argIndex	!<	データのインデックス
		 *  @return		データの名前
		 */
		inline const char *Name(int argIndex) const { return data_[argIndex].name_; }
		/**
		 *  @fn			Data
		 *  @brief		整数のデータの取得
		 *  @param[in]	argIndex	!<	データのインデックス
		 *  @return		データの整数
		 */
		inline const unsigned char *Data(int argIndex) const { return data_[argIndex].data_; }
		/**
		 *  @fn			DataSize
		 *  @brief		データ数の取得
		 *  @return		データの数
		 */
		inline size_t DataSize() const{ return data_.size(); }

		/**
		 *	@fn			DataPath
		 *	@brief		データのパスの設定
		 *	@param[in]	argDataPath	!<	データのパス
		 */
		static void DataPath(std::string argDataPath);
		/**
		 *	@fn			DataPath
		 *	@brief		データのパスを返す
		 *	@return		データのパス
		 */
		static std::string DataPath();

	};
}