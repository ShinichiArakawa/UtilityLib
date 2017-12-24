/**
 *	@file	Shape2D.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../Math/Math.h"

namespace Utility
{
	/**
	 *	@enum	eShapeType
	 *	@brief	形状の種類
	 */
	enum class eShapeType
	{
		Circle,
		Rect,

		Max,
	};

	/**
	 *	@class	IShape
	 *	@brief	形状のインターフェイスクラス
	 */
	class IShape
	{
	public:
		virtual ~IShape() = default;
		inline virtual eShapeType Type() const = 0;
		inline virtual Math::Vector2 Pos()const = 0;
		inline virtual float Radius()const = 0;
		inline virtual RECT Rect()const = 0;
	};

	/**
	 *	@class	CShape
	 *	@brief	形状のベースクラス
	 */
	class CShape : public IShape
	{
	protected:
		const Math::Vector2 *parent_;

	public:
		CShape(const Math::Vector2 *argParent)
			:parent_(argParent)
		{
		}
		virtual ~CShape() = default;
		inline virtual eShapeType Type() const = 0;
		inline virtual Math::Vector2 Pos()const = 0;
		inline virtual float Radius()const = 0;
		inline virtual RECT Rect()const = 0;
	};

	/**
	 *	@class	CircleShape
	 *	@brief	円形状クラス
	 */
	class CircleShape final : public CShape
	{
	private:
		const float		*radius_;
		Math::Vector2			pos_;
	public:
		/**
		 *	@constructor	CircleShape
		 *	@brief			絶対座標の当たり判定
		 *	@param[in]		argRadius	半径
		 *	@param[in]		argPos		座標
		 */
		CircleShape(float *argRadius, Math::Vector2 argPos)
			:CShape(nullptr), pos_(argPos), radius_(argRadius)
		{
		}
		/**
		 *	@constructor	CircleShape
		 *	@brief			相対座標の当たり判定、親と違う位置
		 *	@param[in]		argParent	当たり判定を持つ親座標
		 *	@param[in]		argRadius	半径
		 *	@param[in]		argPos		座標
		 */
		CircleShape(Math::Vector2 *argParent, float *argRadius, Math::Vector2 argPos)
			:CShape(argParent), pos_(argPos), radius_(argRadius)
		{
		}
		/**
		 *	@constructor	CircleShape
		 *	@brief			相対座標の当たり判定、親と同じ位置
		 *	@param[in]		argParent	当たり判定の親座標
		 *	@param[in]		argRadius	半径
		 */
		CircleShape(const Math::Vector2 *argParent, float *argRadius)
			: CShape(argParent), pos_(Math::Vector2()), radius_(argRadius)
		{
		}

		~CircleShape() = default;
		inline eShapeType Type()const override { return eShapeType::Circle; }
		inline Math::Vector2 Pos()const override { return (parent_) ? (*parent_ + pos_) : pos_; }
		inline float Radius()const override { return (radius_) ? *radius_ : 0.f; }
		inline RECT Rect()const override
		{
			assert(false && "Mistaking the SharpType");
			return RECT();
		}
	};

	/**
	 *	@class	RectShape
	 *	@brief	矩形形状クラス
	 */
	class RectShape : public CShape
	{
	private:
		RECT	rect_;

	public:
		RectShape(RECT argRect, Math::Vector2 *argParent)
			:CShape(argParent), rect_(argRect)
		{
		}

		virtual ~RectShape() = default;
		inline eShapeType Type()const override { return eShapeType::Rect; }
		inline RECT Rect()const override { return rect_; }
		inline virtual Math::Vector2 Pos()const override
		{
			assert(false && "Mistaking the SharpType");
			return Math::Vector2();
		}
		inline virtual float Radius()const override
		{
			assert(false && "Mistaking the SharpType");
			return 0.f;
		}
	};

};