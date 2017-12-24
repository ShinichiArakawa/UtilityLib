/**
 *	@file	Mouse.h
 *	@date	2017 / 8 / 15
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <map>
#include <windows.h>

namespace Utility
{
	/**
	 *	@enum	eMouseButton
	 *	@brief	マウスの入力
	 */
	enum class eMouseButton
	{
		Left,
		Middle,
		Right,
		Max
	};

	class Mouse
	{
	private:
		int resetCnt_;
		POINT nowPos_, oldPos_;
		std::map<eMouseButton, unsigned char> button_;
		bool nowButton_[static_cast<int>(eMouseButton::Max)], oldButton_[static_cast<int>(eMouseButton::Max)],
			doubleClickButton_[static_cast<int>(eMouseButton::Max)];
	public:
		Mouse();
		~Mouse();

		/**
		 *  @fn		Update
		 *  @brief	更新
		 *  @retval	true	!<	成功
		 *  @retval	false	!<	失敗
		 */
		bool Update();
		/**
		 *	@fn			IsPressed
		 *	@brief		マウスボタンの押しっぱなし判定
		 *	@param[in]	argButton	!<	どのボタンを押しているか
		 *	@retval		true		!<	引数のボタンを押している
		 *	@retval		false		!<	引数のボタンを押していない
		 */
		bool IsPressed(eMouseButton argButton);
		/**
		 *	@fn			IsClicked
		 *	@brief		マウスボタンの押した瞬間の判定
		 *	@param[in]	argButton	!<	どのボタンを押しているか
		 *	@retval		true		!<	引数のボタンを押した瞬間である
		 *	@retval		false		!<	引数のボタンを押した瞬間でない
		 */
		bool IsClicked(eMouseButton argButton);
		/**
		 *	@fn			IsFreed
		 *	@brief		マウスボタンの離した瞬間の判定
		 *	@param[in]	argButton	!<	どのボタンを離したか
		 *	@retval		true		!<	引数のボタンを離した瞬間である
		 *	@retval		false		!<	引数のボタンを離した瞬間でない
		 */
		bool IsFreed(eMouseButton argButton);
		/**
		 *	@fn			IsDoubleClicked
		 *	@brief		マウスボタンのダブルクリックの判定
		 *	@param[in]	argButton	!<	どのボタンをダブルクリックしたか
		 *	@retval		true		!<	引数のボタンをダブルクリックした
		 *	@retval		false		!<	引数のボタンをダブルクリックしてない
		 */
		bool IsDoubleClicked(eMouseButton argButton);
		POINT NowPoint() const { return nowPos_; }
		POINT OldPoint() const { return oldPos_; }
		void NowPoint(POINT argPoint) { nowPos_ = argPoint; }
	};
};