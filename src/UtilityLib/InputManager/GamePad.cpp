/**
 *	@file	GamePad.cpp
 *	@date	2017 / 10 / 4
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "GamePad.h"
#include "../Debug/Debug.h"
#include "../Math/Math.h"
#include "InputManager.h"

#include <sstream>

#pragma comment(lib, "winmm.lib")

namespace
{
	static float StickInputValueMin = 0.f;
	static unsigned long StickInputValueDefault = 32511;
	static float StickInputValueMax = 65535.f;

	float Normalize(DWORD argValue)
	{
		return Utility::Math::Normalize(StickInputValueMin, StickInputValueMax, static_cast<float>(argValue));
	}
}

Utility::GamePad::GamePad()
	:showJoystickId_(JOYSTICKID1), isConnected_(false)
{
	Initialize();
}

Utility::GamePad::GamePad(unsigned int argPadNumber)
	:showJoystickId_(argPadNumber), isConnected_(false)
{
	Initialize();
}

Utility::GamePad::~GamePad() = default;
/**
 *	@fn		Reset
 *	@brief	初期化関数
 */
void Utility::GamePad::Initialize()
{
	joyInfoNow_.dwSize = sizeof(JOYINFOEX);
	joyInfoNow_.dwFlags = JOY_RETURNALL;

	const int JoystickCount = joyGetNumDevs();
	if (JoystickCount == 0)
		Debug::OutputDebugStringFormat(L"ジョイスティックが接続されていません\n");

	if (JOYERR_NOERROR == joyGetPosEx(showJoystickId_, &joyInfoNow_))
	{
		isConnected_ = true;
		Debug::OutputDebugStringFormat(L"ジョイスティック No.%d　接続されています\n", showJoystickId_);
	}

}
/**
 *	@fn		Update
 *	@brief	更新
 */
void Utility::GamePad::Update()
{
	if (!isConnected_)
		return;

	joyInfoOld_ = joyInfoNow_;
	joyInfoNow_.dwSize = sizeof(JOYINFOEX);
	joyInfoNow_.dwFlags = JOY_RETURNALL;
	MMRESULT res = joyGetPosEx(showJoystickId_, &joyInfoNow_);

	if (JOYERR_NOERROR != res) 
	{
		isConnected_ = false;
		memset(&joyInfoNow_, 0, sizeof(joyInfoNow_));
	}
}
/**
 *	@fn			IsConnectCheck
 *	@brief		接続確認
 *	@retval		true	!<	接続あり
 *	@retval		false	!<	接続なし
 */
bool Utility::GamePad::IsConnectCheck()
{
	return isConnected_;
}
/**
 *	@fn			IsPressed
 *	@brief		パッドボタンが押されているか
 *	@param[in]	argStick	!<	スティックの指定
 *	@param[in]	argKeyCode	!<	ボタンのキーコード
 *	@retval		true		!<	押された
 *	@retval		false		!<	押されていない
 */
bool Utility::GamePad::IsPressed(eStick argStick, eKey argKeyCode)
{
	bool res = false;
	
	if (argKeyCode == GenericKeys::Up ||
		argKeyCode == GenericKeys::Down ||
		argKeyCode == GenericKeys::Left ||
		argKeyCode == GenericKeys::Right)
	{
		res = IsTilted(argStick, argKeyCode);
	}
	else
	{
		res = IsButtonPressed(argKeyCode);
	}
	return res;
}
/**
 *	@fn			IsTouched
 *	@brief		パッドボタンが押された瞬間
 *	@param[in]	argStick	!<	スティックの指定
 *	@param[in]	argKeyCode	!<	ボタンのキーコード
 *	@retval		true		!<	押された
 *	@retval		false		!<	押されていない
 */
bool Utility::GamePad::IsTouched(eStick argStick, eKey argKeyCode)
{
	bool res = false;

	if (argKeyCode == GenericKeys::Up ||
		argKeyCode == GenericKeys::Down ||
		argKeyCode == GenericKeys::Left ||
		argKeyCode == GenericKeys::Right)
	{
		res = IsReached(argStick, argKeyCode);
	}
	else
	{
		res = IsButtonTouched(argKeyCode);
	}

	return res;
}
/**
 *	@fn			RregisterButton
 *	@brief		ボタンの登録
 *	@param[in]	argkeycode		!<	登録するキーコード
 *	@param[in]	argBottonCode	!<	登録するパッドボタンの番号
 */
