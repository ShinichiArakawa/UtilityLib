/**
 *	@file	Vector4.cpp
 *	@date	2017 / 9 / 27
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "Vector4f.h"

#include "Vector2f.h"
#include "Vector3f.h"
#include "Matrix.h"
#include "Quaternion.h"

const Utility::Math::Vector4 Utility::Math::Vector4::Zero = Utility::Math::Vector4(0, 0, 0, 0);

void Utility::Math::Vector4::Transform(const Utility::Math::Vector2& v, const Utility::Math::Quaternion& quat, Utility::Math::Vector4& result)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(&v);
	XMVECTOR q = XMLoadFloat4(&quat);
	XMVECTOR X = XMVector3Rotate(v1, q);
	X = XMVectorSelect(g_XMIdentityR3, X, g_XMSelect1110); // result.w = 1.f
	XMStoreFloat4(&result, X);
}

Utility::Math::Vector4 Utility::Math::Vector4::Transform(const Utility::Math::Vector2& v, const Utility::Math::Quaternion& quat)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(&v);
	XMVECTOR q = XMLoadFloat4(&quat);
	XMVECTOR X = XMVector3Rotate(v1, q);
	X = XMVectorSelect(g_XMIdentityR3, X, g_XMSelect1110); // result.w = 1.f

	Utility::Math::Vector4 result;
	XMStoreFloat4(&result, X);
	return result;
}

void Utility::Math::Vector4::Transform(const Utility::Math::Vector3& v, const Utility::Math::Quaternion& quat, Utility::Math::Vector4& result)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(&v);
	XMVECTOR q = XMLoadFloat4(&quat);
	XMVECTOR X = XMVector3Rotate(v1, q);
	X = XMVectorSelect(g_XMIdentityR3, X, g_XMSelect1110); // result.w = 1.f
	XMStoreFloat4(&result, X);
}

Utility::Math::Vector4 Utility::Math::Vector4::Transform(const Utility::Math::Vector3& v, const Utility::Math::Quaternion& quat)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(&v);
	XMVECTOR q = XMLoadFloat4(&quat);
	XMVECTOR X = XMVector3Rotate(v1, q);
	X = XMVectorSelect(g_XMIdentityR3, X, g_XMSelect1110); // result.w = 1.f

	Utility::Math::Vector4 result;
	XMStoreFloat4(&result, X);
	return result;
}

void Utility::Math::Vector4::Transform(const Utility::Math::Vector4& v, const Utility::Math::Quaternion& quat, Utility::Math::Vector4& result)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(&v);
	XMVECTOR q = XMLoadFloat4(&quat);
	XMVECTOR X = XMVector3Rotate(v1, q);
	X = XMVectorSelect(v1, X, g_XMSelect1110); // result.w = v.w
	XMStoreFloat4(&result, X);
}

Utility::Math::Vector4 Utility::Math::Vector4::Transform(const Utility::Math::Vector4& v, const Utility::Math::Quaternion& quat)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(&v);
	XMVECTOR q = XMLoadFloat4(&quat);
	XMVECTOR X = XMVector3Rotate(v1, q);
	X = XMVectorSelect(v1, X, g_XMSelect1110); // result.w = v.w

	Utility::Math::Vector4 result;
	XMStoreFloat4(&result, X);
	return result;
}

void Utility::Math::Vector4::Transform(const Utility::Math::Vector4& v, const Utility::Math::Matrix& m, Utility::Math::Vector4& result)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(&v);
	XMMATRIX M = XMLoadFloat4x4(&m);
	XMVECTOR X = XMVector4Transform(v1, M);
	XMStoreFloat4(&result, X);
}

Utility::Math::Vector4 Utility::Math::Vector4::Transform(const Utility::Math::Vector4& v, const Utility::Math::Matrix& m)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat4(&v);
	XMMATRIX M = XMLoadFloat4x4(&m);
	XMVECTOR X = XMVector4Transform(v1, M);

	Utility::Math::Vector4 result;
	XMStoreFloat4(&result, X);
	return result;
}
void Utility::Math::Vector4::Transform(const Utility::Math::Vector4* varray, size_t count, const Utility::Math::Matrix& m, Utility::Math::Vector4* resultArray)
{
	using namespace DirectX;
	XMMATRIX M = XMLoadFloat4x4(&m);
	XMVector4TransformStream(resultArray, sizeof(XMFLOAT4), varray, sizeof(XMFLOAT4), count, M);
}
