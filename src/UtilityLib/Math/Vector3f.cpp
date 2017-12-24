/**
 *	@file	Vector3.cpp
 *	@date	2017 / 9 / 27
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "Vector3f.h"

#include "Vector4f.h"
#include "Matrix.h"
#include "Quaternion.h"

const Utility::Math::Vector3 Utility::Math::Vector3::Zero = Utility::Math::Vector3(0, 0, 0);

void Utility::Math::Vector3::Transform(const Utility::Math::Vector3& v, const Utility::Math::Quaternion& quat, Utility::Math::Vector3& result)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(&v);
	XMVECTOR q = XMLoadFloat4(&quat);
	XMVECTOR X = XMVector3Rotate(v1, q);
	XMStoreFloat3(&result, X);
}
Utility::Math::Vector3 Utility::Math::Vector3::Transform(const Utility::Math::Vector3& v, const Utility::Math::Quaternion& quat)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(&v);
	XMVECTOR q = XMLoadFloat4(&quat);
	XMVECTOR X = XMVector3Rotate(v1, q);

	Utility::Math::Vector3 result;
	XMStoreFloat3(&result, X);
	return result;
}
void Utility::Math::Vector3::Transform(const Utility::Math::Vector3& v, const Utility::Math::Matrix& m, Utility::Math::Vector3& result)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(&v);
	XMMATRIX M = XMLoadFloat4x4(&m);
	XMVECTOR X = XMVector3TransformCoord(v1, M);
	XMStoreFloat3(&result, X);
}
Utility::Math::Vector3 Utility::Math::Vector3::Transform(const Utility::Math::Vector3& v, const Utility::Math::Matrix& m)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(&v);
	XMMATRIX M = XMLoadFloat4x4(&m);
	XMVECTOR X = XMVector3TransformCoord(v1, M);

	Utility::Math::Vector3 result;
	XMStoreFloat3(&result, X);
	return result;
}
void Utility::Math::Vector3::Transform(const Utility::Math::Vector3* varray, size_t count, const Utility::Math::Matrix& m, Utility::Math::Vector3* resultArray)
{
	using namespace DirectX;
	XMMATRIX M = XMLoadFloat4x4(&m);
	XMVector3TransformCoordStream(resultArray, sizeof(XMFLOAT3), varray, sizeof(XMFLOAT3), count, M);
}
void Utility::Math::Vector3::Transform(const Utility::Math::Vector3& v, const Utility::Math::Matrix& m, Utility::Math::Vector4& result)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(&v);
	XMMATRIX M = XMLoadFloat4x4(&m);
	XMVECTOR X = XMVector3Transform(v1, M);
	XMStoreFloat4(&result, X);
}

void Utility::Math::Vector3::Transform(const Utility::Math::Vector3* varray, size_t count, const Utility::Math::Matrix& m, Utility::Math::Vector4* resultArray)
{
	using namespace DirectX;
	XMMATRIX M = XMLoadFloat4x4(&m);
	XMVector3TransformStream(resultArray, sizeof(XMFLOAT4), varray, sizeof(XMFLOAT3), count, M);
}
void Utility::Math::Vector3::TransformNormal(const Utility::Math::Vector3& v, const Utility::Math::Matrix& m, Utility::Math::Vector3& result)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(&v);
	XMMATRIX M = XMLoadFloat4x4(&m);
	XMVECTOR X = XMVector3TransformNormal(v1, M);
	XMStoreFloat3(&result, X);
}
Utility::Math::Vector3 Utility::Math::Vector3::TransformNormal(const Utility::Math::Vector3& v, const Utility::Math::Matrix& m)
{
	using namespace DirectX;
	XMVECTOR v1 = XMLoadFloat3(&v);
	XMMATRIX M = XMLoadFloat4x4(&m);
	XMVECTOR X = XMVector3TransformNormal(v1, M);

	Utility::Math::Vector3 result;
	XMStoreFloat3(&result, X);
	return result;
}

void Utility::Math::Vector3::TransformNormal(const Utility::Math::Vector3* varray, size_t count, const Utility::Math::Matrix& m, Utility::Math::Vector3* resultArray)
{
	using namespace DirectX;
	XMMATRIX M = XMLoadFloat4x4(&m);
	XMVector3TransformNormalStream(resultArray, sizeof(XMFLOAT3), varray, sizeof(XMFLOAT3), count, M);
}

Utility::Math::Vector3 Utility::Math::Vector3::MultiplyMatrix(const Utility::Math::Vector3 & argVector3, const Utility::Math::Matrix & argMatrix)
{
	float x, y, z;
	x = argVector3.x * argMatrix[0][0] + argVector3.y * argMatrix[0][1] + argVector3.z * argMatrix[0][2] + argMatrix[0][3];
	y = argVector3.x * argMatrix[1][0] + argVector3.y * argMatrix[1][1] + argVector3.z * argMatrix[1][2] + argMatrix[1][3];
	z = argVector3.x * argMatrix[2][0] + argVector3.y * argMatrix[2][1] + argVector3.z * argMatrix[2][2] + argMatrix[2][3];

	return Utility::Math::Vector3(x, y, z);
}
