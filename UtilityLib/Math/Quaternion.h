/**
 *	@file	Quaternion.h
 *	@date	2017 / 9 / 27
 *	@author	Katsumi Takei
 *	@note	http://www.j3d.org/matrix_faq/matrfaq_latest.html#Q56
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <DirectXMath.h>
#include <functional>

namespace Utility
{
	namespace Math
	{
		struct Matrix;
		struct Vector3;
		struct Vector4;

		struct Quaternion : public DirectX::XMFLOAT4
		{
		public:
			Quaternion()
				: XMFLOAT4(0, 0, 0, 1.f)
			{
			}
			Quaternion(float _x, float _y, float _z, float _w)
				: XMFLOAT4(_x, _y, _z, _w)
			{
			}
			Quaternion(const Vector3& v, float scalar);
			explicit Quaternion(const Vector4& v);
			explicit Quaternion(_In_reads_(4) const float *pArray)
				: XMFLOAT4(pArray)
			{
			}
			Quaternion(DirectX::FXMVECTOR V)
			{
				XMStoreFloat4(this, V);
			}
			Quaternion(const XMFLOAT4& q)
			{
				x = q.x;
				y = q.y;
				z = q.z;
				w = q.w;
			}
			explicit Quaternion(const DirectX::XMVECTORF32& F)
			{
				x = F.f[0];
				y = F.f[1];
				z = F.f[2];
				w = F.f[3];
			}

		public:
#pragma region	operator
			operator DirectX::XMVECTOR() const
			{
				return XMLoadFloat4(this);
			}

			inline bool operator == (const Quaternion& q) const
			{
				using namespace DirectX;
				XMVECTOR q1 = XMLoadFloat4(this);
				XMVECTOR q2 = XMLoadFloat4(&q);
				return XMQuaternionEqual(q1, q2);
			}

			inline bool operator != (const Quaternion& q) const
			{
				using namespace DirectX;
				XMVECTOR q1 = XMLoadFloat4(this);
				XMVECTOR q2 = XMLoadFloat4(&q);
				return XMQuaternionNotEqual(q1, q2);
			}

			Quaternion& operator= (const Quaternion& q)
			{
				x = q.x;
				y = q.y;
				z = q.z;
				w = q.w;
				return *this;
			}
			Quaternion& operator= (const XMFLOAT4& q)
			{
				x = q.x;
				y = q.y;
				z = q.z;
				w = q.w;
				return *this;
			}
			Quaternion& operator= (const DirectX::XMVECTORF32& F)
			{
				x = F.f[0];
				y = F.f[1];
				z = F.f[2];
				w = F.f[3];
				return *this;
			}

			inline Quaternion& operator+= (const Quaternion& q)
			{
				using namespace DirectX;
				XMVECTOR q1 = XMLoadFloat4(this);
				XMVECTOR q2 = XMLoadFloat4(&q);
				XMStoreFloat4(this, XMVectorAdd(q1, q2));
				return *this;
			}

			inline Quaternion& operator-= (const Quaternion& q)
			{
				using namespace DirectX;
				XMVECTOR q1 = XMLoadFloat4(this);
				XMVECTOR q2 = XMLoadFloat4(&q);
				XMStoreFloat4(this, XMVectorSubtract(q1, q2));
				return *this;
			}

			inline Quaternion& operator*= (const Quaternion& q)
			{
				using namespace DirectX;
				XMVECTOR q1 = XMLoadFloat4(this);
				XMVECTOR q2 = XMLoadFloat4(&q);
				XMStoreFloat4(this, XMQuaternionMultiply(q1, q2));
				return *this;
			}

			inline Quaternion& operator*= (float S)
			{
				using namespace DirectX;
				XMVECTOR q = XMLoadFloat4(this);
				XMStoreFloat4(this, XMVectorScale(q, S));
				return *this;
			}

			inline Quaternion& operator/= (const Quaternion& q)
			{
				using namespace DirectX;
				XMVECTOR q1 = XMLoadFloat4(this);
				XMVECTOR q2 = XMLoadFloat4(&q);
				q2 = XMQuaternionInverse(q2);
				XMStoreFloat4(this, XMQuaternionMultiply(q1, q2));
				return *this;
			}

			Quaternion operator+ () const
			{
				return *this;
			}

			inline Quaternion operator- () const
			{
				using namespace DirectX;
				XMVECTOR q = XMLoadFloat4(this);

				Quaternion R;
				XMStoreFloat4(&R, XMVectorNegate(q));
				return R;
			}
#pragma endregion	operator
		public:
#pragma region	Funtion

			inline float Length() const
			{
				using namespace DirectX;
				XMVECTOR q = XMLoadFloat4(this);
				return XMVectorGetX(XMQuaternionLength(q));
			}

			inline float LengthSquared() const
			{
				using namespace DirectX;
				XMVECTOR q = XMLoadFloat4(this);
				return XMVectorGetX(XMQuaternionLengthSq(q));
			}

			inline void Normalize()
			{
				using namespace DirectX;
				XMVECTOR q = XMLoadFloat4(this);
				XMStoreFloat4(this, XMQuaternionNormalize(q));
			}

			inline void Normalize(Quaternion& result) const
			{
				using namespace DirectX;
				XMVECTOR q = XMLoadFloat4(this);
				XMStoreFloat4(&result, XMQuaternionNormalize(q));
			}

			inline void Conjugate()
			{
				using namespace DirectX;
				XMVECTOR q = XMLoadFloat4(this);
				XMStoreFloat4(this, XMQuaternionConjugate(q));
			}

			inline void Conjugate(Quaternion& result) const
			{
				using namespace DirectX;
				XMVECTOR q = XMLoadFloat4(this);
				XMStoreFloat4(&result, XMQuaternionConjugate(q));
			}

			inline void Inverse(Quaternion& result) const
			{
				using namespace DirectX;
				XMVECTOR q = XMLoadFloat4(this);
				XMStoreFloat4(&result, XMQuaternionInverse(q));
			}

			inline float Dot(const Quaternion& q) const
			{
				using namespace DirectX;
				XMVECTOR q1 = XMLoadFloat4(this);
				XMVECTOR q2 = XMLoadFloat4(&q);
				return XMVectorGetX(XMQuaternionDot(q1, q2));
			}
#pragma endregion	Funtion

		public:
#pragma region	StaticFuntion

			static Quaternion CreateFromAxisAngle(const Vector3& axis, float angle);

			inline static Quaternion CreateFromYawPitchRoll(float yaw, float pitch, float roll)
			{
				using namespace DirectX;
				Quaternion R;
				XMStoreFloat4(&R, XMQuaternionRotationRollPitchYaw(pitch, yaw, roll));
				return R;
			}
			static Quaternion CreateFromRotationMatrix(const Matrix& M);

			inline static void Lerp(const Quaternion& q1, const Quaternion& q2, float t, Quaternion& result)
			{
				using namespace DirectX;
				XMVECTOR Q0 = XMLoadFloat4(&q1);
				XMVECTOR Q1 = XMLoadFloat4(&q2);

				XMVECTOR dot = XMVector4Dot(Q0, Q1);

				XMVECTOR R;
				if (XMVector4GreaterOrEqual(dot, XMVectorZero()))
				{
					R = XMVectorLerp(Q0, Q1, t);
				}
				else
				{
					XMVECTOR tv = XMVectorReplicate(t);
					XMVECTOR t1v = XMVectorReplicate(1.f - t);
					XMVECTOR X0 = XMVectorMultiply(Q0, t1v);
					XMVECTOR X1 = XMVectorMultiply(Q1, tv);
					R = XMVectorSubtract(X0, X1);
				}

				XMStoreFloat4(&result, XMQuaternionNormalize(R));
			}

			inline static Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, float t)
			{
				using namespace DirectX;
				XMVECTOR Q0 = XMLoadFloat4(&q1);
				XMVECTOR Q1 = XMLoadFloat4(&q2);

				XMVECTOR dot = XMVector4Dot(Q0, Q1);

				XMVECTOR R;
				if (XMVector4GreaterOrEqual(dot, XMVectorZero()))
				{
					R = XMVectorLerp(Q0, Q1, t);
				}
				else
				{
					XMVECTOR tv = XMVectorReplicate(t);
					XMVECTOR t1v = XMVectorReplicate(1.f - t);
					XMVECTOR X0 = XMVectorMultiply(Q0, t1v);
					XMVECTOR X1 = XMVectorMultiply(Q1, tv);
					R = XMVectorSubtract(X0, X1);
				}

				Quaternion result;
				XMStoreFloat4(&result, XMQuaternionNormalize(R));
				return result;
			}

			inline static void Slerp(const Quaternion& q1, const Quaternion& q2, float t, Quaternion& result)
			{
				using namespace DirectX;
				XMVECTOR Q0 = XMLoadFloat4(&q1);
				XMVECTOR Q1 = XMLoadFloat4(&q2);
				XMStoreFloat4(&result, XMQuaternionSlerp(Q0, Q1, t));
			}

			inline Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t)
			{
				using namespace DirectX;
				XMVECTOR Q0 = XMLoadFloat4(&q1);
				XMVECTOR Q1 = XMLoadFloat4(&q2);

				Quaternion result;
				XMStoreFloat4(&result, XMQuaternionSlerp(Q0, Q1, t));
				return result;
			}

			inline static void Concatenate(const Quaternion& q1, const Quaternion& q2, Quaternion& result)
			{
				using namespace DirectX;
				XMVECTOR Q0 = XMLoadFloat4(&q1);
				XMVECTOR Q1 = XMLoadFloat4(&q2);
				XMStoreFloat4(&result, XMQuaternionMultiply(Q1, Q0));
			}

			inline static Quaternion Concatenate(const Quaternion& q1, const Quaternion& q2)
			{
				using namespace DirectX;
				XMVECTOR Q0 = XMLoadFloat4(&q1);
				XMVECTOR Q1 = XMLoadFloat4(&q2);

				Quaternion result;
				XMStoreFloat4(&result, XMQuaternionMultiply(Q1, Q0));
				return result;
			}

			inline static void Interpolate(Quaternion *result, const Quaternion& start, const Quaternion& end, float factor)
			{
				// calc cosine theta
				float cosom = start.x * end.x + start.y * end.y
					+ start.z * end.z + start.w * end.w;

				// adjust signs (if necessary)
				Quaternion newEnd = end;
				if (cosom < 0.0f)
				{
					cosom = -cosom;
					newEnd.x = -newEnd.x;
					newEnd.y = -newEnd.y;
					newEnd.z = -newEnd.z;
					newEnd.w = -newEnd.w;
				}

				// Calculate coefficients
				float sclp, sclq;
				if ((1.0f - cosom) > 0.0001f) // 0.0001 -> some epsillon
				{
					// Standard case (slerp)
					float omega, sinom;
					omega = std::acos(cosom);
					sinom = std::sin(omega);
					sclp = std::sin((1.0f - factor) * omega) / sinom;
					sclq = std::sin(factor * omega) / sinom;
				}
				else
				{
					// Very close, do linear interp (because it's faster)
					sclp = 1.0f - factor;
					sclq = factor;
				}

				result->x = sclp * start.x + sclq * newEnd.x;
				result->y = sclp * start.y + sclq * newEnd.y;
				result->z = sclp * start.z + sclq * newEnd.z;
				result->w = sclp * start.w + sclq * newEnd.w;
			}
#pragma endregion	StaticFuntion

			static const Quaternion Identity;
		};

		inline Quaternion operator+ (const Quaternion& Q1, const Quaternion& Q2)
		{
			using namespace DirectX;
			XMVECTOR q1 = XMLoadFloat4(&Q1);
			XMVECTOR q2 = XMLoadFloat4(&Q2);

			Quaternion R;
			XMStoreFloat4(&R, XMVectorAdd(q1, q2));
			return R;
		}

		inline Quaternion operator- (const Quaternion& Q1, const Quaternion& Q2)
		{
			using namespace DirectX;
			XMVECTOR q1 = XMLoadFloat4(&Q1);
			XMVECTOR q2 = XMLoadFloat4(&Q2);

			Quaternion R;
			XMStoreFloat4(&R, XMVectorSubtract(q1, q2));
			return R;
		}

		inline Quaternion operator* (const Quaternion& Q1, const Quaternion& Q2)
		{
			using namespace DirectX;
			XMVECTOR q1 = XMLoadFloat4(&Q1);
			XMVECTOR q2 = XMLoadFloat4(&Q2);

			Quaternion R;
			XMStoreFloat4(&R, XMQuaternionMultiply(q1, q2));
			return R;
		}

		inline Quaternion operator* (const Quaternion& Q, float S)
		{
			using namespace DirectX;
			XMVECTOR q = XMLoadFloat4(&Q);

			Quaternion R;
			XMStoreFloat4(&R, XMVectorScale(q, S));
			return R;
		}

		inline Quaternion operator/ (const Quaternion& Q1, const Quaternion& Q2)
		{
			using namespace DirectX;
			XMVECTOR q1 = XMLoadFloat4(&Q1);
			XMVECTOR q2 = XMLoadFloat4(&Q2);
			q2 = XMQuaternionInverse(q2);

			Quaternion R;
			XMStoreFloat4(&R, XMQuaternionMultiply(q1, q2));
			return R;
		}

		inline Quaternion operator* (float S, const Quaternion& Q)
		{
			using namespace DirectX;
			XMVECTOR q1 = XMLoadFloat4(&Q);

			Quaternion R;
			XMStoreFloat4(&R, XMVectorScale(q1, S));
			return R;
		}
	};
};

template<> struct std::less<Utility::Math::Quaternion>
{
	bool operator()(const Utility::Math::Quaternion& Q1, const Utility::Math::Quaternion& Q2) const
	{
		return ((Q1.x < Q2.x)
			|| ((Q1.x == Q2.x) && (Q1.y < Q2.y))
			|| ((Q1.x == Q2.x) && (Q1.y == Q2.y) && (Q1.z < Q2.z))
			|| ((Q1.x == Q2.x) && (Q1.y == Q2.y) && (Q1.z == Q2.z) && (Q1.w < Q2.w)));
	}
};