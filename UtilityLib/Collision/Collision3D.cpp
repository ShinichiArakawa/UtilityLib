/**
 *	@file	CollisionNode3D.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "Collision3D.h"
#include "../../Function.h"

namespace
{
	float GetSqDistanceSegment2Segment(const Utility::Segment& argSegment0, const Utility::Segment& argSegment1);
	float GetSqDistancePoint2Segment(const Utility::Math::Vector3& argPoint, const Utility::Segment& argSegment);
	void Utility::ClosestPtSegment2Segment(const Segment& argSegment0, const Segment& argSegment1, Math::Vector3* argClosest0, Math::Vector3* argClosest1);
	void Utility::ClosestPtPoint2Segment(const Math::Vector3& argPoint, const Segment& argSegment, Math::Vector3* argClosest);


	/**
	 *	@fn			VectorLenghSQ
	 *	@brief		平方根の計算
	 *	@param[in]	argVector	!<	平方根の値
	 *	@return		平方根の解
	 */
	float VectorLenghSQ(const Utility::Math::Vector3& argVector)
	{
		float result = argVector.x * argVector.x + argVector.y * argVector.y + argVector.z * argVector.z;
		return result;
	}
	/**
	 *	@fn			Distance3DSQ
	 *	@brief		立体空間の距離の平方根の計算
	 *	@param[in]	argPoint1	!<	基準点
	 *	@param[in]	argPoint2	!<	目的地
	 *	@return		引数のポイント同士の距離を平方根とした解
	 */
	float Distance3DSQ(const Utility::Math::Vector3& argPoint1, const Utility::Math::Vector3& argPoint2)
	{
		Utility::Math::Vector3 sub = argPoint1 - argPoint2;
		return VectorLenghSQ(sub);
	}
	/**
	 *	@fn			GetSqDistanceSegment2Segment
	 *	@brief		線分と線分の距離の二乗を取得
	 *	@param[in]	argSegment0	!<	線分0
	 *	@param[in]	argSegment1	!<	線分1
	 *	@return		線分と線分の距離の二乗
	 *	@note		平方根を算出しない版
	 *	①直線上の最接近点が両方の線分の内側に存在する時
	 *	②直線上の最接近点が一方のみの線分の内側に存在する時
	 *	③直線上の最接近点が両方の線分の外側に存在する時
	 *	それぞれのケースで二つの線分上の最接近点を求め、その距離を算出する
	 */
	float GetSqDistanceSegment2Segment(const Utility::Segment& argSegment0, const Utility::Segment& argSegment1)
	{
		const float epsilon = 1.0e-5f;	// 誤差吸収用の微小な値
		Utility::Math::Vector3 v = Utility::Math::Vector3();		// c1→c0ベクトル

		Utility::Math::Vector3 d0 = argSegment0.end - argSegment0.start;	// 線分0の方向ベクトル
		Utility::Math::Vector3 d1 = argSegment1.end - argSegment1.start; // 線分1の方向ベクトル
		Utility::Math::Vector3 r = argSegment0.start - argSegment1.start; // 線分1の始点から線分0の始点へのベクトル
		float a = d0.Dot(d0);		// 線分0の距離の二乗
		float e = d1.Dot(d1);		// 線分1の距離の二乗
							//	b = d0.Dot(d1);		// 最適化の為後方に移動した
							//	c = d0.Dot(r);		// 最適化の為後方に移動した
							//	f = d1.Dot(r);		// 最適化の為後方に移動した

							// いづれかの線分の長さが0かどうかチェック
		if (a <= epsilon && e <= epsilon)
		{// 両方長さ0
			v = argSegment0.start - argSegment1.start;

			return v.Dot(v);
		}

		if (a <= epsilon)
		{// 線分0が長さ0
			return GetSqDistancePoint2Segment(argSegment0.start, argSegment1);
		}

		if (e <= epsilon)
		{// 線分1が長さ0
			return GetSqDistancePoint2Segment(argSegment1.start, argSegment0);
		}

		float b = d0.Dot(d1);
		float f = d1.Dot(r);
		float c = d0.Dot(r);

		float denom = a * e - b * b;	//	!<	常に非負
								// 線分が平行でない場合、直線0上の直線1に対する最近接点を計算、そして
								// 線分0上にクランプ。そうでない場合は任意のsを選択

		float s = 0.f, t = 0.f;

		if (denom != 0)
		{
			s = Utility::Math::Clamp((b * f - c * e) / denom, 0, 1);
		}
		else
		{
			s = 0;
		}

		// 直線1上の最接近点を計算
		float tnom = b * s + f;

		if (tnom < 0)
		{
			t = 0;
			s = Utility::Math::Clamp(-c / a, 0, 1);
		}
		else if (tnom > e)
		{
			t = 1;
			s = Utility::Math::Clamp((b - c) / a, 0, 1);
		}
		else
		{
			t = tnom / e;
		}

		Utility::Math::Vector3 c0 = s * d0 + argSegment0.start;
		Utility::Math::Vector3 c1 = t * d1 + argSegment1.start;
		v = c0 - c1;

		return v.Dot(v);
	}

	/**
	 *	@fn			GetSqDistanceSegment2Segment
	 *	@brief		点と線分の距離の二乗を取得
	 *	@param[in]	argPoint	!<	点
	 *	@param[in]	argSegment	!<	線分
	 *	@return		点と線分の距離の二乗
	 *	@note		平方根を算出しない版
	 *	線分の端点をA,B	点をCとして、
	 *	①ABベクトルとACベクトルの内積が負の時、点Aが点Cの最近傍である
	 *	②BAベクトルとBCベクトルの内積が負の時、点Bが点Cの最近傍である
	 *	③　①、②に該当しない場合、点Cの射影がACの内側に存在するため、その点が最近傍である
	 */
	float GetSqDistancePoint2Segment(const Utility::Math::Vector3& argPoint, const Utility::Segment& argSegment)
	{
		const float epsilon = 1.0e-5f;	//	!<	誤差吸収用の微小な値

		// 線分の始点から終点へのベクトル
		Utility::Math::Vector3 SegmentSub = argSegment.end - argSegment.start;

		// 線分の始点から点へのベクトル
		Utility::Math::Vector3 SegmentPoint = argPoint - argSegment.start;
		if (SegmentSub.Dot(SegmentPoint) < epsilon)
		{// ２ベクトルの内積が負なら、線分の始点が最近傍
			return SegmentPoint.Dot(SegmentPoint);
		}

		// 点から線分の終点へのベクトル
		SegmentPoint = argSegment.end - argPoint;
		if (SegmentSub.Dot(SegmentPoint) < epsilon)
		{// ２ベクトルの内積が負なら、線分の終点が最近傍
			return SegmentPoint.Dot(SegmentPoint);
		}

		// 上記のどちらにも該当しない場合、線分上に落とした射影が最近傍
		// (本来ならサインで求めるが、外積の大きさ/線分のベクトルの大きさで求まる)
		Utility::Math::Vector3 CP = SegmentSub.Cross(SegmentPoint);

		return CP.Dot(CP) / SegmentSub.Dot(SegmentSub);
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
bool Utility::CheckSphere2Sphere(const Sphere& argSphereA, const Sphere& argSphereB, Math::Vector3 *argInter)
{
	float distanceSQ = Distance3DSQ(argSphereA.center, argSphereB.center);
	float radiusSum = argSphereA.radius + argSphereB.radius;
	float radiusSumsQ = radiusSum * radiusSum;

	if (distanceSQ > radiusSumsQ)
	{
		return false;
	}
	if (argInter)
	{
		Math::Vector3 sub = argSphereA.center - argSphereB.center;
		Math::Vector3 BtoInter = sub * argSphereB.radius / (argSphereA.radius + argSphereB.radius);
		*argInter = argSphereB.center + BtoInter;
	}

	return true;
}
/**
 *	@fn			CheckCapsule2Capsule
 *	@brief		カプセル型とカプセル型の当たり判定
 *	@param[in]	argCapsuleA	!<	カプセルA
 *	@param[in]	argCapsuleB	!<	カプセルB
 *	@param[in]	argInter	!<	交点（省略可）
 *	@return		カプセル同士が当たっていればtrue、そうでなければfalse
 */
bool Utility::CheckCapsule2Capsule(const Capsule& argCapsuleA, const Capsule& argCapsuleB, Math::Vector3 *argInter)
{
	float distanceSQ = GetSqDistanceSegment2Segment(argCapsuleA.segment, argCapsuleB.segment);
	float radiusSum = argCapsuleA.radius + argCapsuleB.radius;
	float radiusSumsQ = radiusSum * radiusSum;

	if (distanceSQ > radiusSumsQ)
	{
		return false;
	}
	if (argInter) 
	{
		Math::Vector3 cponitA, cponitB;

		ClosestPtSegment2Segment(argCapsuleA.segment, argCapsuleB.segment, &cponitA, &cponitB);

		Math::Vector3 sub = cponitA - cponitB;

		Math::Vector3 BtoInter = sub * argCapsuleB.radius / (argCapsuleA.radius + argCapsuleB.radius);
		*argInter = cponitB+ BtoInter;
	}

	return true;
}
/**
 *	@fn			CheckCapsule2Capsule
 *	@brief		球型とカプセル型の当たり判定
 *	@param[in]	argSphere	!<	球
 *	@param[in]	argCapsule	!<	カプセル
 *	@param[in]	argInter	!<	交点（省略可）
 *	@return		球とカプセルが当たっていればtrue、そうでなければfalse
 */
bool Utility::CheckSphere2Capsule(const Sphere& argSphere, const Capsule& argCapsule, Math::Vector3 *argInter)
{
	float distanceSQ = GetSqDistancePoint2Segment(argSphere.center, argCapsule.segment);
	float radiusSum = argSphere.radius + argCapsule.radius;
	float radiusSumsQ = radiusSum * radiusSum;

	if (distanceSQ > radiusSumsQ)
	{
		return false;
	}
	if (argInter) 
	{
		Math::Vector3 cponit;

		ClosestPtPoint2Segment(argSphere.center, argCapsule.segment, &cponit);

		Math::Vector3 sub = argSphere.center - cponit;

		Math::Vector3 BtoInter = sub * argCapsule.radius / (argSphere.radius + argCapsule.radius);
		*argInter = cponit + BtoInter;
	}

	return true;
}
/**
 *	@fn			ClosestPtPoint2Segment
 *	@brief		点と線分の最近接点を計算
 *	@param[in]	argPoint	!<	点
 *	@param[in]	argSegment	!<	線分
 *	@param[in]	argClosest	!<	点と位置の最近接点（結果出力用）
 */
void Utility::ClosestPtPoint2Segment(const Math::Vector3& argPoint, const Segment& argSegment, Math::Vector3* argClosest)
{
	Math::Vector3 segv = argSegment.end - argSegment.start;
	Math::Vector3 s2p = argPoint - argSegment.start;
	float t = segv.Dot(s2p) / segv.Dot(segv);

	// 線分の外側にある場合、tを最接近点までクランプ
	t = Math::Clamp(t, 0, 1);

	// クランプされているtからの射影されている位置を計算
	*argClosest = t * segv + argSegment.start;
}
/**
 *	@fn			ClosestPtSegment2Segment
 *	@brief		線分と線分の最近接点を計算
 *	@param[in]	argSegment0	!<	線分0
 *	@param[in]	argSegment1	!<	線分1
 *	@param[in]	argClosest0	!<	線分0上の最近接点（結果出力用）
 *	@param[in]	argClosest1	!<	線分1上の最近接点（結果出力用）
 */
void Utility::ClosestPtSegment2Segment(const Segment& argSegment0, const Segment& argSegment1, Math::Vector3* argClosest0, Math::Vector3* argClosest1)
{
	const float epsilon = 1.0e-5f;	//	!<	誤差吸収用の微小な値

	Math::Vector3 d0 = argSegment0.end - argSegment0.start;		//	!<	線分0の方向ベクトル
	Math::Vector3 d1 = argSegment1.end - argSegment1.start;		//	!<	線分1の方向ベクトル
	Math::Vector3 r = argSegment0.start- argSegment1.start;		//	!<	線分1の始点から線分0の始点へのベクトル
	float a = d0.Dot(d0);	//	!<	線分0の距離の二乗
	float e = d1.Dot(d1);	//	!<	線分1の距離の二乗
					//	b = d0.Dot(d1);	// 最適化の為後方に移動した
					//	c = d0.Dot(r);	// 最適化の為後方に移動した
					//	f = d1.Dot(r);	// 最適化の為後方に移動した

	//	いづれかの線分の長さが0かどうかチェック
	if (a <= epsilon && e <= epsilon)
	{// 両方長さ0
		*argClosest0 = argSegment0.start;
		*argClosest1 = argSegment1.start;
		return;
	}

	if (a <= epsilon)
	{// 線分0が長さ0
		*argClosest0 = argSegment0.start;
		ClosestPtPoint2Segment(argSegment0.start, argSegment1, argClosest1);
	}

	if (e <= epsilon)
	{// 線分1が長さ0
		*argClosest1 = argSegment1.start;
		ClosestPtPoint2Segment(argSegment1.start, argSegment0, argClosest0);
	}

	float b = d0.Dot(d1);
	float f = d1.Dot(r);
	float c = d0.Dot(r);

	float denom = a * e - b * b;	//	!<	常に非負
									// 線分が平行でない場合、直線0上の直線1に対する最近接点を計算、そして
									// 線分0上にクランプ。そうでない場合は任意のsを選択
	
	float s = 0.f, t = 0.f;
	
	if (denom != 0)
	{
		s = Math::Clamp((b * f - c * e) / denom, 0, 1);
	}
	else
	{
		s = 0;
	}

	// 直線1上の最接近点を計算
	float tnom = b * s + f;

	if (tnom < 0)
	{
		t = 0;
		s = Math::Clamp(-c / a, 0, 1);
	}
	else if (tnom > e)
	{
		t = 1;
		s = Math::Clamp((b - c) / a, 0, 1);
	}
	else
	{
		t = tnom / e;
	}

	*argClosest0 = s * d0 + argSegment0.start;
	*argClosest1 = t * d1 + argSegment1.start;
}
/**
 *	@fn			ClosestPtPoint2Triangle
 *	@brief		線分と線分の最近接点を計算
 *	@param[in]	argPoint	!<	点
 *	@param[in]	argTriangle	!<	線分
 *	@param[in]	argClosest	!<	点と線分の最近接点（結果出力用）
 */
void Utility::ClosestPtPoint2Triangle(const Math::Vector3& argPoint, const Triangle& argTriangle, Math::Vector3* argClosest)
{
	// argPointがPointTopの外側の頂点領域の中にあるかどうかチェック
	Math::Vector3 P0_P1 = argTriangle.pointLeft - argTriangle.pointTop;
	Math::Vector3 P0_P2 = argTriangle.pointRight - argTriangle.pointTop;
	Math::Vector3 P0_PT = argPoint - argTriangle.pointTop;

	float d1 = P0_P1.Dot(P0_PT);
	float d2 = P0_P2.Dot(P0_PT);

	if (d1 <= 0.0f && d2 <= 0.0f)
	{// PintTopが最近傍
		*argClosest = argTriangle.pointTop;
		return;
	}

	// argPointがPointLeftの外側の頂点領域の中にあるかどうかチェック
	Math::Vector3 P1_PT = argPoint - argTriangle.pointLeft;

	float d3 = P0_P1.Dot(P1_PT);
	float d4 = P0_P2.Dot(P1_PT);

	if (d3 >= 0.0f && d4 <= d3)
	{// PointLeftが最近傍
		*argClosest = argTriangle.pointLeft;
		return;
	}

	// argPointがPointTop_PointLeftの辺領域の中にあるかどうかチェックし、あればargPointのPointTop_PointLeft上に対する射影を返す
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		*argClosest = argTriangle.pointTop + v * P0_P1;
		return;
	}

	// argPointがPointRightの外側の頂点領域の中にあるかどうかチェック
	Math::Vector3 P2_PT = argPoint - argTriangle.pointRight;

	float d5 = P0_P1.Dot(P2_PT);
	float d6 = P0_P2.Dot(P2_PT);
	if (d6 >= 0.0f && d5 <= d6)
	{
		*argClosest = argTriangle.pointRight;
		return;
	}

	// argPointがPointTop_PointRightの辺領域の中にあるかどうかチェックし、あればargPointのPointTop_PointRight上に対する射影を返す
	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		*argClosest = argTriangle.pointTop + w * P0_P2;
		return;
	}

	// argPointがPointLeft_PointRightの辺領域の中にあるかどうかチェックし、あればargPointのPointLeft_PointRight上に対する射影を返す
	float va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		*argClosest = argTriangle.pointLeft + w * (argTriangle.pointRight - argTriangle.pointLeft);
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*argClosest = argTriangle.pointTop + P0_P1 * v + P0_P2 * w;
}
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
bool Utility::CheckPoint2Triangle(const Math::Vector3& argPoint, const Triangle& argTriangle)
{
	// 点0→1、 1→2、 2→0 のベクトルをそれぞれ計算
	Math::Vector3 v01 = argTriangle.pointLeft - argTriangle.pointTop;
	Math::Vector3 v12 = argTriangle.pointRight - argTriangle.pointLeft;
	Math::Vector3 v20 = argTriangle.pointTop - argTriangle.pointRight;
	// 三角形の各頂点から点へのベクトルをそれぞれ計算
	Math::Vector3 v0p = argPoint - argTriangle.pointTop;
	Math::Vector3 v1p = argPoint - argTriangle.pointLeft;
	Math::Vector3 v2p = argPoint - argTriangle.pointRight;
	// 各辺ベクトルと、点へのベクトルの外積を計算
	Math::Vector3 c0 = v01.Cross(v0p);
	Math::Vector3 c1 = v12.Cross(v1p);
	Math::Vector3 c2 = v20.Cross(v2p);
	// 内積で同じ方向かどうか調べる
	float dot01 = c0.Dot(c1);
	float dot02 = c0.Dot(c2);
	// 外積ベクトルが全て同じ方向なら、三角形の内側に点がある
	if (dot01 > 0 && dot02 > 0)
	{
		return true;
	}

	//三角形の外側に点がある
	return false;

}
/**
 *	@fn			CheckSphere2Triangle
 *	@brief		球と法線付き三角形の当たりチェック
 *	@param[in]	argSphere	!<	球
 *	@param[in]	argTriangle	!<	法線付き三角性
 *	@param[in]	argInter	!<	交点（省略可）
 *	@return		点と三角形が交差していればtrue、そうでなければfalse
 *	@note		裏面の当たり判定は取らない
 */
