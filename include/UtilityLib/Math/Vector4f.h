/**
 *	@file	Vector4.h
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
		struct Vector2;
		struct Vector3;

		struct Vector4 : public DirectX::XMFLOAT4
		{
		public:
			Vector4() : XMFLOAT4(0.f, 0.f, 0.f, 0.f) {}
			explicit Vector4(float x) : XMFLOAT4(x, x, x, x) {}
			Vector4(float _x, float _y, float _z, float _w) : XMFLOAT4(_x, _y, _z, _w) {}
			explicit Vector4(_In_reads_(4) const float *pArray) : XMFLOAT4(pArray) {}
			Vector4(DirectX::FXMVECTOR V) { XMStoreFloat4(this, V); }
			Vector4(const XMFLOAT4& V) { this->x = V.x; this->y = V.y; this->z = V.z; this->w = V.w; }
			explicit Vector4(const DirectX::XMVECTORF32& F) { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; this->w = F.f[3]; }

		public:
#pragma region	Operator
			operator DirectX::XMVECTOR() const { return XMLoadFloat4(this); }

			inline bool operator == (const Vector4& V) const
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat4(this);
				XMVECTOR v2 = XMLoadFloat4(&V);
				return XMVector4Equal(v1, v2);
			}

			inline bool operator != (const Vector4& V) const
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat4(this);
				XMVECTOR v2 = XMLoadFloat4(&V);
				return XMVector4NotEqual(v1, v2);
			}

			// Assignment operators
			Vector4& operator= (const Vector4& V) { x = V.x; y = V.y; z = V.z; w = V.w; return *this; }
			Vector4& operator= (const XMFLOAT4& V) { x = V.x; y = V.y; z = V.z; w = V.w; return *this; }
			Vector4& operator= (const DirectX::XMVECTORF32& F) { x = F.f[0]; y = F.f[1]; z = F.f[2]; w = F.f[3]; return *this; }

			inline Vector4& operator+= (const Vector4& V)
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat4(this);
				XMVECTOR v2 = XMLoadFloat4(&V);
				XMVECTOR X = XMVectorAdd(v1, v2);
				XMStoreFloat4(this, X);
				return *this;
			}

			inline Vector4& operator-= (const Vector4& V)
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat4(this);
				XMVECTOR v2 = XMLoadFloat4(&V);
				XMVECTOR X = XMVectorSubtract(v1, v2);
				XMStoreFloat4(this, X);
				return *this;
			}

			inline Vector4& operator*= (const Vector4& V)
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat4(this);
				XMVECTOR v2 = XMLoadFloat4(&V);
				XMVECTOR X = XMVectorMultiply(v1, v2);
				XMStoreFloat4(this, X);
				return *this;
			}

			inline Vector4& operator*= (float S)
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat4(this);
				XMVECTOR X = XMVectorScale(v1, S);
				XMStoreFloat4(this, X);
				return *this;
			}

			inline Vector4& operator/= (float S)
			{
				using namespace DirectX;
				assert(S != 0.0f);
				XMVECTOR v1 = XMLoadFloat4(this);
				XMVECTOR X = XMVectorScale(v1, 1.f / S);
				XMStoreFloat4(this, X);
				return *this;
			}
			Vector4 operator+ () const { return *this; }
			inline Vector4 operator- () const
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat4(this);
				XMVECTOR X = XMVectorNegate(v1);
				Vector4 R;
				XMStoreFloat4(&R, X);
				return R;
			}
#pragma endregion	Operator

		public:
#pragma region	Function
			inline bool InBounds(const Vector4& Bounds) const
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat4(this);
				XMVECTOR v2 = XMLoadFloat4(&Bounds);
				return XMVector4InBounds(v1, v2);
			}

			inline float Length() const
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat4(this);
				XMVECTOR X = XMVector4Length(v1);
				return XMVectorGetX(X);
			}

			inline float LengthSquared() const
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat4(this);
				XMVECTOR X = XMVector4LengthSq(v1);
				return XMVectorGetX(X);
			}

			inline float Dot(const Vector4& V) const
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat4(this);
				XMVECTOR v2 = XMLoadFloat4(&V);
				XMVECTOR X = XMVector4Dot(v1, v2);
				return XMVectorGetX(X);
			}

			inline void Cross(const Vector4& v1, const Vector4& v2, Vector4& result) const
			{
				using namespace DirectX;
				XMVECTOR x1 = XMLoadFloat4(this);
				XMVECTOR x2 = XMLoadFloat4(&v1);
				XMVECTOR x3 = XMLoadFloat4(&v2);
				XMVECTOR R = XMVector4Cross(x1, x2, x3);
				XMStoreFloat4(&result, R);
			}

			inline Vector4 Cross(const Vector4& v1, const Vector4& v2) const
			{
				using namespace DirectX;
				XMVECTOR x1 = XMLoadFloat4(this);
				XMVECTOR x2 = XMLoadFloat4(&v1);
				XMVECTOR x3 = XMLoadFloat4(&v2);
				XMVECTOR R = XMVector4Cross(x1, x2, x3);

				Vector4 result;
				XMStoreFloat4(&result, R);
				return result;
			}

			inline void Normalize()
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat4(this);
				XMVECTOR X = XMVector4Normalize(v1);
				XMStoreFloat4(this, X);
			}

			inline void Normalize(Vector4& result) const
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat4(this);
				XMVECTOR X = XMVector4Normalize(v1);
				XMStoreFloat4(&result, X);
			}

#pragma endregion	Function
#pragma region	StaticFunction

			inline static float Distance(const Vector4& v1, const Vector4& v2)
			{
				using namespace DirectX;
				XMVECTOR x1 = XMLoadFloat4(&v1);
				XMVECTOR x2 = XMLoadFloat4(&v2);
				XMVECTOR V = XMVectorSubtract(x2, x1);
				XMVECTOR X = XMVector4Length(V);
				return XMVectorGetX(X);
			}

			inline static float DistanceSquared(const Vector4& v1, const Vector4& v2)
			{
				using namespace DirectX;
				XMVECTOR x1 = XMLoadFloat4(&v1);
				XMVECTOR x2 = XMLoadFloat4(&v2);
				XMVECTOR V = XMVectorSubtract(x2, x1);
				XMVECTOR X = XMVector4LengthSq(V);
				return XMVectorGetX(X);
			}

			inline static void Min(const Vector4& v1, const Vector4& v2, Vector4& result)
			{
				using namespace DirectX;
				XMVECTOR x1 = XMLoadFloat4(&v1);
				XMVECTOR x2 = XMLoadFloat4(&v2);
				XMVECTOR X = XMVectorMin(x1, x2);
				XMStoreFloat4(&result, X);
			}

			inline static Vector4 Min(const Vector4& v1, const Vector4& v2)
			{
				using namespace DirectX;
				XMVECTOR x1 = XMLoadFloat4(&v1);
				XMVECTOR x2 = XMLoadFloat4(&v2);
				XMVECTOR X = XMVectorMin(x1, x2);

				Vector4 result;
				XMStoreFloat4(&result, X);
				return result;
			}

			inline static void Max(const Vector4& v1, const Vector4& v2, Vector4& result)
			{
				using namespace DirectX;
				XMVECTOR x1 = XMLoadFloat4(&v1);
				XMVECTOR x2 = XMLoadFloat4(&v2);
				XMVECTOR X = XMVectorMax(x1, x2);
				XMStoreFloat4(&result, X);
			}

			inline static Vector4 Max(const Vector4& v1, const Vector4& v2)
			{
				using namespace DirectX;
				XMVECTOR x1 = XMLoadFloat4(&v1);
				XMVECTOR x2 = XMLoadFloat4(&v2);
				XMVECTOR X = XMVectorMax(x1, x2);

				Vector4 result;
				XMStoreFloat4(&result, X);
				return result;
			}

			inline static void Lerp(const Vector4& v1, const Vector4& v2, float t, Vector4& result)
			{
				using namespace DirectX;
				XMVECTOR x1 = XMLoadFloat4(&v1);
				XMVECTOR x2 = XMLoadFloat4(&v2);
				XMVECTOR X = XMVectorLerp(x1, x2, t);
				XMStoreFloat4(&result, X);
			}

			inline static Vector4 Lerp(const Vector4& v1, const Vector4& v2, float t)
			{
				using namespace DirectX;
				XMVECTOR x1 = XMLoadFloat4(&v1);
				XMVECTOR x2 = XMLoadFloat4(&v2);
				XMVECTOR X = XMVectorLerp(x1, x2, t);

				Vector4 result;
				XMStoreFloat4(&result, X);
				return result;
			}


			void static Transform(const Vector2& v, const Quaternion& quat, Vector4& result);
			Vector4 static Transform(const Vector2& v, const Quaternion& quat);
			void static Transform(const Vector3& v, const Quaternion& quat, Vector4& result);
			Vector4 static Transform(const Vector3& v, const Quaternion& quat);
			void static Transform(const Vector4& v, const Quaternion& quat, Vector4& result);
			Vector4 static Transform(const Vector4& v, const Quaternion& quat);
			void static Transform(const Vector4& v, const Matrix& m, Vector4& result);
			Vector4 static Transform(const Vector4& v, const Matrix& m);
			void static Transform(const Vector4* varray, size_t count, const Matrix& m, Vector4* resultArray);
#pragma endregion	StaticFunction
			static const Vector4 Zero;
		};

		inline Vector4 operator+ (const Vector4& V1, const Vector4& V2)
		{
			using namespace DirectX;
			XMVECTOR v1 = XMLoadFloat4(&V1);
			XMVECTOR v2 = XMLoadFloat4(&V2);
			XMVECTOR X = XMVectorAdd(v1, v2);
			Vector4 R;
			XMStoreFloat4(&R, X);
			return R;
		}

		inline Vector4 operator- (const Vector4& V1, const Vector4& V2)
		{
			using namespace DirectX;
			XMVECTOR v1 = XMLoadFloat4(&V1);
			XMVECTOR v2 = XMLoadFloat4(&V2);
			XMVECTOR X = XMVectorSubtract(v1, v2);
			Vector4 R;
			XMStoreFloat4(&R, X);
			return R;
		}

		inline Vector4 operator* (const Vector4& V1, const Vector4& V2)
		{
			using namespace DirectX;
			XMVECTOR v1 = XMLoadFloat4(&V1);
			XMVECTOR v2 = XMLoadFloat4(&V2);
			XMVECTOR X = XMVectorMultiply(v1, v2);
			Vector4 R;
			XMStoreFloat4(&R, X);
			return R;
		}

		inline Vector4 operator* (const Vector4& V, float S)
		{
			using namespace DirectX;
			XMVECTOR v1 = XMLoadFloat4(&V);
			XMVECTOR X = XMVectorScale(v1, S);
			Vector4 R;
			XMStoreFloat4(&R, X);
			return R;
		}

		inline Vector4 operator/ (const Vector4& V1, const Vector4& V2)
		{
			using namespace DirectX;
			XMVECTOR v1 = XMLoadFloat4(&V1);
			XMVECTOR v2 = XMLoadFloat4(&V2);
			XMVECTOR X = XMVectorDivide(v1, v2);
			Vector4 R;
			XMStoreFloat4(&R, X);
			return R;
		}

		inline Vector4 operator* (float S, const Vector4& V)
		{
			using namespace DirectX;
			XMVECTOR v1 = XMLoadFloat4(&V);
			XMVECTOR X = XMVectorScale(v1, S);
			Vector4 R;
			XMStoreFloat4(&R, X);
			return R;
		}
	};
};

template<> struct std::less<Utility::Math::Vector4>
{
	bool operator()(const Utility::Math::Vector4& V1, const Utility::Math::Vector4& V2) const
	{
		return ((V1.x < V2.x)
			|| ((V1.x == V2.x) && (V1.y < V2.y))
			|| ((V1.x == V2.x) && (V1.y == V2.y) && (V1.z < V2.z))
			|| ((V1.x == V2.x) && (V1.y == V2.y) && (V1.z == V2.z) && (V1.w < V2.w)));
	}
};