void Utility::GamePad::RregisterKey(eKey argKeyCode, unsigned int argBottonCode)
{
	pad_.insert(std::map<eKey, unsigned int>::value_type(argKeyCode, argBottonCode));
}
#pragma region		Button

/**
 *	@fn			IsButtonPressed
 *	@brief		指定のボタンを押したか
 *	@param[in]	argKeyCode	!<	ボタンのキーコード
 *	@retval		true		!<	押された
 *	@retval		false		!<	押されていない
 */
bool Utility::GamePad::IsButtonPressed(eKey argKeyCode)
{
	return (Math::IsChackBits(static_cast<uint16_t>(joyInfoNow_.dwButtons), static_cast<uint16_t>(pad_.at(argKeyCode))));
}
/**
 *	@fn			IsButtonTouched
 *	@brief		指定のボタンを押した瞬間
 *	@param[in]	argKeyCode	!<	ボタンのキーコード
 *	@retval		true		!<	押された
 *	@retval		false		!<	押されていない
 */
bool Utility::GamePad::IsButtonTouched(eKey argKeyCode)
{
	bool now = (Math::IsChackBits(static_cast<uint16_t>(joyInfoNow_.dwButtons), static_cast<uint16_t>(pad_.at(argKeyCode))));
	bool old = (Math::IsChackBits(static_cast<uint16_t>(joyInfoOld_.dwButtons), static_cast<uint16_t>(pad_.at(argKeyCode))));

	return (now && !old);
}

#pragma endregion	Button
#pragma region		Stick
/**
 *	@fn			Axis
 *	@brief		スティックの軸の入力値の取得
 *	@param[in]	argStick	!<	スティックの指定
 *	@param[in]	argInfo		!<	ジョイスティックの情報
 *	@return		正規化された入力値
 */
float Utility::GamePad::Axis(eStick argStick, JOYINFOEX argInfo)
{
	float res = 0.f;
	if (argStick == eStick::Right)
	{
		res = Normalize(argInfo.dwRpos);
	}
	else if (argStick == eStick::Left)
	{
		res = Normalize(argInfo.dwXpos);
	}

	return res;
}
/**
 *	@fn			Horizon
 *	@brief		スティックの水平の入力値の取得
 *	@param[in]	argStick	!<	スティックの指定
 *	@param[in]	argInfo		!<	ジョイスティックの情報
 *	@return		正規化された入力値
 */
float Utility::GamePad::Horizon(eStick argStick, JOYINFOEX argInfo)
{
	float res = 0.f;
	if (argStick == eStick::Right)
	{
		res = Normalize(argInfo.dwZpos);
	}
	else if (argStick == eStick::Left)
	{
		res = Normalize(argInfo.dwYpos);
	}

	return res;

}
/**
 *	@fn			IsTilted
 *	@brief		指定のスティックが指定の方向に入力されているか
 *	@param[in]	argStick	!<	スティックの指定
 *	@param[in]	argKeyCode	!<	スティックの入力方向
 *	@retval		true		!<	入力された
 *	@retval		false		!<	入力されていない
 */
bool Utility::GamePad::IsTilted(eStick argStick, eKey argKeyCode)
{
	bool res = false;
	if (argKeyCode == GenericKeys::Right)
	{
		res = (Axis(argStick, joyInfoNow_) > Normalize(StickInputValueDefault));
	}
	else if (argKeyCode == GenericKeys::Left)
	{
		res = (Axis(argStick, joyInfoNow_) < Normalize(StickInputValueDefault));
	}
	else if (argKeyCode == GenericKeys::Up)
	{
		res = (Horizon(argStick, joyInfoNow_) < Normalize(StickInputValueDefault));
	}
	else if (argKeyCode == GenericKeys::Down)
	{
		res = (Horizon(argStick, joyInfoNow_) > Normalize(StickInputValueDefault));
	}
	return res;
}
/**
 *	@fn			IsReached
 *	@brief		指定のスティックが指定の方向に入力された瞬間
 *	@param[in]	argStick	!<	スティックの指定
 *	@param[in]	argKeyCode	!<	スティックの入力方向
 *	@retval		true		!<	入力された
 *	@retval		false		!<	入力されていない
 */