bool Utility::CheckSphere2Triangle(const Sphere& argSphere, const Triangle& argTriangle, Math::Vector3 *argInter)
{
	Math::Vector3 p;

	// 球の中心に対する最近接点である三角形上にある点pを見つける
	ClosestPtPoint2Triangle(argSphere.center, argTriangle, &p);

	Math::Vector3 v = p - argSphere.center;

	// 球と三角形が交差するのは、球の中心から点pまでの距離が球の半径よりも小さい場合
	if (v.Dot(v) <= argSphere.radius * argSphere.radius)
	{
		if (argInter)
		{
			*argInter = p;
		}

		return true;
	}

	return false;
#if 0
	// 球と平面（三角形が乗っている平面）の当たり判定
	// 球と平面の距離を計算
	float ds = argSphere.center.Dot(argTriangle.normal);
	float dt = argTriangle.pointTop.Dot(argTriangle.normal);
	float dist = ds - dt;
	// 距離が半径以上なら、当たらない
	if (fabsf(dist) > argSphere.radius)	
		return false;
	// 中心点を平面に射影したとき、三角形の内側にあれば、当たっている
	// 射影した座標
	Math::Vector3 center = -dist * argTriangle.normal+ argSphere.center;

	// 三角形の外側になければ、当たっていない
	if (!CheckPoint2Triangle(center, argTriangle))	
		return false;

	if (argInter)
	{
		*argInter = center;	// 交点をコピー
	}

	return true;
#endif
}
/**
 *	@fn			CheckSphere2Triangle
 *	@brief		球と法線付き三角形の当たりチェック
 *	@param[in]	argPointTop		!<	三角形の頂点
 *	@param[in]	argPointLeft	!<	三角形の左角
 *	@param[in]	argPointRight	!<	三角形の右角
 *	@param[in]	argTriangle		!<	操作したい三角形
 */
