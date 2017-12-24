/**
 *	@file	CircleGauge.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "Gauge.h"

namespace Utility
{
	enum class eGaugeType
	{
		GaugeFill,
		GaugeLine,
	};

	class CircleGauge : public Gauge
	{
	private:
		void Draw() override;
	private:
		eGaugeType type_;
	public:
		CircleGauge();
		~CircleGauge();

	public:
		void Draw(ID2D1RenderTarget* argRt);

	public:
		inline void GaugeType(eGaugeType type) { type_ = type; }
	};
};