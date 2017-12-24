/**
 *	@file	GamePad.h
 *	@date	2017 / 10 / 4
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <windows.h>
#include <mmsystem.h>
#include <string>
#include <map>

namespace Utility
{
	enum class eKey;
	/**
	 *	@enum	eStick
	 *	@brief	スティックの指定
	 */
	enum eStick
	{
		Right,
		Left,
	};

	class GamePad final
	{
	private:
		/**
		 *	@enum	ePadButton
		 *	@brief	パッドボタンの入力値
		 */
		enum ePadButton : uint16_t
		{
			Left = 0x1,
			Top = 0x2,
			Under = 0x4,
			Right = 0x8,

			L1 = 0x10,
			R1 = 0x20,
			L2 = 0x40,
			R2 = 0x80,

		};
	private:
		JOYINFOEX joyInfoNow_;
		JOYINFOEX joyInfoOld_;
		unsigned int showJoystickId_;
		std::map<eKey, unsigned int> pad_;	//	パッドのキー登録
		bool isConnected_;
	private:
		void Initialize();
	public:

		/**
		 *  @constructor	GamePad
		 *  @brief			ゲームパッド
		 *  @param[in]		argPadNumber	!<	パッドの接続番号
		 */
		GamePad(unsigned int argPadNumber);
		GamePad();
		~GamePad();

	public:
		/**
		 *	@fn		Update
		 *	@brief	更新
		 */
		void Update();
		/**
		 *	@fn		IsConnectCheck
		 *	@brief	接続確認
		 *	@retval	true	!<	接続あり
		 *	@retval	false	!<	接続なし
		 */
		bool IsConnectCheck();
		/**
		 *	@fn			IsPressed
		 *	@brief		パッドボタンが押されているか
		 *	@param[in]	argStick	!<	スティックの指定
		 *	@param[in]	argKeyCode	!<	ボタンのキーコード
		 *	@retval		true		!<	押されている
		 *	@retval		false		!<	押されていない
		 */
		bool IsPressed(eStick argStick, eKey argKeyCode);
		/**
		 *	@fn			IsTouched
		 *	@brief		パッドボタンが押された瞬間
		 *	@param[in]	argStick	!<	スティックの指定
		 *	@param[in]	argKeyCode	!<	ボタンのキーコード
		 *	@retval		true		!<	押された
		 *	@retval		false		!<	押されていない
		 */
		bool IsTouched(eStick argStick, eKey argKeyCode);
		/**
		 *	@fn			RregisterButton
		 *	@brief		ボタンの登録
		 *	@param[in]	argKeyCode		!<	登録するキーコード
		 *	@param[in]	argBottonCode	!<	登録するパッドボタンの番号
		 */
		void RregisterKey(eKey argKeyCode, unsigned int argBottonCode);

#pragma region		Button
	public:
		/**
		 *	@fn			IsButtonPressed
		 *	@brief		指定のボタンを押したか
		 *	@param[in]	argKeyCode	!<	ボタンのキーコード
		 *	@retval		true		!<	押された
		 *	@retval		false		!<	押されていない
		 */
		bool IsButtonPressed(eKey argKeyCode);
		/**
		 *	@fn			IsButtonTouched
		 *	@brief		指定のボタンを押した瞬間
		 *	@param[in]	argKeyCode	!<	ボタンのキーコード
		 *	@retval		true		!<	押された
		 *	@retval		false		!<	押されていない
		 */
		bool IsButtonTouched(eKey argKeyCode);
#pragma endregion	Button
#pragma region		Stick
	private:
		/**
		 *	@fn			Axis
		 *	@brief		スティックの軸の入力値の取得
		 *	@param[in]	argStick	!<	スティックの指定
		 *	@param[in]	argInfo		!<	ジョイスティックの情報
		 *	@return		正規化された入力値
		 */
		float Axis(eStick argStick, JOYINFOEX argInfo);
		/**
		 *	@fn			Horizon
		 *	@brief		スティックの水平の入力値の取得
		 *	@param[in]	argStick	!<	スティックの指定
		 *	@param[in]	argInfo		!<	ジョイスティックの情報
		 *	@return		正規化された入力値
		 */
		float Horizon(eStick argStick, JOYINFOEX argInfo);
	public:
		/**
		 *	@fn			IsTilted
		 *	@brief		指定のスティックが指定の方向に入力されているか
		 *	@param[in]	argStick	!<	スティックの指定
		 *	@param[in]	argKeyCode	!<	スティックの入力方向
		 *	@retval		true		!<	入力された
		 *	@retval		false		!<	入力されていない
		 */
		bool IsTilted(eStick argStick, eKey argKeyCode);
		/**
		 *	@fn			IsReached
		 *	@brief		指定のスティックが指定の方向に入力された瞬間
		 *	@param[in]	argStick	!<	スティックの指定
		 *	@param[in]	argKeyCode	!<	スティックの入力方向
		 *	@retval		true		!<	入力された
		 *	@retval		false		!<	入力されていない
		 */
		bool IsReached(eStick argStick, eKey argKeyCode);
		
#pragma endregion	Stick
#pragma region		DebugMsg 
	private:
		/**
		 *	@fn			GetActiveJoystickIdString
		 *	@brief		パッドの接続を文字列で返す
		 *	@return		サポートしているパッドの接続状況の文字列
		 */
		std::string GetActiveJoystickIdString();
		/**
		 *	@fn			MakeTabSplitString
		 *	@brief		エラーチェック文字列スプリットする
		 *	@param[in]	argResults			!<	ジョイスティックのメモリ
		 *	@param[in]	argSupportJoystickNum	!<	サポートパッド数
		 *	@return		スプリットされた文字列
		 */
		std::string MakeTabSplitString(MMRESULT* argResults, unsigned int argSupportJoystickNum);
	public:
		/**
		 *	@fn			CheckConnectCountMsg
		 *	@brief		スティックの入力数の文字列
		 *	@return		接続パッド数とサポートパッド数の文字列
		 */
		std::string CheckConnectCountMsg();
		/**
		 *	@fn			ChackErrorMsg
		 *	@brief		パッドのエラーチェック
		 *	
		 *							0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
		 *	MMSYSERR_NODRIVER		  ××××××××××××××××××××××
		 *	MMSYSERR_INVALPARAM		  ××××××××××××××××××××××
		 *	MMSYSERR_BADDEVICEID	  ××××××××××××××××××××××
		 *	JOYERR_UNPLUGGED		  ××××××××××××××××××××××
		 *	@return		サポートパッドの数のエラーチェック文字列
		 */
		std::string ChackErrorMsg();
		/**
		 *	@fn			ChackInputValueMsg
		 *	@brief		パッドの入力値を文字列で返す
		 *	@return		パッドの入力値の文字列
		 */
		std::string ChackInputValueMsg();
#pragma endregion	DebugMsg 

	};


};