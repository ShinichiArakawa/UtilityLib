/**
 *	@file	SoundManager.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <memory>
#include <string>

namespace Utility
{
	/**
	 *  @enum		eSoundType
	 *  @brief		サウンドの再生形式
	 */
	enum class eSoundType
	{
		Se,
		Loop,
	};

	class SoundManager final
	{
	private:
		class Impl;
		std::unique_ptr<Impl> pImpl;

	public:
		SoundManager();
		~SoundManager();

	public:
		/**
		 *	@fn			AddAndLoadSound
		 *	@brief		サウンドの読み込み
		 *	@param[in]	argFilename	読み込むファイルの名前
		 *	@param[in]	argBuffer	読み込むファイルのバイナリデータ
		 */
		void AddAndLoadSound(std::string argFilename, void *argBuffer);
		/**
		 *	@fn			AddAndLoadSound
		 *	@brief		サウンドの読み込み
		 *	@param[in]	argFilename	読み込むファイルの名前
		 */
		void AddAndLoadSound(std::string argFilename);
		/**
		 *	@fn			PlaySounds
		 *	@brief		サウンドの再生
		 *	@param[in]	argKey	再生したいサウンドのキー
		 *	@param[in]	argType	再生形式
		 */
		void PlaySounds(std::string argKey, eSoundType argType)const;
		/**
		 *	@fn			StopSounds
		 *	@brief		サウンドの停止
		 *	@param[in]	argKey	停止したいサウンドのキー
		 */
		void StopSounds(std::string argKey)const;
		/**
		 *	@fn			StopCurrentBgm
		 *	@brief		現在再生しているサウンドの停止
		 */
		void StopCurrentBgm()const;
		/**
		 *	@fn			SoundPath
		 *	@brief		サウンドのパスを設定する
		 *	@param[in]	argPath	!<	パスの設定
		 */
		void SoundPath(std::string argPath)const;
		/**
		 *	@fn			SoundPath
		 *	@brief		サウンドのパスを取得
		 *	@return		パス
		 */
		std::string SoundPath()const;
		/**
		 *	@fn			CanUseArchive
		 *	@brief		アーカイブの設定
		 *	@param[in]	パス
		 */
		void CanUseArchive(bool argCanUseArchive);

		/**
		 *	@fn			CurrentPlayBgm
		 *	@brief		現在再生しているBGM
		 *	@return		再生しているBGMの名前
		 */
		std::string CurrentPlayBgm()const;

	};
};