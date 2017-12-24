/**
 *	@file	Collision2D.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

namespace Utility
{
	class CShape;

	/**
	 *  @fn			CircleToCircle
	 *  @brief		円vs円の当たり判定
	 *  @param[in]	s1		!<	円の当たり判定	
	 *  @param[in]	s2		!<	円の当たり判定
	 *  @retval		true	!<	当たっている
	 *  @retval		false	!<	当たったいない
	 */
	bool CircleToCircle(const CShape& s1, const CShape& s2);
	/**
	 *  @fn			CircleToRect
	 *  @brief		円vs四角の当たり判定
	 *  @param[in]	s1		!<	円の当たり判定	
	 *  @param[in]	s2		!<	四角の当たり判定
	 *  @retval		true	!<	当たっている
	 *  @retval		false	!<	当たったいない
	 */
	bool CircleToRect(const CShape& s1, const CShape& s2);
	/**
	 *  @fn			RectToRect
	 *  @brief		四角vs四角の当たり判定
	 *  @param[in]	s1		!<	四角の当たり判定	
	 *  @param[in]	s2		!<	四角の当たり判定
	 *  @retval		true	!<	当たっている
	 *  @retval		false	!<	当たったいない
	 */
	bool RectToRect(const CShape& s1, const CShape& s2);
	/**
	 *  @fn			IsHitJudgment
	 *  @brief		当たり判定の判定
	 *  @param[in]	s1		!<	当たり判定	
	 *  @param[in]	s2		!<	当たり判定
	 *  @retval		true	!<	当たっている
	 *  @retval		false	!<	当たったいない
	 */
	bool IsHitJudgment(const CShape& s1, const CShape& s2);
};