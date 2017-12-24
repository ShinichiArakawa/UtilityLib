/**
 *	@file	Quaternion.cpp
 *	@date	2017 / 9 / 27
 *	@author	Katsumi Takei
 *	@note	http://www.j3d.org/matrix_faq/matrfaq_latest.html#Q56
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "Quaternion.h"

#include "Vector3f.h"
#include "Vector4f.h"
#include "Matrix.h"


Utility::Math::Quaternion::Quaternion(const Utility::Math::Vector3& v, float scalar)
	: XMFLOAT4(v.x, v.y, v.z, scalar)
{
}

Utility::Math::Quaternion::Quaternion(const Utility::Math::Vector4& v)
	: XMFLOAT4(v.x, v.y, v.z, v.w)
{
}

Utility::Math::Quaternion Utility::Math::Quaternion::CreateFromAxisAngle(const Utility::Math::Vector3& axis, float angle)
{
	using namespace DirectX;
	XMVECTOR a = XMLoadFloat3(&axis);

	Utility::Math::Quaternion R;
	XMStoreFloat4(&R, XMQuaternionRotationAxis(a, angle));
	return R;
}

Utility::Math::Quaternion Utility::Math::Quaternion::CreateFromRotationMatrix(const Utility::Math::Matrix& M)
{
	using namespace DirectX;
	XMMATRIX M0 = XMLoadFloat4x4(&M);

	Utility::Math::Quaternion R;
	XMStoreFloat4(&R, XMQuaternionRotationMatrix(M0));
	return R;
}