void Utility::ComputeTriangle(const Math::Vector3& argPointTop, const Math::Vector3& argPointLeft, const Math::Vector3& argPointRight, Triangle* argTriangle)
{
	argTriangle->pointTop = argPointTop;
	argTriangle->pointLeft = argPointLeft;
	argTriangle->pointRight = argPointRight;

	Math::Vector3 P0_P1 = argTriangle->pointLeft - argTriangle->pointTop;
	Math::Vector3 P1_P2 = argTriangle->pointRight - argTriangle->pointLeft;

	argTriangle->normal = P0_P1.Cross(P1_P2);
	argTriangle->normal.Normalize();
}
/**
 *	@fn			CheckSegment2Triangle
 *	@brief		線分（有向）と法線付き三角形の当たりチェック
 *	@param[in]	argSegment	!<	光線の線分
 *	@param[in]	argTriangle	!<	法線付き三角形
 *	@param[in]	argInter	!<	交点（省略可）
 *	@return		線分と三角形が交差していればtrue、そうでなければfalse
 *	@note		裏面の当たりはとらない
 */
bool Utility::CheckSegment2Triangle(const Segment& argSegment, const Triangle& argTriangle, Math::Vector3 *argInter)
{
	const float epsilon = -1.0e-5f;	//	!<	誤差吸収用の微小な値

	Math::Vector3 tls = argSegment.start - argTriangle.pointTop;
	float distl0 = tls.Dot(argTriangle.normal);	//	!<	線分の始点と平面の距離
	if (distl0 <= epsilon)
	{// 線分の始点が三角系の裏側にあれば、当たらない
		return false;
	}
	
	Math::Vector3 tle = argSegment.end - argTriangle.pointTop;
	float distl1 = tle.Dot(argTriangle.normal);	//	!<	線分の終点と平面の距離
	if (distl1 >= -epsilon)
	{// 線分の終点が三角系の表側にあれば、当たらない
		return false;
	}
	// 直線と平面との交点sを取る
	float denom = distl0 - distl1;
	float t = distl0 / denom;
	Math::Vector3 LayV = argSegment.end - argSegment.start;	//	!<	線分の方向ベクトルを取得
	Math::Vector3 s = t * LayV + argSegment.start;

	// 交点が三角形の内側にあるかどうかを調べる。
	// 三角形の内側にある場合、交点から各頂点へのベクトルと各辺ベクトルの外積（三組）が、
	// 全て法線と同じ方向を向く
	// 一つでも方向が一致しなければ、当たらない。
	Math::Vector3 st0 = argTriangle.pointTop - s;
	Math::Vector3 t01 = argTriangle.pointLeft - argTriangle.pointTop;
	Math::Vector3 m = st0.Cross(t01);
	float dp = m.Dot(argTriangle.normal);
	if (dp <= epsilon)
	{
		return false;
	}
	Math::Vector3 st1 = argTriangle.pointLeft - s;
	Math::Vector3 t12 = argTriangle.pointRight - argTriangle.pointLeft;
	m = st1.Cross(t12);
	dp = m.Dot(argTriangle.normal);
	if (dp <= epsilon)
	{
		return false;
	}
	Math::Vector3 st2 = argTriangle.pointRight - s;
	Math::Vector3 t20 = argTriangle.pointTop - argTriangle.pointRight;
	m = st2.Cross(t20);
	dp = m.Dot(argTriangle.normal);
	if (dp <= epsilon)
	{
		return false;
	}
	if (argInter)
	{//	交点をコピー
		*argInter = s;	
	}

	return true;
}
