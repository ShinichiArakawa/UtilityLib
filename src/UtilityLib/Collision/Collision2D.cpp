/**
 *	@file	Collision2D.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "Collision2D.h"
#include "Shape2D.h"

bool Utility::CircleToCircle(const CShape& s1, const CShape& s2)
{
	bool result = false;

	if (s1.Type() != eShapeType::Circle || s2.Type() != eShapeType::Circle)
		return result;

	float x1 = s1.Pos().x + s1.Radius();
	float x2 = s2.Pos().x + s2.Radius();
	float y1 = s1.Pos().y + s1.Radius();
	float y2 = s2.Pos().y + s2.Radius();
	float r1 = s1.Radius();
	float r2 = s2.Radius();

	if ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)
		<= (r1 + r2) * (r1 + r2))
	{
		result = true;
	}
	return result;
};

bool Utility::CircleToRect(const CShape& s1, const CShape& s2)
{
	bool result = false;
	
	if (s1.Type() != eShapeType::Circle || s2.Type() != eShapeType::Rect)
		return result;

	// 点の中心点と矩形の当たり判定
	bool a = (s1.Pos().x > s2.Rect().left) && (s1.Pos().x < s2.Rect().left) &&
		(s1.Pos().y < s2.Rect().top) && (s1.Pos().y > s2.Rect().bottom);
	bool b = (s1.Pos().x > s2.Rect().left) && (s1.Pos().x < s2.Rect().right) &&
		(s1.Pos().y < s2.Rect().top) && (s1.Pos().y > s2.Rect().bottom);

	// 矩形の角と円の当たり判定
	bool c = (s2.Rect().left - s1.Pos().x) * (s2.Rect().left - s1.Pos().x) +
		(s2.Rect().top - s1.Pos().y) * (s2.Rect().top - s1.Pos().y) < s1.Radius() * s1.Radius();
	bool d = (s2.Rect().right - s1.Pos().x) * (s2.Rect().right - s1.Pos().x) +
		(s2.Rect().top - s1.Pos().y) * (s2.Rect().top - s1.Pos().y) < s1.Radius() * s1.Radius();
	bool e = (s2.Rect().right - s1.Pos().x) * (s2.Rect().right - s1.Pos().x) +
		(s2.Rect().bottom - s1.Pos().y) * (s2.Rect().bottom - s1.Pos().y) < s1.Radius() * s1.Radius();
	bool f = (s2.Rect().left - s1.Pos().x) * (s2.Rect().left - s1.Pos().x) +
		(s2.Rect().bottom - s1.Pos().y) * (s2.Rect().bottom - s1.Pos().y) < s1.Radius() * s1.Radius();

	result = a || b || c || d || e || f;

	return result;
};

bool Utility::RectToRect(const CShape& s1, const CShape& s2)
{
	bool result = false;

	if (s1.Type() != eShapeType::Rect || s2.Type() != eShapeType::Rect)
		return result;

	auto ax1 = s1.Rect().left;
	auto ax2 = s1.Rect().right;
	auto ay1 = s1.Rect().top;
	auto ay2 = s1.Rect().bottom;
	auto bx1 = s2.Rect().left;
	auto bx2 = s2.Rect().right;
	auto by1 = s2.Rect().top;
	auto by2 = s2.Rect().bottom;

	if ((ax1 <= bx2) && (bx1 <= ax2) && (ay1 >= by2) && (by1 >= ay2))
	{
		result = true;
	}
	return result;
};

bool Utility::IsHitJudgment(const CShape& s1, const CShape& s2)
{
	if (CircleToCircle(s1, s2))
	{
		return true;
	}
	if (CircleToRect(s1, s2))
	{
		return true;
	}
	if (RectToRect(s1, s2))
	{
		return true;
	}
	return false;
}