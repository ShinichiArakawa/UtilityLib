/**
 *	@file	IButton.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

namespace Utility
{
	/**
	 *  @class	IButton
	 *  @brief	ボタンのインターフェイス
	 */
	class IButton
	{
	public:
		virtual ~IButton() = default;

	public:
		/**
		 *  @fn		Execute
		 *  @brief	ボタンの処理を実行
		 *	@retval	true	!<	成功
		 *	@retval	false	!<	失敗
		 */
		virtual bool Execute() = 0;
		/**
		 *  @fn		Enter
		 *  @brief	ボタンの選択
		 *	@retval	true	!<	選択中
		 *	@retval	false	!<	未選択
		 */
		virtual bool Enter() = 0;
		/**
		 *  @fn		Draw
		 *  @brief	ボタンの描画
		 */
		virtual void Draw() = 0;

	};
};