/**
 *	@file	Matrix.h
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
		struct Vector2;
		struct Vector3;
		struct Vector4;

		struct Matrix : public DirectX::XMFLOAT4X4
		{
#pragma region		Constructor
			Matrix()
				: XMFLOAT4X4(
					1.f, 0, 0, 0,
					0, 1.f, 0, 0,
					0, 0, 1.f, 0,
					0, 0, 0, 1.f)
			{
			}
			Matrix(float m00, float m01, float m02, float m03,
				float m10, float m11, float m12, float m13,
				float m20, float m21, float m22, float m23,
				float m30, float m31, float m32, float m33)
				: XMFLOAT4X4(
					m00, m01, m02, m03,
					m10, m11, m12, m13,
					m20, m21, m22, m23,
					m30, m31, m32, m33)
			{
			}
			explicit Matrix(const Vector3& r0, const Vector3& r1, const Vector3& r2);
			explicit Matrix(const Vector4& r0, const Vector4& r1, const Vector4& r2, const Vector4& r3);
			Matrix(const XMFLOAT4X4& M)
			{
				memcpy_s(this, sizeof(float) * 16, &M, sizeof(XMFLOAT4X4));
			}
			inline Matrix::Matrix(const DirectX::XMFLOAT3X3& M)
			{
				_11 = M._11; _12 = M._12; _13 = M._13; _14 = 0.f;
				_21 = M._21; _22 = M._22; _23 = M._23; _24 = 0.f;
				_31 = M._31; _32 = M._32; _33 = M._33; _34 = 0.f;
				_41 = 0.f;   _42 = 0.f;   _43 = 0.f;   _44 = 1.f;
			}

			inline Matrix::Matrix(const DirectX::XMFLOAT4X3& M)
			{
				_11 = M._11; _12 = M._12; _13 = M._13; _14 = 0.f;
				_21 = M._21; _22 = M._22; _23 = M._23; _24 = 0.f;
				_31 = M._31; _32 = M._32; _33 = M._33; _34 = 0.f;
				_41 = M._41; _42 = M._42; _43 = M._43; _44 = 1.f;
			}

			explicit Matrix(_In_reads_(16) const float *pArray) : XMFLOAT4X4(pArray) {}
			Matrix(DirectX::CXMMATRIX M) { XMStoreFloat4x4(this, M); }

			explicit Matrix(const double argOther[16])
			{
				m[0][0] = static_cast<float>(argOther[0]);
				m[0][1] = static_cast<float>(argOther[1]);
				m[0][2] = static_cast<float>(argOther[2]);
				m[0][3] = static_cast<float>(argOther[3]);

				m[1][0] = static_cast<float>(argOther[4]);
				m[1][1] = static_cast<float>(argOther[5]);
				m[1][2] = static_cast<float>(argOther[6]);
				m[1][3] = static_cast<float>(argOther[7]);

				m[2][0] = static_cast<float>(argOther[8]);
				m[2][1] = static_cast<float>(argOther[9]);
				m[2][2] = static_cast<float>(argOther[10]);
				m[2][3] = static_cast<float>(argOther[11]);

				m[3][0] = static_cast<float>(argOther[12]);
				m[3][1] = static_cast<float>(argOther[13]);
				m[3][2] = static_cast<float>(argOther[14]);
				m[3][3] = static_cast<float>(argOther[15]);
			}
#pragma endregion		Constructor
#pragma region		Operator
		public:
			operator DirectX::XMMATRIX() const { return XMLoadFloat4x4(this); }

			inline bool operator == (const Matrix& M) const
			{
				using namespace DirectX;
				XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
				XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
				XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
				XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

				XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
				XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
				XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
				XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

				return (XMVector4Equal(x1, y1)
					&& XMVector4Equal(x2, y2)
					&& XMVector4Equal(x3, y3)
					&& XMVector4Equal(x4, y4)) != 0;
			}

			inline bool operator != (const Matrix& M) const
			{
				using namespace DirectX;
				XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
				XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
				XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
				XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

				XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
				XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
				XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
				XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

				return (XMVector4NotEqual(x1, y1)
					|| XMVector4NotEqual(x2, y2)
					|| XMVector4NotEqual(x3, y3)
					|| XMVector4NotEqual(x4, y4)) != 0;
			}

			// Assignment operators
			Matrix& operator= (const Matrix& M) { memcpy_s(this, sizeof(float) * 16, &M, sizeof(float) * 16); return *this; }
			Matrix& operator= (const XMFLOAT4X4& M) { memcpy_s(this, sizeof(float) * 16, &M, sizeof(XMFLOAT4X4)); return *this; }

			inline Matrix& operator= (const DirectX::XMFLOAT3X3& M)
			{
				_11 = M._11; _12 = M._12; _13 = M._13; _14 = 0.f;
				_21 = M._21; _22 = M._22; _23 = M._23; _24 = 0.f;
				_31 = M._31; _32 = M._32; _33 = M._33; _34 = 0.f;
				_41 = 0.f;   _42 = 0.f;   _43 = 0.f;   _44 = 1.f;
				return *this;
			}

			inline Matrix& operator= (const DirectX::XMFLOAT4X3& M)
			{
				_11 = M._11; _12 = M._12; _13 = M._13; _14 = 0.f;
				_21 = M._21; _22 = M._22; _23 = M._23; _24 = 0.f;
				_31 = M._31; _32 = M._32; _33 = M._33; _34 = 0.f;
				_41 = M._41; _42 = M._42; _43 = M._43; _44 = 1.f;
				return *this;
			}

			inline Matrix& operator+= (const Matrix& M)
			{
				using namespace DirectX;
				XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
				XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
				XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
				XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

				XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
				XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
				XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
				XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

				x1 = XMVectorAdd(x1, y1);
				x2 = XMVectorAdd(x2, y2);
				x3 = XMVectorAdd(x3, y3);
				x4 = XMVectorAdd(x4, y4);

				XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
				XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
				XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
				XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);
				return *this;
			}

			inline Matrix& operator-= (const Matrix& M)
			{
				using namespace DirectX;
				XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
				XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
				XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
				XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

				XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
				XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
				XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
				XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

				x1 = XMVectorSubtract(x1, y1);
				x2 = XMVectorSubtract(x2, y2);
				x3 = XMVectorSubtract(x3, y3);
				x4 = XMVectorSubtract(x4, y4);

				XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
				XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
				XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
				XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);
				return *this;
			}

			inline Matrix& operator*= (const Matrix& M)
			{
				using namespace DirectX;
				XMMATRIX M1 = XMLoadFloat4x4(this);
				XMMATRIX M2 = XMLoadFloat4x4(&M);
				XMMATRIX X = XMMatrixMultiply(M1, M2);
				XMStoreFloat4x4(this, X);
				return *this;
			}

			inline Matrix& operator*= (float S)
			{
				using namespace DirectX;
				XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
				XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
				XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
				XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

				x1 = XMVectorScale(x1, S);
				x2 = XMVectorScale(x2, S);
				x3 = XMVectorScale(x3, S);
				x4 = XMVectorScale(x4, S);

				XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
				XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
				XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
				XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);
				return *this;
			}

			inline Matrix& operator/= (float S)
			{
				using namespace DirectX;
				assert(S != 0.f);
				XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
				XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
				XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
				XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

				float rs = 1.f / S;

				x1 = XMVectorScale(x1, rs);
				x2 = XMVectorScale(x2, rs);
				x3 = XMVectorScale(x3, rs);
				x4 = XMVectorScale(x4, rs);

				XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
				XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
				XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
				XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);
				return *this;
			}

			inline Matrix& operator/= (const Matrix& M)
			{
				using namespace DirectX;
				XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
				XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
				XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
				XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

				XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
				XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
				XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
				XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

				x1 = XMVectorDivide(x1, y1);
				x2 = XMVectorDivide(x2, y2);
				x3 = XMVectorDivide(x3, y3);
				x4 = XMVectorDivide(x4, y4);

				XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
				XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
				XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
				XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);
				return *this;
			}
			// Element-wise divide

			// Unary operators
			Matrix operator+ () const { return *this; }

			inline Matrix operator- () const
			{
				using namespace DirectX;
				XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
				XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
				XMVECTOR v3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
				XMVECTOR v4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

				v1 = XMVectorNegate(v1);
				v2 = XMVectorNegate(v2);
				v3 = XMVectorNegate(v3);
				v4 = XMVectorNegate(v4);

				Matrix R;
				XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), v1);
				XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), v2);
				XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), v3);
				XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), v4);
				return R;
			}

			inline const float* Matrix::operator[](int i) const
			{
				assert(i >= 0 && i < 4);
				return m[i];
			}

			inline float* Matrix::operator[](int i)
			{
				assert(i >= 0 && i < 4);
				return m[i];
			}


#pragma endregion	Operator
#pragma region		Properties
		public:
			Vector3 Up() const;
			void Up(const Vector3& v);
			Vector3 Down() const;
			void Down(const Vector3& v);
			Vector3 Right() const;
			void Right(const Vector3& v);
			Vector3 Left() const;
			void Left(const Vector3& v);

			Vector3 Forward() const;
			void Forward(const Vector3& v);

			Vector3 Backward() const;
			void Backward(const Vector3& v);

			Vector3 Translation() const;
			void Translation(const Vector3& v);
#pragma endregion	Properties
#pragma region	Function

			inline Matrix Transpose() const
			{
				using namespace DirectX;
				XMMATRIX M = XMLoadFloat4x4(this);
				Matrix R;
				XMStoreFloat4x4(&R, XMMatrixTranspose(M));
				return R;
			}

			inline void Transpose(Matrix& result) const
			{
				using namespace DirectX;
				XMMATRIX M = XMLoadFloat4x4(this);
				XMStoreFloat4x4(&result, XMMatrixTranspose(M));
			}

			inline Matrix Invert() const
			{
				using namespace DirectX;
				XMMATRIX M = XMLoadFloat4x4(this);
				Matrix R;
				XMVECTOR det;
				XMStoreFloat4x4(&R, XMMatrixInverse(&det, M));
				return R;
			}

			inline void Invert(Matrix& result) const
			{
				using namespace DirectX;
				XMMATRIX M = XMLoadFloat4x4(this);
				XMVECTOR det;
				XMStoreFloat4x4(&result, XMMatrixInverse(&det, M));
			}

			inline float Determinant() const
			{
				using namespace DirectX;
				XMMATRIX M = XMLoadFloat4x4(this);
				return XMVectorGetX(XMMatrixDeterminant(M));
			}


			Vector3 Normal(const Vector3 &n) const;
			Vector3 GetRowTransform() const;
			Vector3 GetRowScale() const;


#pragma endregion	Function
#pragma region	StaticFunction
		public:
			static Matrix CreateBillboard(const Vector3& object, const Vector3& cameraPosition, const Vector3& cameraUp, const Vector3* cameraForward);
			static Matrix CreateConstrainedBillboard(const Vector3& object, const Vector3& cameraPosition, const Vector3& rotateAxis,
				const Vector3* cameraForward, const Vector3* objectForward);
			static Matrix CreateTranslation(const Vector3& position); \

				inline static Matrix CreateTranslation(float x, float y, float z)
			{
				using namespace DirectX;
				Matrix R;
				XMStoreFloat4x4(&R, XMMatrixTranslation(x, y, z));
				return R;
			}

			static Matrix CreateScale(const Vector3& scales);
			inline static Matrix CreateScale(float xs, float ys, float zs)
			{
				using namespace DirectX;
				Matrix R;
				XMStoreFloat4x4(&R, XMMatrixScaling(xs, ys, zs));
				return R;
			}

			inline static Matrix CreateScale(float scale)
			{
				using namespace DirectX;
				Matrix R;
				XMStoreFloat4x4(&R, XMMatrixScaling(scale, scale, scale));
				return R;
			}

			inline static Matrix CreateRotationX(float radians)
			{
				using namespace DirectX;
				Matrix R;
				XMStoreFloat4x4(&R, XMMatrixRotationX(radians));
				return R;
			}

			inline static Matrix CreateRotationY(float radians)
			{
				using namespace DirectX;
				Matrix R;
				XMStoreFloat4x4(&R, XMMatrixRotationY(radians));
				return R;
			}

			inline static Matrix CreateRotationZ(float radians)
			{
				using namespace DirectX;
				Matrix R;
				XMStoreFloat4x4(&R, XMMatrixRotationZ(radians));
				return R;
			}

			static Matrix CreateFromAxisAngle(const Vector3& axis, float angle);
			inline static Matrix CreatePerspectiveFieldOfView(float fov, float aspectRatio, float nearPlane, float farPlane)
			{
				using namespace DirectX;
				Matrix R;
				XMStoreFloat4x4(&R, XMMatrixPerspectiveFovRH(fov, aspectRatio, nearPlane, farPlane));
				return R;
			}

			inline static Matrix CreatePerspective(float width, float height, float nearPlane, float farPlane)
			{
				using namespace DirectX;
				Matrix R;
				XMStoreFloat4x4(&R, XMMatrixPerspectiveRH(width, height, nearPlane, farPlane));
				return R;
			}

			inline Matrix CreatePerspectiveOffCenter(float left, float right, float bottom, float top, float nearPlane, float farPlane)
			{
				using namespace DirectX;
				Matrix R;
				XMStoreFloat4x4(&R, XMMatrixPerspectiveOffCenterRH(left, right, bottom, top, nearPlane, farPlane));
				return R;
			}

			inline static Matrix CreateOrthographic(float width, float height, float zNearPlane, float zFarPlane)
			{
				using namespace DirectX;
				Matrix R;
				XMStoreFloat4x4(&R, XMMatrixOrthographicRH(width, height, zNearPlane, zFarPlane));
				return R;
			}

			inline static Matrix CreateOrthographicOffCenter(float left, float right, float bottom, float top, float zNearPlane, float zFarPlane)
			{
				using namespace DirectX;
				Matrix R;
				XMStoreFloat4x4(&R, XMMatrixOrthographicOffCenterRH(left, right, bottom, top, zNearPlane, zFarPlane));
				return R;
			}

			static Matrix CreateLookAt(const Vector3& eye, const Vector3& target, const Vector3& up);
			static Matrix CreateWorld(const Vector3& position, const Vector3& forward, const Vector3& up);
			static Matrix CreateFromQuaternion(const Quaternion& rotation);

			inline static Matrix CreateFromYawPitchRoll(float yaw, float pitch, float roll)
			{
				using namespace DirectX;
				Matrix R;
				XMStoreFloat4x4(&R, XMMatrixRotationRollPitchYaw(pitch, yaw, roll));
				return R;
			}

			inline static void Lerp(const Matrix& M1, const Matrix& M2, float t, Matrix& result)
			{
				using namespace DirectX;
				XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._11));
				XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._21));
				XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._31));
				XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._41));

				XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._11));
				XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._21));
				XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._31));
				XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._41));

				x1 = XMVectorLerp(x1, y1, t);
				x2 = XMVectorLerp(x2, y2, t);
				x3 = XMVectorLerp(x3, y3, t);
				x4 = XMVectorLerp(x4, y4, t);

				XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._11), x1);
				XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._21), x2);
				XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._31), x3);
				XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._41), x4);
			}

			inline static Matrix Lerp(const Matrix& M1, const Matrix& M2, float t)
			{
				using namespace DirectX;
				XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._11));
				XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._21));
				XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._31));
				XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._41));

				XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._11));
				XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._21));
				XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._31));
				XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._41));

				x1 = XMVectorLerp(x1, y1, t);
				x2 = XMVectorLerp(x2, y2, t);
				x3 = XMVectorLerp(x3, y3, t);
				x4 = XMVectorLerp(x4, y4, t);

				Matrix result;
				XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._11), x1);
				XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._21), x2);
				XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._31), x3);
				XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._41), x4);
				return result;
			}

			static void Transform(const Matrix& M, const Quaternion& rotation, Matrix& result);
			static Matrix Transform(const Matrix& M, const Quaternion& rotation);

#pragma endregion	StaticFunction

			static const Matrix Identity;
		};


		inline Matrix operator+ (const Matrix& M1, const Matrix& M2)
		{
			using namespace DirectX;
			XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._11));
			XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._21));
			XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._31));
			XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._41));

			XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._11));
			XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._21));
			XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._31));
			XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._41));

			x1 = XMVectorAdd(x1, y1);
			x2 = XMVectorAdd(x2, y2);
			x3 = XMVectorAdd(x3, y3);
			x4 = XMVectorAdd(x4, y4);

			Matrix R;
			XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
			XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
			XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
			XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
			return R;
		}

		inline Matrix operator- (const Matrix& M1, const Matrix& M2)
		{
			using namespace DirectX;
			XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._11));
			XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._21));
			XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._31));
			XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._41));

			XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._11));
			XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._21));
			XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._31));
			XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._41));

			x1 = XMVectorSubtract(x1, y1);
			x2 = XMVectorSubtract(x2, y2);
			x3 = XMVectorSubtract(x3, y3);
			x4 = XMVectorSubtract(x4, y4);

			Matrix R;
			XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
			XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
			XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
			XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
			return R;
		}

		inline Matrix operator* (const Matrix& M1, const Matrix& M2)
		{
			using namespace DirectX;
			XMMATRIX m1 = XMLoadFloat4x4(&M1);
			XMMATRIX m2 = XMLoadFloat4x4(&M2);
			XMMATRIX X = XMMatrixMultiply(m1, m2);

			Matrix R;
			XMStoreFloat4x4(&R, X);
			return R;
		}

		inline Matrix operator* (const Matrix& M, float S)
		{
			using namespace DirectX;
			XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
			XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
			XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
			XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

			x1 = XMVectorScale(x1, S);
			x2 = XMVectorScale(x2, S);
			x3 = XMVectorScale(x3, S);
			x4 = XMVectorScale(x4, S);

			Matrix R;
			XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
			XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
			XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
			XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
			return R;
		}

		inline Matrix operator/ (const Matrix& M, float S)
		{
			using namespace DirectX;
			assert(S != 0.f);

			XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
			XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
			XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
			XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

			float rs = 1.f / S;

			x1 = XMVectorScale(x1, rs);
			x2 = XMVectorScale(x2, rs);
			x3 = XMVectorScale(x3, rs);
			x4 = XMVectorScale(x4, rs);

			Matrix R;
			XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
			XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
			XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
			XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
			return R;
		}

		inline Matrix operator/ (const Matrix& M1, const Matrix& M2)
		{
			using namespace DirectX;
			XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._11));
			XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._21));
			XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._31));
			XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._41));

			XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._11));
			XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._21));
			XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._31));
			XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._41));

			x1 = XMVectorDivide(x1, y1);
			x2 = XMVectorDivide(x2, y2);
			x3 = XMVectorDivide(x3, y3);
			x4 = XMVectorDivide(x4, y4);

			Matrix R;
			XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
			XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
			XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
			XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
			return R;
		}

		inline Matrix operator* (float S, const Matrix& M)
		{
			using namespace DirectX;

			XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
			XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
			XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
			XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

			x1 = XMVectorScale(x1, S);
			x2 = XMVectorScale(x2, S);
			x3 = XMVectorScale(x3, S);
			x4 = XMVectorScale(x4, S);

			Matrix R;
			XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
			XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
			XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
			XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
			return R;
		}
	};
};

template<> struct std::less<Utility::Math::Matrix>
{
	bool operator()(const Utility::Math::Matrix& M1, const Utility::Math::Matrix& M2) const
	{
		if (M1._11 != M2._11) return M1._11 < M2._11;
		if (M1._12 != M2._12) return M1._12 < M2._12;
		if (M1._13 != M2._13) return M1._13 < M2._13;
		if (M1._14 != M2._14) return M1._14 < M2._14;
		if (M1._21 != M2._21) return M1._21 < M2._21;
		if (M1._22 != M2._22) return M1._22 < M2._22;
		if (M1._23 != M2._23) return M1._23 < M2._23;
		if (M1._24 != M2._24) return M1._24 < M2._24;
		if (M1._31 != M2._31) return M1._31 < M2._31;
		if (M1._32 != M2._32) return M1._32 < M2._32;
		if (M1._33 != M2._33) return M1._33 < M2._33;
		if (M1._34 != M2._34) return M1._34 < M2._34;
		if (M1._41 != M2._41) return M1._41 < M2._41;
		if (M1._42 != M2._42) return M1._42 < M2._42;
		if (M1._43 != M2._43) return M1._43 < M2._43;
		if (M1._44 != M2._44) return M1._44 < M2._44;

		return false;
	}
};