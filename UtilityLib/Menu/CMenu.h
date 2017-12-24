/**
 *	@file	CMenu.h
 *	@date	2017 / 7 / 30
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <memory>
#include "CMenuButton.h"

namespace Utility
{
	class CMenu
	{
	private:
		class Impl;
		std::unique_ptr<Impl> pImpl;

	protected:
		CMenuButton *currect_;
		CMenuButton *first_;
		CMenuButton *last_;

	public:

		/**
		 *  @constructor	CMenu
		 *  @brief			メニュー
		 *  @param[in]		argFirstSelectButton	!<	最初に置くボタン
		 *  @param[in]		argLastSelectButton		!<	最後に置くボタン
		 */
		CMenu(CMenuButton *argFirstSelectButton, CMenuButton *argLastSelectButton);
		virtual ~CMenu();
	public:

		/**
		 *  @fn			AddButton
		 *  @brief		ボタンの追加
		 *  @param[in]	argButton	!<	追加するボタン
		 */
		void AddButton(CMenuButton *argButton);
		/**
		 *  @fn		Next
		 *  @brief	次のボタンに進む
		 */
		void Next();
		/**
		 *  @fn		Prev
		 *  @brief	前のボタンに進む
		 */
		void Prev();
		/**
		 *  @fn		Enter
		 *  @brief	ボタンの選択
		 *  @retval	true	!<	選択中
		 *  @retval	false	!<	未選択
		 */
		bool Enter();
		/**
		 *  @fn		Draw
		 *  @brief	描画
		 */
		void Draw();

	};
};