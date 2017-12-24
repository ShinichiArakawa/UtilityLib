/**
 *	@file	Sound.h
 *	@date	2017 / 10 / 04
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <memory>
#include <windows.h>

namespace Utility
{
	class Sound
	{
	private:
		class Impl;
		std::unique_ptr<Impl> pImpl;
	public:
		Sound();
		~Sound();

	public:
		/**
		 *	@fn			Create
		 *	@brief		構築
		 *	@param[in]	argHwnd	!<	ウィンドウハンドル
		 *	@retval		true	!<	成功
		 *	@retval		false	!<	失敗
		 */
		bool Create(HWND argHwnd);
		/**
		 *	@fn			Release
		 *	@brief		解放
		 */
		void Release();
		/**
		 *	@fn			Load
		 *	@brief		サウンドの登録
		 *	@param[in]	argIndex	!<	登録するインデックス
		 *	@param[in]	argFilename	!<	ロードするファイルのパス
		 *	@retval		true		!<	成功
		 *	@retval		false		!<	失敗
		 */
		bool Load(uint32_t argIndex, const wchar_t* filename);
		/**
		 *	@fn			Load
		 *	@brief		サウンドの登録
		 *	@param[in]	argIndex	!<	登録するインデックス
		 *	@param[in]	argBuffer	!<	バッファのデータ
		 *	@retval		true		!<	成功
		 *	@retval		false		!<	失敗
		 */
		bool Load(uint32_t argIndex, void *argBuffer);
		/**
		 *	@fn			Play
		 *	@brief		再生
		 *	@param[in]	argIndex	!<	再生するサウンドのインデックス
		 */
		void Play(uint32_t argIndex);
		/**
		 *	@fn			PlayLoop
		 *	@brief		ループ再生
		 *	@param[in]	argIndex	!<	再生するサウンドのインデックス
		 */
		void PlayLoop(uint32_t argIndex);
		/**
		 *	@fn			Stop
		 *	@brief		停止
		 *	@param[in]	argIndex	!<	停止するサウンドのインデックス
		 */
		void Stop(uint32_t argIndex);


#pragma region		Static
	public:
		/**
		 *	@fn			Create
		 *	@brief		構築
		 *	@param[in]	argHwnd	!<	ウィンドウハンドル
		 */
		static void CreateS(HWND argHwnd);
		/**
		 *	@fn			Release
		 *	@brief		解放
		 */
		static void ReleaseS();
		/**
		 *	@fn			Load
		 *	@brief		サウンドの登録
		 *	@param[in]	argIndex	!<	登録するインデックス
		 *	@param[in]	argFilename	!<	ロードするファイルのパス
		 *	@retval		true		!<	成功
		 *	@retval		false		!<	失敗
		 */
		static bool LoadS(uint32_t argIndex, const char* argFilename);
		/**
		 *	@fn			Load
		 *	@brief		サウンドの登録
		 *	@param[in]	argIndex	!<	登録するインデックス
		 *	@param[in]	argBuffer	!<	バッファのデータ
		 *	@retval		true		!<	成功
		 *	@retval		false		!<	失敗
		 */
		static bool LoadS(uint32_t argIndex, void *argBuffer);
		/**
		 *	@fn			Play
		 *	@brief		再生
		 *	@param[in]	argIndex	!<	再生するサウンドのインデックス
		 */
		static void PlayS(int argIndex);
		/**
		 *	@fn			PlayLoop
		 *	@brief		ループ再生
		 *	@param[in]	argIndex	!<	再生するサウンドのインデックス
		 */
		static void PlayLoopS(int argIndex);
		/**
		 *	@fn			Stop
		 *	@brief		停止
		 *	@param[in]	argIndex	!<	停止するサウンドのインデックス
		 */
		static void StopS(int argIndex);

#pragma endregion	Static

	};

};