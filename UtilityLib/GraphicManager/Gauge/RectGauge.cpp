/**
 *	@file	RectGauge.cpp
 *	@date	2017 / 8 / 31
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "RectGauge.h"

Utility::RectGauge::RectGauge()
{
	property_.Value = 0.0f;
	property_.MinValue = 0.0f;
	property_.MaxValue = 1.0f;

	property_.Pos = 0.0f;
	property_.MinPos = 0;
	property_.MaxPos = 0;

	pos_ = Math::Vector2(200, 200);
}

Utility::RectGauge::~RectGauge() = default;

void Utility::RectGauge::Draw(ID2D1RenderTarget * argRt)
{
	Gauge::Draw(argRt);
}

void Utility::RectGauge::Draw()
{
	property_.UpdatePosition();
	
	ShadowEnable(true);
	MainColor(color_);
	FillRect(pos_.x, pos_.y, property_.Pos, wight_);

}
