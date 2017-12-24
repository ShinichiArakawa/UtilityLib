/**
 *	@file	Matrix.cpp
 *	@date	2017 / 9 / 27
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "Matrix.h"

#include "Vector3f.h"
#include "Vector4f.h"
#include "Quaternion.h"

const Utility::Math::Matrix Utility::Math::Matrix::Identity = Utility::Math::Matrix();

Utility::Math::Matrix::Matrix(const Utility::Math::Vector3& r0, const Utility::Math::Vector3& r1, const Utility::Math::Vector3& r2)
	: XMFLOAT4X4( 
	r0.x, r0.y, r0.z, 0,
	r1.x, r1.y, r1.z, 0,
	r2.x, r2.y, r2.z, 0,
	0, 0, 0, 1.f) 
{
}
Utility::Math::Matrix::Matrix(const Utility::Math::Vector4& r0, const Utility::Math::Vector4& r1, const Utility::Math::Vector4& r2, const Utility::Math::Vector4& r3)
	: XMFLOAT4X4(
	r0.x, r0.y, r0.z, r0.w,
	r1.x, r1.y, r1.z, r1.w,
	r2.x, r2.y, r2.z, r2.w,
	r3.x, r3.y, r3.z, r3.w)
{
}
#pragma region	Property

Utility::Math::Vector3 Utility::Math::Matrix::Up() const
{
	return Utility::Math::Vector3(_21, _22, _23);
}

void Utility::Math::Matrix::Up(const Utility::Math::Vector3& v)
{
	_21 = v.x;
	_22 = v.y; 
	_23 = v.z;
}

Utility::Math::Vector3 Utility::Math::Matrix::Down() const
{
	return Utility::Math::Vector3(-_21, -_22, -_23);
}
void Utility::Math::Matrix::Down(const Utility::Math::Vector3& v)
{
	_21 = -v.x; 
	_22 = -v.y; 
	_23 = -v.z; 
}

Utility::Math::Vector3 Utility::Math::Matrix::Right() const
{
	return Utility::Math::Vector3(_11, _12, _13);
}
void Utility::Math::Matrix::Right(const Utility::Math::Vector3& v)
{
	_11 = v.x; 
	_12 = v.y;
	_13 = v.z; 
}

Utility::Math::Vector3 Utility::Math::Matrix::Left() const
{
	return Utility::Math::Vector3(-_11, -_12, -_13);
}
void Utility::Math::Matrix::Left(const Utility::Math::Vector3& v)
{ 
	_11 = -v.x; 
	_12 = -v.y;
	_13 = -v.z; 
}

Utility::Math::Vector3 Utility::Math::Matrix::Forward() const
{ 
	return Utility::Math::Vector3(-_31, -_32, -_33);
}
void Utility::Math::Matrix::Forward(const Utility::Math::Vector3& v)
{
	_31 = -v.x; 
	_32 = -v.y; 
	_33 = -v.z; 
}

Utility::Math::Vector3 Utility::Math::Matrix::Backward() const
{ 
	return Utility::Math::Vector3(_31, _32, _33);
}
void Utility::Math::Matrix::Backward(const Utility::Math::Vector3& v)
{ 
	_31 = v.x; 
	_32 = v.y; 
	_33 = v.z; 
}

Utility::Math::Vector3 Utility::Math::Matrix::Translation() const
{
	return Utility::Math::Vector3(_41, _42, _43);
}
void Utility::Math::Matrix::Translation(const Utility::Math::Vector3& v)
{
	_41 = v.x;
	_42 = v.y;
	_43 = v.z;
}

#pragma endregion	Property

#pragma region	Function

Utility::Math::Vector3 Utility::Math::Matrix::Normal(const Utility::Math::Vector3 &n) const
{
	Utility::Math::Vector3 v;
	v.x = n.x * m[0][0] + n.y * m[0][1] + n.z * m[0][2];
	v.y = n.x * m[1][0] + n.y * m[1][1] + n.z * m[1][2];
	v.z = n.x * m[2][0] + n.y * m[2][1] + n.z * m[2][2];

	v.Normalize();
	return v;
}

Utility::Math::Vector3 Utility::Math::Matrix::GetRowTransform() const
{
	Utility::Math::Vector3 T;
	T.x = m[0][3];
	T.y = m[1][3];
	T.z = m[2][3];
	return T;
}

Utility::Math::Vector3 Utility::Math::Matrix::GetRowScale() const
{
	Utility::Math::Vector3 S;
	S.x = Utility::Math::Vector3(m[0][0], m[1][0], m[2][0]).Length();
	S.y = Utility::Math::Vector3(m[0][1], m[1][1], m[2][1]).Length();
	S.z = Utility::Math::Vector3(m[0][2], m[1][2], m[2][2]).Length();
	return S;
}

#pragma endregion	Function

#pragma region	StaticFunction
Utility::Math::Matrix Utility::Math::Matrix::CreateBillboard(const Utility::Math::Vector3& object, const Utility::Math::Vector3& cameraPosition, const Utility::Math::Vector3& cameraUp, const Utility::Math::Vector3* cameraForward)
{
	using namespace DirectX;
	XMVECTOR O = XMLoadFloat3(&object);
	XMVECTOR C = XMLoadFloat3(&cameraPosition);
	XMVECTOR Z = XMVectorSubtract(O, C);

	XMVECTOR N = XMVector3LengthSq(Z);
	if (XMVector3Less(N, g_XMEpsilon))
	{
		if (cameraForward)
		{
			XMVECTOR F = XMLoadFloat3(cameraForward);
			Z = XMVectorNegate(F);
		}
		else
			Z = g_XMNegIdentityR2;
	}
	else
	{
		Z = XMVector3Normalize(Z);
	}

	XMVECTOR up = XMLoadFloat3(&cameraUp);
	XMVECTOR X = XMVector3Cross(up, Z);
	X = XMVector3Normalize(X);

	XMVECTOR Y = XMVector3Cross(Z, X);

	XMMATRIX M;
	M.r[0] = X;
	M.r[1] = Y;
	M.r[2] = Z;
	M.r[3] = XMVectorSetW(O, 1.f);

	Matrix R;
	XMStoreFloat4x4(&R, M);
	return R;
}

Utility::Math::Matrix Utility::Math::Matrix::CreateConstrainedBillboard(const Utility::Math::Vector3& object, const Utility::Math::Vector3& cameraPosition, const Utility::Math::Vector3& rotateAxis,
	const Utility::Math::Vector3* cameraForward, const Utility::Math::Vector3* objectForward)
{
	using namespace DirectX;

	static const XMVECTORF32 s_minAngle = { 0.99825467075f, 0.99825467075f, 0.99825467075f, 0.99825467075f }; // 1.0 - XMConvertToRadians( 0.1f );

	XMVECTOR O = XMLoadFloat3(&object);
	XMVECTOR C = XMLoadFloat3(&cameraPosition);
	XMVECTOR faceDir = XMVectorSubtract(O, C);

	XMVECTOR N = XMVector3LengthSq(faceDir);
	if (XMVector3Less(N, g_XMEpsilon))
	{
		if (cameraForward)
		{
			XMVECTOR F = XMLoadFloat3(cameraForward);
			faceDir = XMVectorNegate(F);
		}
		else
		{
			faceDir = g_XMNegIdentityR2;
		}
	}
	else
	{
		faceDir = XMVector3Normalize(faceDir);
	}

	XMVECTOR Y = XMLoadFloat3(&rotateAxis);
	XMVECTOR X, Z;

	XMVECTOR dot = XMVectorAbs(XMVector3Dot(Y, faceDir));
	if (XMVector3Greater(dot, s_minAngle))
	{
		if (objectForward)
		{
			Z = XMLoadFloat3(objectForward);
			dot = XMVectorAbs(XMVector3Dot(Y, Z));
			if (XMVector3Greater(dot, s_minAngle))
			{
				dot = XMVectorAbs(XMVector3Dot(Y, g_XMNegIdentityR2));
				Z = (XMVector3Greater(dot, s_minAngle)) ? g_XMIdentityR0 : g_XMNegIdentityR2;
			}
		}
		else
		{
			dot = XMVectorAbs(XMVector3Dot(Y, g_XMNegIdentityR2));
			Z = (XMVector3Greater(dot, s_minAngle)) ? g_XMIdentityR0 : g_XMNegIdentityR2;
		}

		X = XMVector3Cross(Y, Z);
		X = XMVector3Normalize(X);

		Z = XMVector3Cross(X, Y);
		Z = XMVector3Normalize(Z);
	}
	else
	{
		X = XMVector3Cross(Y, faceDir);
		X = XMVector3Normalize(X);

		Z = XMVector3Cross(X, Y);
		Z = XMVector3Normalize(Z);
	}

	XMMATRIX M;
	M.r[0] = X;
	M.r[1] = Y;
	M.r[2] = Z;
	M.r[3] = XMVectorSetW(O, 1.f);

	Utility::Math::Matrix R;
	XMStoreFloat4x4(&R, M);
	return R;
}
Utility::Math::Matrix Utility::Math::Matrix::CreateTranslation(const Utility::Math::Vector3& position)
{
	using namespace DirectX;
	Utility::Math::Matrix R;
	XMStoreFloat4x4(&R, XMMatrixTranslation(position.x, position.y, position.z));
	return R;
}

Utility::Math::Matrix Utility::Math::Matrix::CreateScale(const Utility::Math::Vector3& scales)
{
	using namespace DirectX;
	Utility::Math::Matrix R;
	XMStoreFloat4x4(&R, XMMatrixScaling(scales.x, scales.y, scales.z));
	return R;
}

Utility::Math::Matrix Utility::Math::Matrix::CreateFromAxisAngle(const Utility::Math::Vector3& axis, float angle)
{
	using namespace DirectX;
	Utility::Math::Matrix R;
	XMVECTOR a = XMLoadFloat3(&axis);
	XMStoreFloat4x4(&R, XMMatrixRotationAxis(a, angle));
	return R;
}
Utility::Math::Matrix Utility::Math::Matrix::CreateLookAt(const Utility::Math::Vector3& eye, const Utility::Math::Vector3& target, const Utility::Math::Vector3& up)
{
	using namespace DirectX;
	Utility::Math::Matrix R;
	XMVECTOR eyev = XMLoadFloat3(&eye);
	XMVECTOR targetv = XMLoadFloat3(&target);
	XMVECTOR upv = XMLoadFloat3(&up);
	XMStoreFloat4x4(&R, XMMatrixLookAtRH(eyev, targetv, upv));
	return R;
}
Utility::Math::Matrix Utility::Math::Matrix::CreateWorld(const Utility::Math::Vector3& position, const Utility::Math::Vector3& forward, const Utility::Math::Vector3& up)
{
	using namespace DirectX;
	XMVECTOR zaxis = XMVector3Normalize(XMVectorNegate(XMLoadFloat3(&forward)));
	XMVECTOR yaxis = XMLoadFloat3(&up);
	XMVECTOR xaxis = XMVector3Normalize(XMVector3Cross(yaxis, zaxis));
	yaxis = XMVector3Cross(zaxis, xaxis);

	Utility::Math::Matrix R;
	XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&R._11), xaxis);
	XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&R._21), yaxis);
	XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&R._31), zaxis);
	R._14 = R._24 = R._34 = 0.f;
	R._41 = position.x; R._42 = position.y; R._43 = position.z;
	R._44 = 1.f;
	return R;
}

Utility::Math::Matrix Utility::Math::Matrix::CreateFromQuaternion(const Utility::Math::Quaternion& rotation)
{
	using namespace DirectX;
	Utility::Math::Matrix R;
	XMVECTOR quatv = XMLoadFloat4(&rotation);
	XMStoreFloat4x4(&R, XMMatrixRotationQuaternion(quatv));
	return R;
}
void Utility::Math::Matrix::Transform(const Utility::Math::Matrix& M, const Utility::Math::Quaternion& rotation, Utility::Math::Matrix& result)
{
	using namespace DirectX;
	XMVECTOR quatv = XMLoadFloat4(&rotation);

	XMMATRIX M0 = XMLoadFloat4x4(&M);
	XMMATRIX M1 = XMMatrixRotationQuaternion(quatv);

	XMStoreFloat4x4(&result, XMMatrixMultiply(M0, M1));
}

Utility::Math::Matrix Utility::Math::Matrix::Transform(const Utility::Math::Matrix& M, const Utility::Math::Quaternion& rotation)
{
	using namespace DirectX;
	XMVECTOR quatv = XMLoadFloat4(&rotation);

	XMMATRIX M0 = XMLoadFloat4x4(&M);
	XMMATRIX M1 = XMMatrixRotationQuaternion(quatv);

	Utility::Math::Matrix result;
	XMStoreFloat4x4(&result, XMMatrixMultiply(M0, M1));
	return result;
}
#pragma endregion	StaticFunction