/**
 *	@file	Vector2.cpp
 *	@date	2017 / 9 / 27
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "Vector2f.h"

#include "Vector4f.h"
#include "Matrix.h"
#include "Quaternion.h"

const Utility::Math::Vector2 Utility::Math::Vector2::Zero = Utility::Math::Vector2(0, 0);

void Utility::Math::Vector2::Transform(const Utility::Math::Vector2& v, const Utility::Math::Quaternion& quat, Utility::Math::Vector2& result)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(&v);
	XMVECTOR q = XMLoadFloat4(&quat);
	XMVECTOR X = XMVector3Rotate(v1, q);
	XMStoreFloat2(&result, X);
}
Utility::Math::Vector2 Utility::Math::Vector2::Transform(const Utility::Math::Vector2& v, const Utility::Math::Quaternion& quat)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(&v);
	XMVECTOR q = XMLoadFloat4(&quat);
	XMVECTOR X = XMVector3Rotate(v1, q);

	Utility::Math::Vector2 result;
	XMStoreFloat2(&result, X);
	return result;
}
void Utility::Math::Vector2::Transform(const Utility::Math::Vector2& v, const Utility::Math::Matrix& m, Utility::Math::Vector2& result)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(&v);
	XMMATRIX M = XMLoadFloat4x4(&m);
	XMVECTOR X = XMVector2TransformCoord(v1, M);
	XMStoreFloat2(&result, X);
}

Utility::Math::Vector2 Utility::Math::Vector2::Transform(const Utility::Math::Vector2& v, const Utility::Math::Matrix& m)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(&v);
	XMMATRIX M = XMLoadFloat4x4(&m);
	XMVECTOR X = XMVector2TransformCoord(v1, M);

	Utility::Math::Vector2 result;
	XMStoreFloat2(&result, X);
	return result;
}

void Utility::Math::Vector2::Transform(const Utility::Math::Vector2* varray, size_t count, const Utility::Math::Matrix& m, Utility::Math::Vector2* resultArray)
{
	using namespace DirectX;
	XMMATRIX M = XMLoadFloat4x4(&m);
	XMVector2TransformCoordStream(resultArray, sizeof(XMFLOAT2), varray, sizeof(XMFLOAT2), count, M);
}

void Utility::Math::Vector2::Transform(const Utility::Math::Vector2& v, const Utility::Math::Matrix& m, Utility::Math::Vector4& result)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(&v);
	XMMATRIX M = XMLoadFloat4x4(&m);
	XMVECTOR X = XMVector2Transform(v1, M);
	XMStoreFloat4(&result, X);
}

void Utility::Math::Vector2::Transform(const Utility::Math::Vector2* varray, size_t count, const Utility::Math::Matrix& m, Utility::Math::Vector4* resultArray)
{
	using namespace DirectX;
	XMMATRIX M = XMLoadFloat4x4(&m);
	XMVector2TransformStream(resultArray, sizeof(XMFLOAT4), varray, sizeof(XMFLOAT2), count, M);
}

void Utility::Math::Vector2::TransformNormal(const Utility::Math::Vector2& v, const Utility::Math::Matrix& m, Utility::Math::Vector2& result)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(&v);
	XMMATRIX M = XMLoadFloat4x4(&m);
	XMVECTOR X = XMVector2TransformNormal(v1, M);
	XMStoreFloat2(&result, X);
}
Utility::Math::Vector2 Utility::Math::Vector2::TransformNormal(const Utility::Math::Vector2& v, const Utility::Math::Matrix& m)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat2(&v);
	XMMATRIX M = XMLoadFloat4x4(&m);
	XMVECTOR X = XMVector2TransformNormal(v1, M);

	Vector2 result;
	XMStoreFloat2(&result, X);
	return result;
}

void Utility::Math::Vector2::TransformNormal(const Utility::Math::Vector2* varray, size_t count, const Utility::Math::Matrix& m, Utility::Math::Vector2* resultArray)
{
	using namespace DirectX;
	XMMATRIX M = XMLoadFloat4x4(&m);
	XMVector2TransformNormalStream(resultArray, sizeof(XMFLOAT2), varray, sizeof(XMFLOAT2), count, M);
}