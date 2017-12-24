/**
 *	@file	CMenu.cpp
 *	@date	2017 / 7 / 30
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "CMenu.h"

class Utility::CMenu::Impl
{
public:
	Impl(CMenu *argMenu) 
		:menu_(argMenu)
	{
	}
	~Impl() = default;

public:
	CMenuButton* FreeButton(CMenuButton *argButton)
	{
		// 次のボタンを保持
		CMenuButton *next = argButton->next_;
		bool isLast = (argButton == menu_->last_);

		// ボタンを削除
		delete argButton;

		// 次のボタンかnullptrを返す
		return (isLast) ? nullptr : next;
	}
public:
	CMenu *menu_;
};

Utility::CMenu::CMenu(CMenuButton *argFirstSelectButton, CMenuButton *argLastSelectButton)
	:pImpl(new Impl(this)), currect_(argFirstSelectButton), first_(argFirstSelectButton), last_(argLastSelectButton)
{
	first_->next_ = last_;
	first_->prev_ = last_;
	last_->next_ = first_;
	last_->prev_ = first_;
}

Utility::CMenu::~CMenu()
{
	for (auto button = last_->next_; button; button = pImpl->FreeButton(button));
}

void Utility::CMenu::AddButton(CMenuButton *argButton)
{
	argButton->prev_ = first_;
	argButton->next_ = last_;
	first_->next_ = argButton;
	last_->prev_ = argButton;
	first_ = argButton;
}

void Utility::CMenu::Next()
{
	currect_ = currect_->next_;
}

void Utility::CMenu::Prev()
{
	currect_ = currect_->prev_;
}

bool Utility::CMenu::Enter()
{
	return currect_->Enter();
}

void Utility::CMenu::Draw()
{
	currect_->Draw();
}