bool Utility::GamePad::IsReached(eStick argStick, eKey argKeyCode)
{
	bool now = false;
	bool old = false;
	if (argKeyCode == GenericKeys::Right)
	{
		now = (Axis(argStick, joyInfoNow_) > Normalize(StickInputValueDefault));
		old = (Axis(argStick, joyInfoOld_) > Normalize(StickInputValueDefault));
	}
	else if (argKeyCode == GenericKeys::Left)
	{
		now = (Axis(argStick, joyInfoNow_) < Normalize(StickInputValueDefault));
		old = (Axis(argStick, joyInfoOld_) < Normalize(StickInputValueDefault));
	}
	else if (argKeyCode == GenericKeys::Up)
	{
		now = (Horizon(argStick, joyInfoNow_) < Normalize(StickInputValueDefault));
		old = (Horizon(argStick, joyInfoOld_) < Normalize(StickInputValueDefault));
	}
	else if (argKeyCode == GenericKeys::Down)
	{
		now = (Horizon(argStick, joyInfoNow_) > Normalize(StickInputValueDefault));
		old = (Horizon(argStick, joyInfoOld_) > Normalize(StickInputValueDefault));
	}
	return (now && !old);
}
#pragma endregion	Stick
#pragma region		DebugMsg 
/**
 *	@fn			CheckConnectCountMsg
 *	@brief		スティックの入力数の文字列
 *	@return		接続パッド数とサポートパッド数の文字列
 */
std::string Utility::GamePad::CheckConnectCountMsg()
{
	std::ostringstream stream;

	stream << ("Joystickサポート数:") << joyGetNumDevs() << std::endl <<
		("接続JoystickID:") << GetActiveJoystickIdString() << std::endl;

	return stream.str();
}
/**
 *	@fn			ChackErrorMsg
 *	@brief		パッドのエラーチェック
 *	@return		サポートパッドの数のエラーチェック文字列
 */
std::string Utility::GamePad::ChackErrorMsg()
{
	const unsigned int SupportJoystickNum = joyGetNumDevs();

	MMRESULT* mmResults = reinterpret_cast<MMRESULT*>(malloc(sizeof(MMRESULT) * SupportJoystickNum));
	memset(mmResults, 0, sizeof(MMRESULT) * SupportJoystickNum);

	JOYINFOEX joyInfoEx;
	joyInfoEx.dwSize = sizeof(JOYINFOEX);
	joyInfoEx.dwFlags = JOY_RETURNALL;
	for (unsigned int lJoyStickIndex = JOYSTICKID1; lJoyStickIndex < SupportJoystickNum; lJoyStickIndex++)
	{
		mmResults[lJoyStickIndex] = joyGetPosEx(lJoyStickIndex, &joyInfoEx);
	}

	const std::string Str = MakeTabSplitString(mmResults, SupportJoystickNum);

	free(mmResults);
	mmResults = nullptr;

	return Str;
}
/**
 *	@fn			MakeTabSplitString
 *	@brief		エラーチェック文字列スプリットする
 *	@param[in]	argResults			!<	ジョイスティックのメモリ
 *	@param[in]	argSupportJoystickNum	!<	サポートパッド数
 *	@return		スプリットされた文字列
 */
