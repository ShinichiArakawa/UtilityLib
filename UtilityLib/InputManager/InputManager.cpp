/**
 *	@file	InputManager.cpp
 *	@date	2017 / 8 / 13
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "InputManager.h"
#include "Keyboard.h"
#include "Mouse.h"

Utility::InputManager::InputManager()
	:keyboard_(std::make_unique<Keyboard>())
{
}

Utility::InputManager::~InputManager() = default;
#pragma region		Keyboard
/**
 *	@fn			RregisterKeyboardKey
 *	@brief		キーボードのキーの登録
 *	@param[in]	argkeycode	!<	登録するキー
 *	@param[in]	argByteCode	!<	キーボードのバイトコード
 */
void Utility::InputManager::RregisterKeyboardKey(eKey argkeycode, unsigned char argByteCode)
{
	keyboard_->RregisterKey(argkeycode, argByteCode);
}
/**
 *	@fn		Update
 *	@brief	更新
 *	@retval	true	!<	更新成功
 *	@retval	false	!<	更新失敗
 */
bool Utility::InputManager::Update()
{
	bool res = keyboard_->Update();
	if(mouse_)
		res = mouse_->Update();
	for (const auto & Pad : pads_)
	{
		Pad->Update();
	}
	return res;
}
/**
 *	@fn			IsPressed
 *	@brief		キーボードの指定のキーが入力されている間
 *	@param[in]	argKeyCode	!<	キーコード
 *	@retval		true		!<	入力あり
 *	@retval		false		!<	入力なし
 */
bool Utility::InputManager::IsPressed(eKey argKeyCode)
{
	return keyboard_->IsPressed(argKeyCode);
}
/**
 *	@fn			IsPressed
 *	@brief		キーボードの指定のキーが入力された瞬間
 *	@param[in]	argKeyCode	!<	キーコード
 *	@retval		true		!<	入力あり
 *	@retval		false		!<	入力なし
 */
bool Utility::InputManager::IsTouched(eKey argKeyCode)
{
	return keyboard_->IsTouched(argKeyCode);
}
#pragma	endregion	Keyboard

#pragma region		Pad
/**
 *	@fn			RregisterAllGamepadKey
 *	@brief		全てのパッドのキーの登録
 *	@param[in]	argkeycode	!<	登録するキー
 *	@param[in]	argByteCode	!<	キーボードのバイトコード
 */
void Utility::InputManager::RregisterAllGamepadKey(eKey argkeycode, unsigned int argButoonCode)
{
	for (const auto & Pad : pads_)
	{
		Pad->RregisterKey(argkeycode, argButoonCode);
	}
}
/**
 *	@fn			RregisterKeyboardKey
 *	@brief		特定のパッドのキーの登録
 *	@param[in]	argkeycode	!<	登録するキー
 *	@param[in]	argByteCode	!<	キーボードのバイトコード
 *	@param[in]	argPadIndex	!<	パッドの番号
 */
void Utility::InputManager::RregisterGamepadKey(eKey argkeycode, unsigned int argButoonCode, unsigned int argPadIndex)
{
	pads_[argPadIndex]->RregisterKey(argkeycode, argButoonCode);
}
/**
 *	@fn		AddPadInput
 *	@brief	パッドの追加
 */
void Utility::InputManager::AddPadInput()
{
	pads_.push_back(std::make_unique<GamePad>(padCnt_++));
}
/**
*	@fn		ConnectedPadNum
*	@brief	接続パッド数の取得
 *	@return	接続パッド数
 */
int Utility::InputManager::ConnectedPadNum() const
{
	int padNum = 0;
	for (const auto & Pad : pads_)
	{
		if (Pad->IsConnectCheck())
			padNum++;
	}

	return padNum;
}
/**
 *	@fn			IsPressed
 *	@brief		接続されているパッドの内、指定のキーが入力されている間（パッドが接続されていない場合キーボードから入力）
 *	@param[in]	argStick	!<	スティックの指定
 *	@param[in]	argKeyCode	!<	キーコード
 *	@retval		true		!<	入力あり
 *	@retval		false		!<	入力なし
 */
