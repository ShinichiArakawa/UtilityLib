/**
 *	@file	CircleGauge.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "CircleGauge.h"
#include "../../Math/Math.h"

Utility::CircleGauge::CircleGauge()
	: type_(eGaugeType::GaugeLine)
{
	property_.Value = 0.0f;
	property_.MinValue = 0.0f;
	property_.MaxValue = 1.0f;

	property_.Pos = 0.0f;
	property_.MinPos = Math::ToRadian(0);
	property_.MaxPos = Math::ToRadian(360);

	pos_ =  Math::Vector2(200, 200);
	radius_ = 25.f;
	
}

Utility::CircleGauge::~CircleGauge() = default;

void Utility::CircleGauge::Draw(ID2D1RenderTarget * argRt)
{
	Gauge::Draw(argRt);
}

void Utility::CircleGauge::Draw()
{
	property_.UpdatePosition();
	ArcStartAngle(Math::ToRadian(-90));

	MainColor(color_);
	if (type_ == eGaugeType::GaugeLine)
	{
		DrawArc(pos_.x, pos_.y, radius_, property_.MinPos, property_.Pos);
	}
	else
	{
		FillArc(pos_.x, pos_.y, radius_, property_.MinPos, property_.Pos);
	}
}
