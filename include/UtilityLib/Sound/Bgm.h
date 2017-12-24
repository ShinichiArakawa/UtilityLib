/**
 *	@file	Bgm.h
 *	@date	2017 / 10 / 04
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once
namespace Utility
{
#ifdef __cplusplus_cli
#pragma unmanaged
#endif

	/**
	 *	@class		BGM
	 *	@brief		BGM関連のクラス
	 */
	class Bgm
	{
	public:
		Bgm();
		~Bgm();

	public:
		/**
		 *	@fn			Loab
		 *	@brief		メディアをロードする
		 *	@param[in]	argFilename	!<	ファイルパス
		 *	@retval		true		!<	成功
		 *	@retval		false		!<	失敗
		 */
		bool Loab(wchar_t* argFilename);
		/**
		 *	@fn			Unload
		 *	@brief		メディアをアンロードする
		 *	@retval		true		!<	成功
		 *	@retval		false		!<	失敗
		 */
		bool Unload();
		/**
		 *	@fn			Play
		 *	@brief		メディアを再生する(繰り返し)
		 *	@retval		true		!<	成功
		 *	@retval		false		!<	失敗
		 */
		bool Play();
		/**
		 *	@fn			Stop
		 *	@brief		再生中のメディアを停止する
		 *	@retval		true		!<	成功
		 *	@retval		false		!<	失敗
		 */
		bool Stop();
		/**
		 *	@fn			Pause
		 *	@brief		再生中のメディアをポーズする
		 *	@retval		true		!<	成功
		 *	@retval		false		!<	失敗
		 */
		bool Pause();
		/**
		 *	@fn			Resume
		 *	@brief		ポーズ中のメディアを再生する
		 *	@retval		true		!<	成功
		 *	@retval		false		!<	失敗
		 */
		bool Resume();
		/**
		 *	@fn			Volume
		 *	@brief		ボリュームの変更
		 *	@param[in]	volume	!<	0～1000のボリューム
		 *	@retval		true	!<	成功
		 *	@retval		false	!<	失敗
		 */
		bool Volume(int argVolume);

	};

#ifdef __cplusplus_cli
#pragma managed
#endif
};