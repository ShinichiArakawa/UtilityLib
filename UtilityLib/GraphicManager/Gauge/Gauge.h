/**
 *	@file	Gauge.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "PaintBrush.h"
#include "../../Math/Vector2f.h"

namespace Utility
{
	class Gauge : public PaintBrush
	{
	public:
		/**
		 *	@struct	Property
		 *	@brief	ゲージのプロパティ
		 */
		struct Property
		{
			float MinValue;
			float MaxValue;
			float Value;

			float MinPos;
			float MaxPos;
			float Pos;

			operator float() const;
			float operator=(float value);

			void UpdatePosition();
		};

	protected:
		D2D1::ColorF color_;
		float wight_;
		float radius_;
	protected:
		virtual void Draw() override;

	public:
		Gauge();
		virtual ~Gauge();

	public:
		Property property_;
		Math::Vector2 pos_;

	public:
		void Draw(ID2D1RenderTarget* argRt);

		inline void LineWight(float argWight) { LineWidth(argWight); }
		inline void Color(D2D1::ColorF argColor) { color_ = argColor; }
		inline void Radius(float argRadius) { radius_ = argRadius; }
		inline void Wight(float argWight) { wight_ = argWight; }
	};
};