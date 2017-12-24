/**
 *	@file	CollisionNode3D.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../Math/Math.h"

namespace Utility
{
	/**
	 *	@struct	Triangle
	 *	@brief	法線付き三角形（反時計回りが表面）
	 */
	struct Triangle
	{
	public:
		Math::Vector3	pointTop;
		Math::Vector3	pointLeft;
		Math::Vector3	pointRight;
		Math::Vector3	normal;
	};
	/**
	 *	@struct	Sphere
	 *	@brief	球型の当たり判定
	 */
	struct Sphere
	{
	public:
		Math::Vector3 center;
		float radius;

		Sphere()
			:radius(1.0f)
		{
		}
	};
	/**
	 *	@struct	Segment
	 *	@brief	当たり判定に使う線分
	 */
	struct Segment
	{
	public:
		Math::Vector3 start;
		Math::Vector3 end;

		Segment()
		{
		}

		Segment(Math::Vector3 argStart, Math::Vector3 argEnd)
			:start(argStart), end(argEnd)
		{
		}
	};
	/**
	 *	@struct	Capsule
	 *	@brief	カプセル型の当たり判定
	 */
	struct Capsule
	{
	public:
		Segment segment;
		float radius;
		Capsule()
			:segment(Math::Vector3(), Math::Vector3(0, 1, 0)), radius(1.0f)
		{
		}

	};
	/**
	 *	@fn			CheckCapsule2Capsule
	 *	@brief		カプセル型とカプセル型の当たり判定
	 *	@param[in]	argSphereA	!<	球A
	 *	@param[in]	argSphereA	!<	球B
	 *	@param[in]	argInter	!<	交点（省略可）
	 *	@return		球同士が当たっていればtrue、そうでなければfalse
	 */
	bool CheckSphere2Sphere(const Sphere& argSphereA, const Sphere& argSphereB, Math::Vector3 *argInter = nullptr);
	/**
	 *	@fn			CheckCapsule2Capsule
	 *	@brief		カプセル型とカプセル型の当たり判定
	 *	@param[in]	argCapsuleA	!<	カプセルA
	 *	@param[in]	argCapsuleB	!<	カプセルB
	 *	@param[in]	argInter	!<	交点（省略可）
	 *	@return		カプセル同士が当たっていればtrue、そうでなければfalse
	 */
	bool CheckCapsule2Capsule(const Capsule& argCapsuleA, const Capsule& argCapsuleB, Math::Vector3 *argInter = nullptr);
	/**
	 *	@fn			CheckSphere2Capsule
	 *	@brief		球型とカプセル型の当たり判定
	 *	@param[in]	argSphere	!<	球
	 *	@param[in]	argCapsule	!<	カプセル
	 *	@param[in]	argInter	!<	交点（省略可）
	 *	@return		球とカプセルが当たっていればtrue、そうでなければfalse
	 */
	bool CheckSphere2Capsule(const Sphere& argSphereA, const Capsule& argCapsuleB, Math::Vector3 *argInter = nullptr);
	/**
	 *	@fn			ClosestPtSegment2Segment
	 *	@brief		線分と線分の最近接点を計算
	 *	@param[in]	argSegment0	!<	線分0
	 *	@param[in]	argSegment1	!<	線分1
	 *	@param[in]	argClosest0	!<	線分0上の最近接点（結果出力用）
	 *	@param[in]	argClosest1	!<	線分1上の最近接点（結果出力用）
	 */
	void ClosestPtSegment2Segment(const Segment& argSegment0, const Segment& argSegment1, Math::Vector3* argClosest0, Math::Vector3* argClosest1);
	/**
	 *	@fn			ClosestPtPoint2Segment
	 *	@brief		点と線分の最近接点を計算
	 *	@param[in]	argPoint	!<	点
	 *	@param[in]	argSegment	!<	線分
	 *	@param[in]	argClosest	!<	点と位置の最近接点（結果出力用）
	 */
	void ClosestPtPoint2Segment(const Math::Vector3& argPoint, const Segment& argSegment, Math::Vector3* argClosest);
	/**
	 *	@fn			ClosestPtPoint2Triangle
	 *	@brief		線分と線分の最近接点を計算
	 *	@param[in]	argPoint	!<	点
	 *	@param[in]	argTriangle	!<	線分
	 *	@param[in]	argClosest	!<	点と線分の最近接点（結果出力用）
	 */
	void ClosestPtPoint2Triangle(const Math::Vector3& argPoint, const Triangle& argTriangle, Math::Vector3* argClosest);
	/**
	 *	@fn			CheckPoint2Triangle
	 *	@brief		点と三角形の当たり判定
	 *	@param[in]	argPoint	!<	点
	 *	@param[in]	argTriangle	!<	線分
	 *	@return		点と三角形が当たって入ればtrue、そうでなければfalse
	 *	@note		点と三角形は同一平面上にあるものとしています。同一平面上に無い場合は正しい結果になりません
	 *	@note		線上は外とみなします
	 *	@note		ABCが三角形かどうかのチェックは省略...
	 */
	bool CheckPoint2Triangle(const Math::Vector3& argPoint, const Triangle& argTriangle);
	/**
	 *	@fn			CheckSphere2Triangle
	 *	@brief		球と法線付き三角形の当たりチェック
	 *	@param[in]	argSphere	!<	球
	 *	@param[in]	argTriangle	!<	法線付き三角性
	 *	@param[in]	argInter	!<	交点（省略可）
	 *	@return		点と三角形が交差していればtrue、そうでなければfalse
	 *	@note		裏面の当たり判定は取らない
	 */
	bool CheckSphere2Triangle(const Sphere& argSphere, const Triangle& argTriangle, Math::Vector3 *argInter = nullptr);
	/**
	 *	@fn			CheckSphere2Triangle
	 *	@brief		球と法線付き三角形の当たりチェック
	 *	@param[in]	argPointTop		!<	三角形の頂点
	 *	@param[in]	argPointLeft	!<	三角形の左角
	 *	@param[in]	argPointRight	!<	三角形の右角
	 *	@param[in]	argTriangle		!<	操作したい三角形
	 */
	void ComputeTriangle(const Math::Vector3& argPointTop, const Math::Vector3& argPointLeft, const Math::Vector3& argPointRight, Triangle* argTriangle);
	/**
	 *	@fn			CheckSegment2Triangle
	 *	@brief		線分（有向）と法線付き三角形の当たりチェック
	 *	@param[in]	argSegment	!<	光線の線分
	 *	@param[in]	argTriangle	!<	法線付き三角形
	 *	@param[in]	argInter	!<	交点（省略可）
	 *	@return		線分と三角形が交差していればtrue、そうでなければfalse
	 *	@note		裏面の当たりはとらない
	 */
	bool CheckSegment2Triangle(const Segment& argSegment, const Triangle& argTriangle, Math::Vector3 *argInter = nullptr);
};