std::string Utility::GamePad::MakeTabSplitString(MMRESULT* argResults, unsigned int argSupportJoystickNum)
{
	std::ostringstream stream;

	// ヘッダ
	stream << ("エラー種別") << ("\t");
	for (unsigned int lJoyStickIndex = 0; lJoyStickIndex < argSupportJoystickNum; lJoyStickIndex++)
	{
		stream << lJoyStickIndex;
		if (lJoyStickIndex < argSupportJoystickNum - 1) 
		{
			stream << ("\t"); 
		}
	}
	stream << std::endl;

	// ジョイスティックドライバが存在しません。
	stream << ("MMSYSERR_NODRIVER") << ("\t");
	for (unsigned int lJoyStickIndex = 0; lJoyStickIndex < argSupportJoystickNum; lJoyStickIndex++) 
	{
		if (argResults[lJoyStickIndex] & MMSYSERR_NODRIVER) 
		{
			stream << ("×");
		}
		else 
		{
			stream << ("  ");
		}
		if (lJoyStickIndex < argSupportJoystickNum - 1) 
		{
			stream << ("\t"); 
		}
	}
	stream << std::endl;

	// 無効なパラメータが渡されました。
	stream << ("MMSYSERR_INVALPARAM") << ("\t");
	for (unsigned int lJoyStickIndex = 0; lJoyStickIndex < argSupportJoystickNum; lJoyStickIndex++) 
	{
		if (argResults[lJoyStickIndex] & MMSYSERR_INVALPARAM) 
		{
			stream << ("×");
		}
		else 
		{
			stream << ("  ");
		}
		if (lJoyStickIndex < argSupportJoystickNum - 1) 
		{
			stream << ("\t"); 
		}
	}
	stream << std::endl;

	// 指定されたジョイスティック識別子は無効です。
	stream << ("MMSYSERR_BADDEVICEID") << ("\t");
	for (unsigned int lJoyStickIndex = 0; lJoyStickIndex < argSupportJoystickNum; lJoyStickIndex++) 
	{
		if (argResults[lJoyStickIndex] & MMSYSERR_BADDEVICEID) 
		{
			stream << ("×");
		}
		else 
		{
			stream << ("  ");
		}
		if (lJoyStickIndex < argSupportJoystickNum - 1) 
		{
			stream << ("\t"); 
		}
	}
	stream << std::endl;

	// 指定されたジョイスティックはシステムに接続されていません。
	stream << ("JOYERR_UNPLUGGED") << ("\t");
	for (unsigned int lJoyStickIndex = 0; lJoyStickIndex < argSupportJoystickNum; lJoyStickIndex++) 
	{
		if (argResults[lJoyStickIndex] & JOYERR_UNPLUGGED) 
		{
			stream << ("×");
		}
		else 
		{
			stream << ("  ");
		}
		if (lJoyStickIndex < argSupportJoystickNum - 1)
		{
			stream << ("\t"); 
		}
	}
	//stream << endl;

	return stream.str();
}
/**
 *	@fn			GetActiveJoystickIdString
 *	@brief		パッドの接続を文字列で返す
 *	@return		サポートしているパッドの接続状況の文字列
 */
std::string Utility::GamePad::GetActiveJoystickIdString()
{
	std::stringstream stream;

	JOYINFOEX joyInfoEx;
	joyInfoEx.dwSize = sizeof(JOYINFOEX);
	joyInfoEx.dwFlags = JOY_RETURNALL;
	for (unsigned int lJoyStickIndex = 0, lJoyCount = joyGetNumDevs(); lJoyStickIndex < lJoyCount; lJoyStickIndex++)
	{
		if (JOYERR_NOERROR == joyGetPosEx(lJoyStickIndex, &joyInfoEx)) 
		{
			stream << lJoyStickIndex << (",");
		}
	}

	std::string str = stream.str();
	if (0 < str.length())
	{
		str.erase(--str.end());
	}
	return str;
}
/**
 *	@fn			ChackInputValueMsg
 *	@brief		パッドの入力値を文字列で返す
 *	@return		パッドの入力値の文字列
 */
std::string Utility::GamePad::ChackInputValueMsg()
{
	std::ostringstream stream;
	stream << ("----- JoyInfoEx -----") << std::endl <<
		("dwSize:") << joyInfoNow_.dwSize << std::endl <<
		("dwFlags:") << joyInfoNow_.dwFlags << std::endl <<
		("dwXpos:") << joyInfoNow_.dwXpos << std::endl <<
		("dwYpos:") << joyInfoNow_.dwYpos << std::endl <<
		("dwZpos:") << joyInfoNow_.dwZpos << std::endl <<
		("dwRpos:") << joyInfoNow_.dwRpos << std::endl <<
		("dwUpos:") << joyInfoNow_.dwUpos << std::endl <<
		("dwVpos:") << joyInfoNow_.dwVpos << std::endl <<
		("dwButtons:") << joyInfoNow_.dwButtons << std::endl <<
		("dwButtonNumber:") << joyInfoNow_.dwButtonNumber << std::endl <<
		("dwPOV:") << joyInfoNow_.dwPOV << std::endl <<
		("dwReserved1:") << joyInfoNow_.dwReserved1 << std::endl <<
		("dwReserved2:") << joyInfoNow_.dwReserved2 << std::endl;
	return stream.str();
}
#pragma endregion	DebugMsg 
