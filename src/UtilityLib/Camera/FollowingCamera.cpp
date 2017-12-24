/**
 *	@file	FollowingCamera.cpp
 *	@date	2017 / 8 / 15
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "Camera.h"
#include "FollowingCamera.h"

Utility::FollowingCamera::FollowingCamera()
	:delay_(1.f)
{
	projData_.fovY = Math::ToRadian(60.0f);
	projData_.aspect = (1280.0f / 720.0f);
	projData_.nearClip = 0.1f;
	projData_.farClip = 2000.0f;

	view_ = Math::Matrix::Identity;
	proj_ = Math::Matrix::Identity;

	viewData_.eye = Math::Vector3(0.0f, 6.0f, -20.0f);
	viewData_.ref = Math::Vector3(0.0f, 2.0f, 0.0f);
	viewData_.up = Math::Vector3(0.0f, 1.0f, 0.0f);
}
Utility::FollowingCamera::~FollowingCamera() = default;

void Utility::FollowingCamera::Following(DirectX::XMFLOAT3 argTrans, DirectX::XMFLOAT3 argRot)
{
	Math::Vector3 eyepos = argTrans + Math::Vector3(0.f, 5.f, 0.f);
	Math::Vector3 refpos = argTrans + Math::Vector3(0.0f, 2.0f, 0.0f);

	Math::Matrix rotx = Math::Matrix::CreateRotationX(argRot.x);
	Math::Matrix roty = Math::Matrix::CreateRotationY(argRot.y);
	Math::Matrix rotz = Math::Matrix::CreateRotationZ(argRot.z);
	Math::Matrix rot = rotx * roty * rotz;

	Math::Vector3 cameraV = Math::Vector3::TransformNormal(cameraV, rot);
	eyepos = refpos + cameraV;
	
	refpos = viewData_.ref + (refpos - viewData_.ref) * delay_;
	eyepos = viewData_.eye + (eyepos - viewData_.eye) * delay_;
	viewData_.eye = eyepos;
	viewData_.ref = refpos;
	Camera::Update();
}