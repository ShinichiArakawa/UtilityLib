/**
 *	@file	Camera.cpp
 *	@date	2017 / 8 / 15
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "Camera.h"

Utility::Camera::Camera()
	: view_(DirectX::XMFLOAT4X4()), proj_(DirectX::XMFLOAT4X4()),
	viewData_(ViewProperty(ViewProperty::Eye(0.0f, 2.0f, 10.0f), ViewProperty::Ref(0.0f, 0.0f, 0.0f), ViewProperty::Up(0.0f, 1.0f, 0.0f))),
	projData_(ProjProperty(DirectX::XM_PIDIV4, (640.0f / 480.0f), 1.f, 1000.0f)), yaw_(0.f), pitc_(0.f), length_(0.f)
{
	using namespace DirectX;

	// 注視点と始点の距離を求める
	DirectX::XMVECTOR vec = XMLoadFloat3(&viewData_.ref) - XMLoadFloat3(&viewData_.eye);
	length_ = XMVectorGetX(XMVector3Length(vec));

	//	垂直角度を求める
	//  vec の座標を水平座標に変換し距離をX、カメラの高さをYとして
	//  カメラの垂直角度を求める
	vec = XMVectorSetY(vec, 0);
	float len = XMVectorGetX(XMVector3Length(vec));
	yaw_ = -atan2f(viewData_.ref.y - viewData_.eye.y, len);

	// 水平角度を求める
	pitc_ = atan2(XMVectorGetX(vec), XMVectorGetZ(vec));
	
	Update();
}//	Camera::Camera()

Utility::Camera::~Camera() = default;

void Utility::Camera::Update()
{
	using namespace DirectX;
	XMStoreFloat4x4(&view_, XMMatrixLookAtLH(XMLoadFloat3(&viewData_.eye), XMLoadFloat3(&viewData_.ref), XMLoadFloat3(&viewData_.up)));
	XMStoreFloat4x4(&proj_, XMMatrixPerspectiveFovLH(projData_.fovY, projData_.aspect, projData_.nearClip, projData_.farClip));
}
