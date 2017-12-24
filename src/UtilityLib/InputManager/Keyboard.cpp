/**
 *	@file	Keyboard.cpp
 *	@date	2017 / 8 / 15
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "Keyboard.h"
#include <Windows.h>

Utility::Keyboard::Keyboard()
{
	ZeroMemory(&now_, sizeof(now_));
	ZeroMemory(&old_, sizeof(old_));
}

Utility::Keyboard::~Keyboard() = default;

void Utility::Keyboard::RregisterKey(eKey argkeycode, unsigned char argBytecode)
{
	keyboard_.insert(std::map<eKey, unsigned char>::value_type(argkeycode, argBytecode));
}

bool Utility::Keyboard::Update()
{
	old_ = now_;
	GetKeyboardState(now_);

	return true;
}

bool Utility::Keyboard::IsPressed(eKey argKeyCode)
{
	return (now_[keyboard_.at(argKeyCode)] & 0x80) ? true : false;
}

bool Utility::Keyboard::IsTouched(eKey argKeyCode)
{
	bool oldkey = (old_[keyboard_.at(argKeyCode)] & 0x80) ? true : false;
	bool nowkey = (now_[keyboard_.at(argKeyCode)] & 0x80) ? true : false;

	return (!oldkey && nowkey);
}