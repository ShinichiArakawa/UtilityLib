/**
 *	@file	Gauge.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "Gauge.h"

Utility::Gauge::Property::operator float() const
{
	return Value;
}

float Utility::Gauge::Property::operator=(float argValue)
{
	//return this->Value = std::clamp(Value,MinValue,MaxValue); // C++17 <algorithm>
	return this->Value = max(MinValue, min(MaxValue, argValue));
}

void Utility::Gauge::Property::UpdatePosition()
{
	// 線形補完と同じ式
	//                             s                         * (v2-v1)                       * v1            
	Pos = (Value - MinValue) / (MaxValue - MinValue) * (MaxPos - MinPos) + MinPos;
}

Utility::Gauge::Gauge()
	:color_(D2D1::ColorF(0, 0, 0)), wight_(0.f)
{	
	property_.Value = 0;
	property_.MinValue = 0;
	property_.MaxValue = 100;

	property_.Pos = 0;
	property_.MinPos = 0;
	property_.MaxPos = 400;

	pos_ = Math::Vector2(50, 100);
}

Utility::Gauge::~Gauge() = default;

void Utility::Gauge::Draw(ID2D1RenderTarget * argRt)
{
	PaintBrush::Draw(argRt);
}

void Utility::Gauge::Draw()
{
	property_.UpdatePosition();

	GetRenderTarget()->SetTransform(D2D1::Matrix3x2F::Skew(30, 0));

	LineWidth(2);
	MainColor(D2D1::ColorF(D2D1::ColorF::LightBlue));
	FillRect(pos_.x, pos_.y, property_.MaxPos, 10);

	MainColor(D2D1::ColorF(D2D1::ColorF::Blue));
	FillRect(pos_.x, pos_.y, property_.Pos, 10);

	MainColor(D2D1::ColorF(D2D1::ColorF::White));
	DrawRect(pos_.x - 5, pos_.y, property_.MaxPos+10, 10);
}
