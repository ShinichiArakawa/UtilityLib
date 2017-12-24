/**
 *	@file	OrgSound.h
 *	@date	2017 / 10 / 04
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <windows.h>
#include <memory>
#include <cstdint>

namespace Utility
{
	/**
	 *	@class		OrgSound
	 *	@brief		DirectSoundの管理クラス
	 */
	class OrgSound
	{

	private:
		class Impl;
		std::unique_ptr<Impl> pImpl;

	public:
		OrgSound();
		virtual ~OrgSound();
	public:
		/**
		 *	@fn			Create
		 *	@brief		インターフェイスを作成
		 *	@param[in]	argHWnd	!<	ファイルパス
		 *	@retval		true	!<	成功
		 *	@retval		false	!<	失敗
		 *	@note		DirectSoundのインターフェイスを協調モードで作成
		 */
		bool Create(HWND argHWnd);
		/**
		 *	@fn			Create
		 *	@brief		指定インデックスにバッファを作成
		 *	@param[in]	argBuf		!<	Wave形式のデータ
		 *	@param[in]	argIndex	!<	登録先のインデックス
		 *	@retval		true		!<	成功
		 *	@retval		false		!<	失敗
		 */
		bool CreateBuffer(void* argBuf, uint32_t argIndex);
		/**
		 *	@fn			Release
		 *	@brief		オブジェクトを解放
		 */
		void Release();
		/**
		 *	@fn			ReleaseBufferAll
		 *	@brief		すべてのバッファを開放する
		 */
		void ReleaseBufferAll();
		/**
		 *	@fn			ReleaseBuffer
		 *	@brief		指定したインデックスのバッファを開放
		 *	@param[in]	argIndex	!<	解放する音のインデックス
		 */
		void ReleaseBuffer(uint32_t argIndex);
		/**
		 *	@fn			SetPan
		 *	@brief		パンコントロール
		 *	@param[in]	argIndex	!<	解放する音のインデックス
		 *	@param[in]	argPan		!<	パンのインデックス
		 */
		void SetPan(uint32_t argIndex, int argPan);
		/**
		 *	@fn			SetVolume
		 *	@brief		ボリュームコントロール
		 *	@param[in]	argIndex	!<	解放する音のインデックス
		 *	@param[in]	argVolume	!<	ボリュームのインデックス
		 */
		void SetVolume(uint32_t agrIndex, int argVolume);	
		/**
		 *	@fn			SolvePause
		 *	@brief		ポーズを解除する
		 *	@return		ポーズを解除した音の数
		 */
		int  SolvePause();
		/**
		 *	@fn			Stop
		 *	@brief		再生中のバッファを停止する
		 *	@param[in]	argIndex	!<	停止する音のインデックス
		 *	@retval		true		!<	再生状態のバッファを指定
		 *	@retval		false		!<	停止状態のバッファを指定
		 */
		bool Stop(uint32_t argIndex);
		/**
		 *	@fn			GetPauseNum
		 *	@brief		ポーズされているバッファの数を取得
		 *	@return		ポーズされているバッファの数
		 */
		int  GetPauseNum();
		
		/**
 		 *	@fn		IsPause
		 *	@brief	ポーズされているか
		 *	@retval	true	!<	ポーズされている
		 *	@retval	false	!<	ポーズされていない
		 */
		bool IsPause();
		/**
		 *	@fn			PauseAll
		 *	@brief		すべての音をポーズする
		 *	@return		ポーズしたバッファの数
		 */
		int  PauseAll();
		/**
		 *	@fn			Replay
		 *	@brief		ストップした位置から再生する
		 *	@param[in]	argIndex	!<	再生する音のインデックス
		 *	@retval		true		!<	成功
		 *	@retval		false		!<	失敗
		 */
		bool Replay(uint32_t argIndex);
		/**
		 *	@fn			Play
		 *	@brief		指定した位置から再生する
		 *	@param[in]	argIndex	!<	再生する音のインデックス
		 *	@retval		true		!<	成功
		 *	@retval		fasle		!<	失敗
		 */
		bool Play(unsigned int argIndex);
		/**
		 *	@fn			Play
		 *	@brief		指定した位置から再生する
		 *	@param[in]	argIndex	!<	再生する音のインデックス
		 *	@param[in]	argPos		!<	再生する位置
		 *	@param[in]	argIsLoop	!<	ループするか
		 *	@retval		true		!<	成功
		 *	@retval		fasle		!<	失敗
		 */
		bool Play(unsigned int argIndex, int argPos, BOOL argIsLoop);


	};
};