/**
 *	@file	Math.h
 *	@date	2017 / 10 / 01
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "Matrix.h"
#include "Quaternion.h"
#include "Vector2f.h"
#include "Vector3f.h"
#include "Vector4f.h"

#include <algorithm>
#include <windows.h>

namespace Utility
{
	namespace Math
	{
#pragma region		Alias
		using XMMatrix4x3 = DirectX::XMFLOAT4X3;	//	!<	XMMATRIX保持用行列
		using XMMatrix4x4 = DirectX::XMFLOAT4X4;	//	!<	XMMATRIX保持用行列
		using XMMatrix = DirectX::XMMATRIX;			//	!<	SIMD演算用行列
		using CXMMatrix = DirectX::CXMMATRIX;		//	!<	SIMD演算用行列の定数参照
		using FXMMatrix = DirectX::FXMMATRIX;		//	!<	SIMD演算用行列の定数

		using XMVector2 = DirectX::XMFLOAT2;		//	!<	XMVECTOR保持用ベクトル
		using XMVector3 = DirectX::XMFLOAT3;		//	!<	XMVECTOR保持用ベクトル
		using XMVector4 = DirectX::XMFLOAT4;		//	!<	XMVECTOR保持用ベクトル
		using XMVector = DirectX::XMVECTOR;			//	!<	SIMD演算用ベクトル
		using CXMVector = DirectX::CXMVECTOR;		//	!<	SIMD演算用ベクトルの定数参照
		using FXMVector = DirectX::FXMVECTOR;		//	!<	SIMD演算用ベクトルの定数
		using GXMVector = DirectX::GXMVECTOR;
#pragma endregion	Alias

#pragma region		Variables
		const float PI = 3.14159265358979323846f;
		const float PI2 = PI * 2.f;
#pragma endregion	Variables

#pragma region		Method
#undef min
#undef max

		/**
		 *	@fn			XorShift
		 *	@brief		疑似乱数生成アルゴリズム。演算が排他的論理和とシフトだけなので高速。
		 *	@return		生成した疑似乱数
		 */
		inline int XorShift()
		{
			static int x = 123456789, y = 362436069, z = 521288629, w = 88675123;
			int t = (x ^ (x << 11));
			x = y;
			y = z;
			z = w;
			return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
		}
		/**
		*	@fn			Rang
		*	@brief		疑似乱数生成
		*	@param[in]	argAngle	!<	シードにしたい角度
		*	@return		生成した疑似乱数
		*/
		inline double Rang(double argAngle)
		{
			return (-argAngle + static_cast<int>(argAngle) * 2 * XorShift() % (10000 / 10000));
		}
		/**
		 *	@fn			Clamp
		 *	@brief		値を引数のmin～maxの間に補正する
		 *	@param[in]	argSeedValue	!<	補正をかけたい値
		 *	@param[in]	argMinValue		!<	最小値
		 *	@param[in]	argMaxValue		!<	最大値
		 *	@return		補正をかけた値
		 */
		inline float Clamp(float argSeedValue, float argMinValue, float argMaxValue)
		{
			return std::min(std::max(argSeedValue, argMinValue), argMaxValue);
		}
		/**
		 *	@fn			ToDegree
		 *	@brief		radian値をDegree値に変換する
		 *	@param[in]	argRadian	!<	Degree値に変換したいradian値
		 *	@return		変換した角度
		 */
		inline float ToDegree(float radian)
		{
			return radian * 180.f / PI;
		}
		/**
		 *	@fn			ToRadian
		 *	@brief		Degree値をradian値に変換する
		 *	@param[in]	argDegree	!<	Radian値に変換したいdegree値
		 *	@return		変換した角度
		 */
		inline float ToRadian(float degree)
		{
			return degree * PI / 180.f;
		}
		/**
		 *	@fn			LoadRect
		 *	@brief		レクト型からXMVectorへの変換
		 *	@param[in]	argRect	!<	XMVectorに変換したい値
		 *	@return		XMvectorに変換したRect型の値
		 */
		inline XMVector LoadRect(RECT const* argRect)
		{
			XMVector vec = DirectX::XMLoadInt4(reinterpret_cast<uint32_t const*>(argRect));

			vec = DirectX::XMConvertVectorIntToFloat(vec, 0);

			// Convert right/bottom to width/height.
			vec = DirectX::XMVectorSubtract(vec, DirectX::XMVectorPermute<0, 1, 4, 5>(DirectX::XMVectorZero(), vec));

			return vec;
		}
		/**
		 *	@fn			LoopValue
		 *	@brief		値を引数のmin～maxの間でループさせる
		 *	@param[in]	argSeedValue	!<	ループさせたい値
		 *	@param[in]	argMinValue		!<	最小値
		 *	@param[in]	argMaxValue		!<	最大値
		 *	@return		補正をかけた値
		 */
		template<typename T>
		inline T LoopValue(T argSeedValue, T argMinValue, T argMaxValue)
		{
			T result = argSeedValue;

			if (argSeedValue > argMaxValue)
			{
				result = argMinValue;
			}
			else if (argSeedValue < argMinValue)
			{
				result = argMaxValue;
			}

			return result;
		}
		/**
		 *	@fn			Normalize
		 *	@brief		0～1までの正規化
		 *	@param[in]	argMin		!<	最小値
		 *	@param[in]	argMax		!<	最大値
		 *	@param[in]	argFactor	!<	正規化する値
		 *	@return		0～1の正規化された値
		 */
		inline const float Normalize(float argMin, float argMax, float argFactor)
		{
			return (argFactor - argMin) / (argMax - argMin);
		}

		/**
		*	@brief		指定されたbitを立てる
		*	@param[in]	argPos SetBitの位置 (0Bitスタート)
		*	@param[out]	argBit SetBit変数
		*/
		inline void SetBit(int argPos, uint16_t *argBit)
		{
			*argBit = (*argBit | (0x01 << argPos));
		}

		/**
		*	@brief		指定位置から指定個のビットを取得する
		*	@param[in]	argBit SetBit変数
		*	@param[in]	argPos 位置 0スタート
		*	@param[in]	argNum 個数 1スタート
		*	@return		取得されたbit情報
		*/
		inline uint16_t GetBits(uint16_t argBit, int argPos, int argNum)
		{
			return ((argBit >> argPos)&~(~0x0000 << argNum));
		}
		/**
		*	@brief		フラグが立っているかどうか
		*	@param[in]	argLeft		!<	比較されるSetBit変数
		*	@param[in]	argRight	!<	比較するSetBit変数
		*	@retval		true		!<	フラグが立っている
		*	@retval		false		!<	フラグが立って無い
		*/
		inline bool IsChackBits(uint16_t argLeft, uint16_t argRight)
		{
			return ((argLeft & argRight) != 0);
		}

#pragma endregion	Method
	};
};