bool Utility::InputManager::IsPressed(eStick argStick, eKey argKeyCode)
{
	bool res = false;
	for (const auto &Pad : pads_)
	{
		if (Pad->IsConnectCheck())
		{
			res = Pad->IsPressed(argStick, argKeyCode);
			if (res)
				return res;
		}
	}

	res = keyboard_->IsPressed(argKeyCode);
	
	return res;
}
/**
 *	@fn			IsTouched
 *	@brief		接続されているパッドの内、指定のキーが入力された瞬間（パッドが接続されていない場合キーボードから入力）
 *	@param[in]	argStick	!<	スティックの指定
 *	@param[in]	argKeyCode	!<	キーコード
 *	@retval		true		!<	入力あり
 *	@retval		false		!<	入力なし
 */
bool Utility::InputManager::IsTouched(eStick argStick, eKey argKeyCode)
{
	bool res = false;
	for (const auto &Pad : pads_)
	{
		if (Pad->IsConnectCheck())
		{
			res = Pad->IsTouched(argStick, argKeyCode);
			if (res)
				return res;
		}
	}

	res = keyboard_->IsPressed(argKeyCode);

	return res;
}
/**
 *	@fn			IsPressed
 *	@brief		指定したパッドの指定のキーが入力されている間（パッドが接続されていない場合キーボードから入力）
 *	@param[in]	argStick	!<	スティックの指定
 *	@param[in]	argKeyCode	!<	キーコード
 *	@retval		true		!<	入力あり
 *	@retval		false		!<	入力なし
 */
bool Utility::InputManager::IsPressed(eStick argStick, eKey argKeyCode, int argPadIndex)
{
	bool res = (pads_[argPadIndex]->IsConnectCheck()) ?
		pads_[argPadIndex]->IsPressed(argStick, argKeyCode) :
		keyboard_->IsPressed(argKeyCode);
	return res;
}
/**
 *	@fn			IsTouched
 *	@brief		指定したパッドの指定のキーが入力された瞬間（パッドが接続されていない場合キーボードから入力）
 *	@param[in]	argStick	!<	スティックの指定
 *	@param[in]	argKeyCode	!<	キーコード
 *	@retval		true		!<	入力あり
 *	@retval		false		!<	入力なし
 */
bool Utility::InputManager::IsTouched(eStick argStick, eKey argKeyCode, int argPadIndex)
{
	bool res = (pads_[argPadIndex]->IsConnectCheck()) ?
		pads_[argPadIndex]->IsTouched(argStick, argKeyCode) :
		keyboard_->IsTouched(argKeyCode);
	return res;
}
#pragma	endregion	Pad

#pragma region		Mouse
void Utility::InputManager::AddMouseInput()
{
	mouse_ = std::make_unique<Mouse>();
}
/**
 *	@fn			IsPressed
 *	@brief		マウスのクリック入力されている間
 *	@param[in]	argButton	!<	ボタンの指定
 *	@retval		true		!<	入力あり
 *	@retval		false		!<	入力なし
 */
bool Utility::InputManager::IsPressed(eMouseButton argButton)
{
	return mouse_->IsPressed(argButton);
}
/**
 *	@fn			IsClicked
 *	@brief		マウスのクリック入力された瞬間
 *	@param[in]	argButton	!<	ボタンの指定
 *	@retval		true		!<	入力あり
 *	@retval		false		!<	入力なし
 */
bool Utility::InputManager::IsClicked(eMouseButton argButton)
{
	return mouse_->IsClicked(argButton);
}
/**
 *	@fn			IsFreed
 *	@brief		マウスのクリック入力が無くなった瞬間
 *	@param[in]	argButton	!<	ボタンの指定
 *	@retval		true		!<	入力あり
 *	@retval		false		!<	入力なし
 */
bool Utility::InputManager::IsFreed(eMouseButton argButton)
{
	return mouse_->IsFreed(argButton);
}
/**
 *	@fn			IsDoubleClicked
 *	@brief		マウスのダブルクリック入力の瞬間
 *	@param[in]	argButton	!<	ボタンの指定
 *	@retval		true		!<	入力あり
 *	@retval		false		!<	入力なし
 */
bool Utility::InputManager::IsDoubleClicked(eMouseButton argButton)
{
	return mouse_->IsDoubleClicked(argButton);
}
/**
 *	@fn			NowPoint
 *	@brief		マウスカーソルの現在の座標
 *	@return		現在のカーソルの座標
 */
POINT Utility::InputManager::NowPoint() const
{
	return mouse_->NowPoint();
}
/**
 *	@fn			OldPoint
 *	@brief		マウスカーソルの前フレームの座標
 *	@return		前フレームのカーソルの座標
 */
POINT Utility::InputManager::OldPoint() const
{
	return mouse_->OldPoint();
}
#pragma	endregion	Mouse
