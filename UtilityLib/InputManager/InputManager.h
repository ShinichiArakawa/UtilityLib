/**
 *	@file	InputManager.h
 *	@date	2017 / 8 / 31
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <memory>
#include <vector>
#include "Keyboard.h"
#include "Mouse.h"
#include "GamePad.h"

namespace Utility
{
	/**
	 *	@enum	eKey
	 *	@brief	入力キーコード
	 */
	enum class eKey
	{
	};
	/**
	 *	@struct	GenericKeys
	 *	@brief	キーの登録用構造体、継承して使う
	 */
	struct GenericKeys
	{
	private:
		enum class eKeySource
		{
			Up,
			Down,
			Right,
			Left,

			Debug,
			Escape,
			Decide,
			Cansel,
			Pause,
			Max,
		};
	protected:
		static const int KeyCount = static_cast<int>(eKeySource::Max);

	public:
		
		static const eKey Up = static_cast<eKey>(eKeySource::Up);
		static const eKey Down = static_cast<eKey>(eKeySource::Down);
		static const eKey Right = static_cast<eKey>(eKeySource::Right);
		static const eKey Left = static_cast<eKey>(eKeySource::Left);

		static const eKey Escape = static_cast<eKey>(eKeySource::Escape);
		static const eKey Pause = static_cast<eKey>(eKeySource::Pause);
		static const eKey Decide = static_cast<eKey>(eKeySource::Decide);
		static const eKey Cansel = static_cast<eKey>(eKeySource::Cansel);
		static const eKey Debug = static_cast<eKey>(eKeySource::Debug);
	};

	class InputManager final
	{
	private:
		std::unique_ptr<Keyboard> keyboard_;
		std::unique_ptr<Mouse> mouse_;
		std::vector<std::unique_ptr<GamePad>> pads_;
		int padCnt_ = 0;

	public:
		InputManager();
		~InputManager();

	public:
		bool Update();
#pragma region		Keyboard
	public:
		bool IsPressed(eKey argKeyCode);
		bool IsTouched(eKey argKeyCode);
		void RregisterKeyboardKey(eKey argkeycode, unsigned char argByteCode);
#pragma	endregion	Keyboard
#pragma region		Pad
	public:
		void RregisterAllGamepadKey(eKey argkeycode, unsigned int argButoonCode);
		void RregisterGamepadKey(eKey argkeycode, unsigned int argButoonCode, unsigned int argPadIndex);
		void AddPadInput();
		int ConnectedPadNum()const;
		bool IsPressed(eStick argStick, eKey argKeyCode);
		bool IsTouched(eStick argStick, eKey argKeyCode);
		bool IsPressed(eStick argStick, eKey argKeyCode, int argPadIndex);
		bool IsTouched(eStick argStick, eKey argKeyCode, int argPadIndex);
#pragma	endregion	Pad
#pragma region		Mouse
	public:
		void AddMouseInput();
		bool IsPressed(eMouseButton argButton);
		bool IsClicked(eMouseButton argButton);
		bool IsFreed(eMouseButton argButton);
		bool IsDoubleClicked(eMouseButton argButton);
		POINT NowPoint() const;
		POINT OldPoint() const;
#pragma	endregion	Mouse
	};
};