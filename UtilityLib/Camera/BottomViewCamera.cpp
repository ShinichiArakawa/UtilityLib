/**
 *	@file	BottomViewCamera.cpp
 *	@date	2017 / 8 / 15
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "BottomViewCamera.h"

Utility::BottomViewCamera::BottomViewCamera()
{
	projData_.fovY = Math::ToRadian(60.0f);
	projData_.aspect = (640.0f / 480.0f);
	projData_.nearClip = 0.1f;
	projData_.farClip = 100.0f;

	viewData_.eye = Math::Vector3(0.0f, 6.0f, -20.0f);
	viewData_.ref = Math::Vector3(0.0f, 2.0f, -20.0f);
	viewData_.up = Math::Vector3(0.0f, 1.0f, 0.0f);

	proj_ = Math::Matrix::Identity;
	view_ = Math::Matrix::Identity;

	Camera::Update();
}
Utility::BottomViewCamera::~BottomViewCamera() = default;

void Utility::BottomViewCamera::BottomView()
{
	const float CameraDistance = 50.0f;
	const Math::Vector3 cameraV = Math::Vector3(0.0f, CameraDistance, -0.1f);
	Math::Vector3 eyepos = cameraV;

	eyepos =  (eyepos - viewData_.eye) * 0.03f + Math::Vector3(0.0f, 0.5f, 0.f);
	
	viewData_.eye = viewData_.eye + eyepos;
	viewData_.up = Math::Vector3::TransformNormal(Math::Vector3(0, 1, 0), Math::Matrix::Identity);

	using namespace DirectX;
	XMStoreFloat4x4(&view_, XMMatrixLookAtLH(XMLoadFloat3(&viewData_.eye), XMLoadFloat3(&viewData_.ref), XMLoadFloat3(&viewData_.up)));
	XMStoreFloat4x4(&proj_, XMMatrixPerspectiveFovLH(projData_.fovY, projData_.aspect, projData_.nearClip, projData_.farClip));
	Camera::Update();
}
