/**
 *	@file	Vector2.h
 *	@date	2017 / 9 / 27
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <DirectXMath.h>
#include <functional>

namespace Utility
{
	namespace Math
	{
		struct Quaternion;
		struct Matrix;
		struct Vector4;

		struct Vector2 : public DirectX::XMFLOAT2
		{
		public:
			Vector2() : XMFLOAT2(0.f, 0.f) {}
			explicit Vector2(float x) : XMFLOAT2(x, x) {}
			Vector2(float _x, float _y) : XMFLOAT2(_x, _y) {}
			explicit Vector2(_In_reads_(2) const float *pArray) : XMFLOAT2(pArray) {}
			Vector2(DirectX::FXMVECTOR V) { XMStoreFloat2(this, V); }
			Vector2(const XMFLOAT2& V) { this->x = V.x; this->y = V.y; }
			explicit Vector2(const DirectX::XMVECTORF32& F) { this->x = F.f[0]; this->y = F.f[1]; }

		public:
#pragma region	operator
			operator DirectX::XMVECTOR() const { return XMLoadFloat2(this); }

			inline bool operator == (const Vector2& V) const
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat2(this);
				XMVECTOR v2 = XMLoadFloat2(&V);
				return XMVector2Equal(v1, v2);
			}

			inline bool operator != (const Vector2& V) const
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat2(this);
				XMVECTOR v2 = XMLoadFloat2(&V);
				return XMVector2NotEqual(v1, v2);
			}

			Vector2& operator= (const Vector2& V) { x = V.x; y = V.y; return *this; }
			Vector2& operator= (const XMFLOAT2& V) { x = V.x; y = V.y; return *this; }
			Vector2& operator= (const DirectX::XMVECTORF32& F) { x = F.f[0]; y = F.f[1]; return *this; }


			inline Vector2& operator+= (const Vector2& V)
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat2(this);
				XMVECTOR v2 = XMLoadFloat2(&V);
				XMVECTOR X = XMVectorAdd(v1, v2);
				XMStoreFloat2(this, X);
				return *this;
			}

			inline Vector2& operator-= (const Vector2& V)
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat2(this);
				XMVECTOR v2 = XMLoadFloat2(&V);
				XMVECTOR X = XMVectorSubtract(v1, v2);
				XMStoreFloat2(this, X);
				return *this;
			}

			inline Vector2& operator*= (const Vector2& V)
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat2(this);
				XMVECTOR v2 = XMLoadFloat2(&V);
				XMVECTOR X = XMVectorMultiply(v1, v2);
				XMStoreFloat2(this, X);
				return *this;
			}

			inline Vector2& operator*= (float S)
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat2(this);
				XMVECTOR X = XMVectorScale(v1, S);
				XMStoreFloat2(this, X);
				return *this;
			}

			inline Vector2& operator/= (float S)
			{
				using namespace DirectX;
				assert(S != 0.0f);
				XMVECTOR v1 = XMLoadFloat2(this);
				XMVECTOR X = XMVectorScale(v1, 1.f / S);
				XMStoreFloat2(this, X);
				return *this;
			}

			Vector2 operator+ () const { return *this; }
			Vector2 operator- () const { return Vector2(-x, -y); }
#pragma endregion	operator

		public:
#pragma region	Function

			inline bool InBounds(const Vector2& Bounds) const
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat2(this);
				XMVECTOR v2 = XMLoadFloat2(&Bounds);
				return XMVector2InBounds(v1, v2);
			}

			inline float Length() const
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat2(this);
				XMVECTOR X = XMVector2Length(v1);
				return XMVectorGetX(X);
			}

			inline float LengthSquared() const
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat2(this);
				XMVECTOR X = XMVector2LengthSq(v1);
				return XMVectorGetX(X);
			}

			inline float Dot(const Vector2& V) const
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat2(this);
				XMVECTOR v2 = XMLoadFloat2(&V);
				XMVECTOR X = XMVector2Dot(v1, v2);
				return XMVectorGetX(X);
			}

			inline void Cross(const Vector2& V, Vector2& result) const
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat2(this);
				XMVECTOR v2 = XMLoadFloat2(&V);
				XMVECTOR R = XMVector2Cross(v1, v2);
				XMStoreFloat2(&result, R);
			}

			inline Vector2 Cross(const Vector2& V) const
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat2(this);
				XMVECTOR v2 = XMLoadFloat2(&V);
				XMVECTOR R = XMVector2Cross(v1, v2);

				Vector2 result;
				XMStoreFloat2(&result, R);
				return result;
			}

			inline void Normalize()
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat2(this);
				XMVECTOR X = XMVector2Normalize(v1);
				XMStoreFloat2(this, X);
			}

			inline void Normalize(Vector2& result) const
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat2(this);
				XMVECTOR X = XMVector2Normalize(v1);
				XMStoreFloat2(&result, X);
			}
#pragma endregion	Function
#pragma region	StaticFunction
			static const Vector2 Zero;

			inline static float Distance(const Vector2& v1, const Vector2& v2)
			{
				using namespace DirectX;
				XMVECTOR x1 = XMLoadFloat2(&v1);
				XMVECTOR x2 = XMLoadFloat2(&v2);
				XMVECTOR V = XMVectorSubtract(x2, x1);
				XMVECTOR X = XMVector2Length(V);
				return XMVectorGetX(X);
			}

			inline static float DistanceSquared(const Vector2& v1, const Vector2& v2)
			{
				using namespace DirectX;
				XMVECTOR x1 = XMLoadFloat2(&v1);
				XMVECTOR x2 = XMLoadFloat2(&v2);
				XMVECTOR V = XMVectorSubtract(x2, x1);
				XMVECTOR X = XMVector2LengthSq(V);
				return XMVectorGetX(X);
			}

			inline static void Min(const Vector2& v1, const Vector2& v2, Vector2& result)
			{
				using namespace DirectX;
				XMVECTOR x1 = XMLoadFloat2(&v1);
				XMVECTOR x2 = XMLoadFloat2(&v2);
				XMVECTOR X = XMVectorMin(x1, x2);
				XMStoreFloat2(&result, X);
			}

			inline static Vector2 Min(const Vector2& v1, const Vector2& v2)
			{
				using namespace DirectX;
				XMVECTOR x1 = XMLoadFloat2(&v1);
				XMVECTOR x2 = XMLoadFloat2(&v2);
				XMVECTOR X = XMVectorMin(x1, x2);

				Vector2 result;
				XMStoreFloat2(&result, X);
				return result;
			}

			inline static void Max(const Vector2& v1, const Vector2& v2, Vector2& result)
			{
				using namespace DirectX;
				XMVECTOR x1 = XMLoadFloat2(&v1);
				XMVECTOR x2 = XMLoadFloat2(&v2);
				XMVECTOR X = XMVectorMax(x1, x2);
				XMStoreFloat2(&result, X);
			}

			inline static Vector2 Max(const Vector2& v1, const Vector2& v2)
			{
				using namespace DirectX;
				XMVECTOR x1 = XMLoadFloat2(&v1);
				XMVECTOR x2 = XMLoadFloat2(&v2);
				XMVECTOR X = XMVectorMax(x1, x2);

				Vector2 result;
				XMStoreFloat2(&result, X);
				return result;
			}

			inline static void Lerp(const Vector2& v1, const Vector2& v2, float t, Vector2& result)
			{
				using namespace DirectX;
				XMVECTOR x1 = XMLoadFloat2(&v1);
				XMVECTOR x2 = XMLoadFloat2(&v2);
				XMVECTOR X = XMVectorLerp(x1, x2, t);
				XMStoreFloat2(&result, X);
			}

			inline static Vector2 Lerp(const Vector2& v1, const Vector2& v2, float t)
			{
				using namespace DirectX;
				XMVECTOR x1 = XMLoadFloat2(&v1);
				XMVECTOR x2 = XMLoadFloat2(&v2);
				XMVECTOR X = XMVectorLerp(x1, x2, t);

				Vector2 result;
				XMStoreFloat2(&result, X);
				return result;
			}

			static void Transform(const Vector2& v, const Quaternion& quat, Vector2& result);
			static Vector2 Transform(const Vector2& v, const Quaternion& quat);
			static void Transform(const Vector2& v, const Matrix& m, Vector2& result);
			static Vector2 Transform(const Vector2& v, const Matrix& m);
			static void Transform(const Vector2* varray, size_t count, const Matrix& m, Vector2* resultArray);
			static void Transform(const Vector2& v, const Matrix& m, Vector4& result);
			static void Transform(const Vector2* varray, size_t count, const Matrix& m, Vector4* resultArray);
			static void TransformNormal(const Vector2& v, const Matrix& m, Vector2& result);
			static Vector2 TransformNormal(const Vector2& v, const Matrix& m);
			static void TransformNormal(const Vector2* varray, size_t count, const Matrix& m, Vector2* resultArray);
#pragma endregion	StaticFunction

		};


		inline Vector2 operator+ (const Vector2& V1, const Vector2& V2)
		{
			using namespace DirectX;
			XMVECTOR v1 = XMLoadFloat2(&V1);
			XMVECTOR v2 = XMLoadFloat2(&V2);
			XMVECTOR X = XMVectorAdd(v1, v2);
			Vector2 R;
			XMStoreFloat2(&R, X);
			return R;
		}

		inline Vector2 operator- (const Vector2& V1, const Vector2& V2)
		{
			using namespace DirectX;
			XMVECTOR v1 = XMLoadFloat2(&V1);
			XMVECTOR v2 = XMLoadFloat2(&V2);
			XMVECTOR X = XMVectorSubtract(v1, v2);
			Vector2 R;
			XMStoreFloat2(&R, X);
			return R;
		}

		inline Vector2 operator* (const Vector2& V1, const Vector2& V2)
		{
			using namespace DirectX;
			XMVECTOR v1 = XMLoadFloat2(&V1);
			XMVECTOR v2 = XMLoadFloat2(&V2);
			XMVECTOR X = XMVectorMultiply(v1, v2);
			Vector2 R;
			XMStoreFloat2(&R, X);
			return R;
		}

		inline Vector2 operator* (const Vector2& V, float S)
		{
			using namespace DirectX;
			XMVECTOR v1 = XMLoadFloat2(&V);
			XMVECTOR X = XMVectorScale(v1, S);
			Vector2 R;
			XMStoreFloat2(&R, X);
			return R;
		}

		inline Vector2 operator/ (const Vector2& V1, const Vector2& V2)
		{
			using namespace DirectX;
			XMVECTOR v1 = XMLoadFloat2(&V1);
			XMVECTOR v2 = XMLoadFloat2(&V2);
			XMVECTOR X = XMVectorDivide(v1, v2);
			Vector2 R;
			XMStoreFloat2(&R, X);
			return R;
		}

		inline Vector2 operator* (float S, const Vector2& V)
		{
			using namespace DirectX;
			XMVECTOR v1 = XMLoadFloat2(&V);
			XMVECTOR X = XMVectorScale(v1, S);
			Vector2 R;
			XMStoreFloat2(&R, X);
			return R;
		}
	};
};
template<> struct std::less<Utility::Math::Vector2>
{
	bool operator()(const Utility::Math::Vector2& V1, const Utility::Math::Vector2& V2) const
	{
		return ((V1.x < V2.x) || ((V1.x == V2.x) && (V1.y < V2.y)));
	}
};