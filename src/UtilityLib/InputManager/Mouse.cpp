/**
 *	@file	Mouse.cpp
 *	@date	2017 / 8 / 15
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "Mouse.h"

Utility::Mouse::Mouse()
	: resetCnt_(0)
{
	button_.insert(std::map<eMouseButton, unsigned char>::value_type(eMouseButton::Left, VK_LBUTTON));
	button_.insert(std::map<eMouseButton, unsigned char>::value_type(eMouseButton::Middle, VK_MBUTTON));
	button_.insert(std::map<eMouseButton, unsigned char>::value_type(eMouseButton::Right , VK_RBUTTON));
}

Utility::Mouse::~Mouse() = default;

bool Utility::Mouse::Update()
{
	oldPos_ = nowPos_;
	GetCursorPos(&nowPos_);

	++resetCnt_;
	for (int i = 0; i < static_cast<int>(eMouseButton::Max); i++)
	{
		oldButton_[i] = nowButton_[i];
		nowButton_[i] = false;
		if (doubleClickButton_[i])
		{
			if (resetCnt_ % 60 == 0)
			{
				doubleClickButton_[i] = false;
			}
		}
	}

	return true;
}

bool Utility::Mouse::IsPressed(eMouseButton argButton)
{
	const int Index = static_cast<int>(argButton);
	nowButton_[Index] = (GetKeyState(button_.at(argButton)) & 0x80) ? true : false;
	return 	nowButton_[Index];
}

bool Utility::Mouse::IsClicked(eMouseButton argButton)
{
	const int Index = static_cast<int>(argButton);
	nowButton_[Index] = (GetKeyState(button_.at(argButton)) & 0x80) ? true : false;
	return 	(!oldButton_[Index] && nowButton_[Index]);
}

bool Utility::Mouse::IsFreed(eMouseButton argButton)
{
	const int Index = static_cast<int>(argButton);
	nowButton_[Index] = (GetKeyState(button_.at(argButton)) & 0x80) ? true : false;
	return 	(oldButton_[Index] && !nowButton_[Index]);
}

bool Utility::Mouse::IsDoubleClicked(eMouseButton argButton)
{
	const int Index = static_cast<int>(argButton);
	nowButton_[Index] = (GetKeyState(button_.at(argButton)) & 0x80) ? true : false;

	if (!doubleClickButton_[Index])
	{
		doubleClickButton_[Index] = true;
		resetCnt_ = 0;
		return false;
	}
	
	bool result = (doubleClickButton_[Index] && nowButton_[Index]);
	doubleClickButton_[Index] = false;

	return 	result;
}
