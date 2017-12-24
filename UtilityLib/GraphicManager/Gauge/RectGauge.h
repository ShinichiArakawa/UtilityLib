/**
 *	@file	RectGauge.h
 *	@date	2017 / 8 / 31
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "Gauge.h"

namespace Utility
{
	class RectGauge : public Gauge
	{
	private:
		void Draw() override;
	public:
		RectGauge();
		~RectGauge();
	public:
		void Draw(ID2D1RenderTarget* argRt);

	};
};