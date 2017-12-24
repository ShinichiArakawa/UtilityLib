/**
 *	@file	Vector3.h
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

		struct Vector3 : public DirectX::XMFLOAT3
		{
		public:
			Vector3() : XMFLOAT3(0.f, 0.f, 0.f) {}
			explicit Vector3(float x) : XMFLOAT3(x, x, x) {}
			Vector3(float _x, float _y, float _z) : XMFLOAT3(_x, _y, _z) {}
			explicit Vector3(_In_reads_(3) const float *pArray) : XMFLOAT3(pArray) {}
			Vector3(DirectX::FXMVECTOR V) { XMStoreFloat3(this, V); }
			Vector3(const XMFLOAT3& V) { this->x = V.x; this->y = V.y; this->z = V.z; }
			explicit Vector3(const DirectX::XMVECTORF32& F) { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; }

		public:
			operator DirectX::XMVECTOR() const { return XMLoadFloat3(this); }


			inline bool operator == (const Vector3& V) const
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat3(this);
				XMVECTOR v2 = XMLoadFloat3(&V);
				return XMVector3Equal(v1, v2);
			}

			inline bool operator != (const Vector3& V) const
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat3(this);
				XMVECTOR v2 = XMLoadFloat3(&V);
				return XMVector3NotEqual(v1, v2);
			}

			// Assignment operators
			Vector3& operator= (const Vector3& V) { x = V.x; y = V.y; z = V.z; return *this; }
			Vector3& operator= (const XMFLOAT3& V) { x = V.x; y = V.y; z = V.z; return *this; }
			Vector3& operator= (const DirectX::XMVECTORF32& F) { x = F.f[0]; y = F.f[1]; z = F.f[2]; return *this; }

			inline Vector3& operator+= (const Vector3& V)
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat3(this);
				XMVECTOR v2 = XMLoadFloat3(&V);
				XMVECTOR X = XMVectorAdd(v1, v2);
				XMStoreFloat3(this, X);
				return *this;
			}

			inline Vector3& operator-= (const Vector3& V)
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat3(this);
				XMVECTOR v2 = XMLoadFloat3(&V);
				XMVECTOR X = XMVectorSubtract(v1, v2);
				XMStoreFloat3(this, X);
				return *this;
			}

			inline Vector3& operator*= (const Vector3& V)
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat3(this);
				XMVECTOR v2 = XMLoadFloat3(&V);
				XMVECTOR X = XMVectorMultiply(v1, v2);
				XMStoreFloat3(this, X);
				return *this;
			}

			inline Vector3& operator*= (float S)
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat3(this);
				XMVECTOR X = XMVectorScale(v1, S);
				XMStoreFloat3(this, X);
				return *this;
			}

			inline Vector3& operator/= (float S)
			{
				using namespace DirectX;
				assert(S != 0.0f);
				XMVECTOR v1 = XMLoadFloat3(this);
				XMVECTOR X = XMVectorScale(v1, 1.f / S);
				XMStoreFloat3(this, X);
				return *this;
			}

			// Unary operators
			Vector3 operator+ () const { return *this; }

			inline Vector3 operator- () const
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat3(this);
				XMVECTOR X = XMVectorNegate(v1);
				Vector3 R;
				XMStoreFloat3(&R, X);
				return R;
			}

			inline float operator[](unsigned int i) const
			{
				assert(i >= 0 && i <= 2);
				return (&x)[i];
			}
			inline float& operator[](unsigned int i)
			{
				assert(i >= 0 && i <= 2);
				return (&x)[i];
			}

		public:

			inline bool InBounds(const Vector3& Bounds) const
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat3(this);
				XMVECTOR v2 = XMLoadFloat3(&Bounds);
				return XMVector3InBounds(v1, v2);
			}

			inline float Length() const
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat3(this);
				XMVECTOR X = XMVector3Length(v1);
				return XMVectorGetX(X);
			}

			inline float LengthSquared() const
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat3(this);
				XMVECTOR X = XMVector3LengthSq(v1);
				return XMVectorGetX(X);
			}

			inline float Dot(const Vector3& V) const
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat3(this);
				XMVECTOR v2 = XMLoadFloat3(&V);
				XMVECTOR X = XMVector3Dot(v1, v2);
				return XMVectorGetX(X);
			}

			inline void Cross(const Vector3& V, Vector3& result) const
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat3(this);
				XMVECTOR v2 = XMLoadFloat3(&V);
				XMVECTOR R = XMVector3Cross(v1, v2);
				XMStoreFloat3(&result, R);
			}

			inline Vector3 Cross(const Vector3& V) const
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat3(this);
				XMVECTOR v2 = XMLoadFloat3(&V);
				XMVECTOR R = XMVector3Cross(v1, v2);

				Vector3 result;
				XMStoreFloat3(&result, R);
				return result;
			}

			inline void Normalize()
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat3(this);
				XMVECTOR X = XMVector3Normalize(v1);
				XMStoreFloat3(this, X);
			}

			inline void Normalize(Vector3& result) const
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat3(this);
				XMVECTOR X = XMVector3Normalize(v1);
				XMStoreFloat3(&result, X);
			}


			inline static float Distance(const Vector3& v1, const Vector3& v2)
			{
				using namespace DirectX;
				XMVECTOR x1 = XMLoadFloat3(&v1);
				XMVECTOR x2 = XMLoadFloat3(&v2);
				XMVECTOR V = XMVectorSubtract(x2, x1);
				XMVECTOR X = XMVector3Length(V);
				return XMVectorGetX(X);
			}

			inline static float DistanceSquared(const Vector3& v1, const Vector3& v2)
			{
				using namespace DirectX;
				XMVECTOR x1 = XMLoadFloat3(&v1);
				XMVECTOR x2 = XMLoadFloat3(&v2);
				XMVECTOR V = XMVectorSubtract(x2, x1);
				XMVECTOR X = XMVector3LengthSq(V);
				return XMVectorGetX(X);
			}

			inline static void Min(const Vector3& v1, const Vector3& v2, Vector3& result)
			{
				using namespace DirectX;
				XMVECTOR x1 = XMLoadFloat3(&v1);
				XMVECTOR x2 = XMLoadFloat3(&v2);
				XMVECTOR X = XMVectorMin(x1, x2);
				XMStoreFloat3(&result, X);
			}

			inline static Vector3 Min(const Vector3& v1, const Vector3& v2)
			{
				using namespace DirectX;
				XMVECTOR x1 = XMLoadFloat3(&v1);
				XMVECTOR x2 = XMLoadFloat3(&v2);
				XMVECTOR X = XMVectorMin(x1, x2);

				Vector3 result;
				XMStoreFloat3(&result, X);
				return result;
			}

			inline static void Max(const Vector3& v1, const Vector3& v2, Vector3& result)
			{
				using namespace DirectX;
				XMVECTOR x1 = XMLoadFloat3(&v1);
				XMVECTOR x2 = XMLoadFloat3(&v2);
				XMVECTOR X = XMVectorMax(x1, x2);
				XMStoreFloat3(&result, X);
			}

			inline static Vector3 Max(const Vector3& v1, const Vector3& v2)
			{
				using namespace DirectX;
				XMVECTOR x1 = XMLoadFloat3(&v1);
				XMVECTOR x2 = XMLoadFloat3(&v2);
				XMVECTOR X = XMVectorMax(x1, x2);

				Vector3 result;
				XMStoreFloat3(&result, X);
				return result;
			}

			inline static void Lerp(const Vector3& v1, const Vector3& v2, float t, Vector3& result)
			{
				using namespace DirectX;
				XMVECTOR x1 = XMLoadFloat3(&v1);
				XMVECTOR x2 = XMLoadFloat3(&v2);
				XMVECTOR X = XMVectorLerp(x1, x2, t);
				XMStoreFloat3(&result, X);
			}

			inline static Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t)
			{
				using namespace DirectX;
				XMVECTOR x1 = XMLoadFloat3(&v1);
				XMVECTOR x2 = XMLoadFloat3(&v2);
				XMVECTOR X = XMVectorLerp(x1, x2, t);

				Vector3 result;
				XMStoreFloat3(&result, X);
				return result;
			}

			static void Transform(const Vector3& v, const Quaternion& quat, Vector3& result);
			static Vector3 Transform(const Vector3& v, const Quaternion& quat);
			static void Transform(const Vector3& v, const Matrix& m, Vector3& result);
			static Vector3 Transform(const Vector3& v, const Matrix& m);
			static void Transform(const Vector3* varray, size_t count, const Matrix& m, Vector3* resultArray);
			static void Transform(const Vector3& v, const Matrix& m, Vector4& result);
			static void Transform(const Vector3* varray, size_t count, const Matrix& m, Vector4* resultArray);
			static void TransformNormal(const Vector3& v, const Matrix& m, Vector3& result);
			static Vector3 TransformNormal(const Vector3& v, const Matrix& m);
			static void TransformNormal(const Vector3* varray, size_t count, const Matrix& m, Vector3* resultArray);

			static Vector3 MultiplyMatrix(const Vector3 &argVector3, const Matrix &argMatrix);

			static const Vector3 Zero;

		};


		inline Vector3 operator+ (const Vector3& V1, const Vector3& V2)
		{
			using namespace DirectX;
			XMVECTOR v1 = XMLoadFloat3(&V1);
			XMVECTOR v2 = XMLoadFloat3(&V2);
			XMVECTOR X = XMVectorAdd(v1, v2);
			Vector3 R;
			XMStoreFloat3(&R, X);
			return R;
		}

		inline Vector3 operator- (const Vector3& V1, const Vector3& V2)
		{
			using namespace DirectX;
			XMVECTOR v1 = XMLoadFloat3(&V1);
			XMVECTOR v2 = XMLoadFloat3(&V2);
			XMVECTOR X = XMVectorSubtract(v1, v2);
			Vector3 R;
			XMStoreFloat3(&R, X);
			return R;
		}

		inline Vector3 operator* (const Vector3& V1, const Vector3& V2)
		{
			using namespace DirectX;
			XMVECTOR v1 = XMLoadFloat3(&V1);
			XMVECTOR v2 = XMLoadFloat3(&V2);
			XMVECTOR X = XMVectorMultiply(v1, v2);
			Vector3 R;
			XMStoreFloat3(&R, X);
			return R;
		}

		inline Vector3 operator* (const Vector3& V, float S)
		{
			using namespace DirectX;
			XMVECTOR v1 = XMLoadFloat3(&V);
			XMVECTOR X = XMVectorScale(v1, S);
			Vector3 R;
			XMStoreFloat3(&R, X);
			return R;
		}

		inline Vector3 operator/ (const Vector3& V1, const Vector3& V2)
		{
			using namespace DirectX;
			XMVECTOR v1 = XMLoadFloat3(&V1);
			XMVECTOR v2 = XMLoadFloat3(&V2);
			XMVECTOR X = XMVectorDivide(v1, v2);
			Vector3 R;
			XMStoreFloat3(&R, X);
			return R;
		}

		inline Vector3 operator* (float S, const Vector3& V)
		{
			using namespace DirectX;
			XMVECTOR v1 = XMLoadFloat3(&V);
			XMVECTOR X = XMVectorScale(v1, S);
			Vector3 R;
			XMStoreFloat3(&R, X);
			return R;
		}
	};
};

template<> struct std::less<Utility::Math::Vector3>
{
	bool operator()(const Utility::Math::Vector3& V1, const Utility::Math::Vector3& V2) const
	{
		return ((V1.x < V2.x)
			|| ((V1.x == V2.x) && (V1.y < V2.y))
			|| ((V1.x == V2.x) && (V1.y == V2.y) && (V1.z < V2.z)));
	}
};