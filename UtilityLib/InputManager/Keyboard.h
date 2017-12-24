/**
*	@file	Keyboard.h
*	@date	2017 / 8 / 25
*	@author	Katsumi Takei
*	Copyright (c) Kastumi Takei. All rights reserved.
*/
#pragma once

#include <map>

namespace Utility
{
	enum class eKey;

	class Keyboard
	{
	private:
		/**
		 *  @brief	キーの配列
		 */
		struct
		{
			unsigned char key[256];
			operator unsigned char*() { return key; }
		}now_, old_;

		std::map<eKey, unsigned char> keyboard_;	//	!<	キーボードのキー操作の登録

	public:
		Keyboard();
		~Keyboard();

		/**
		 *  @fn			RregisterKey
		 *  @brief		キーの登録
		 *  @param[in]	argKeyCode	!<	登録するキー
		 *  @param[in]	argBytecode	!<	登録するバイトコード
		 */
		void RregisterKey(eKey argKeyCode, unsigned char argBytecode);
		/**
		 *  @fn			Update
		 *  @brief		更新
		 *  @retval		true	!<	成功
		 *  @retval		false	!<	失敗
		 */
		bool Update();
		/**
		 *  @fn			IsPressed
		 *  @brief		キーが押されているか
		 *  @param[in]	argKeyCode	!<	登録されたキーコード
		 *  @retval		true		!<	押されている
		 *  @retval		false		!<	押されていない
		 */
		bool IsPressed(eKey argKeyCode);
		/**
		 *  @fn			IsTouched
		 *  @brief		キーが押されている瞬間
		 *  @param[in]	argKeyCode	!<	登録されたキーコード
		 *  @retval		true		!<	押されている瞬間
		 *  @retval		false		!<	押されていない
		 */
		bool IsTouched(eKey argKeyCode);
	};
};