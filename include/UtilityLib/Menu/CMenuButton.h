/**
 *	@file	CMenuButton.h
 *	@date	2017 / 7 / 30
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "IButton.h"

namespace Utility
{
	/**
	 *  @class	CMenuButton
	 *  @brief	メニューボタンの抽象クラス
	 */
	class CMenuButton : public IButton
	{
	protected:
		/**
		 *  @fn		Execute
		 *  @brief	ボタンの処理を実行
		 *	@retval	true	!<	成功
		 *	@retval	false	!<	失敗
		 */
		virtual bool Execute() override = 0;

	public:
		CMenuButton *next_;
		CMenuButton *prev_;

	public:
		CMenuButton() = default;
		virtual ~CMenuButton() = default;

	public:
		/**
		 *  @fn		Enter
		 *  @brief	ボタンの選択
		 *	@retval	true	!<	選択中
		 *	@retval	false	!<	未選択
		 */
		virtual bool Enter() override = 0;
		/**
		 *  @fn		Draw
		 *  @brief	ボタンの描画
		 */
		virtual void Draw() override = 0;
	};
};