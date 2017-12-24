/**
 *	@file	DebugCamera.cpp
 *	@date	2017 / 8 / 15
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "DebugCamera.h"
#include "../InputManager/InputManager.h"

#include <Windows.h>

Utility::DebugCamera::DebugCamera()
{
	view_ = DirectX::XMFLOAT4X4();
	proj_ = DirectX::XMFLOAT4X4();
	viewData_ = ViewProperty(ViewProperty::Eye(0.0f, 2.0f, 10.0f), ViewProperty::Ref(0.0f, 0.0f, 0.0f), ViewProperty::Up(0.0f, 1.0f, 0.0f));
	projData_ = ProjProperty(DirectX::XM_PIDIV4, (640.0f / 480.0f), 1.f, 1000.0f);
	yaw_ = 0.f; 
	pitc_ = 0.f; 
	length_ = 0.f;

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
}

Utility::DebugCamera::~DebugCamera() = default;

void Utility::DebugCamera::OperationMouse(InputManager* argInput)
{
	using namespace DirectX;
	const POINT NowPos = argInput->NowPoint();
	const POINT OldPos = argInput->OldPoint();
	const POINT MoveVal = { OldPos.x - NowPos.x,  OldPos.y - NowPos.y };

	if (argInput->IsPressed(eMouseButton::Left) && !argInput->IsPressed(eMouseButton::Middle))
	{//	視点の回転

		// マウスの移動量から回転角度を計算する
		pitc_ += MoveVal.x * 0.01f; // 右手座標では減算、左手座標では加算
		yaw_ += MoveVal.y * 0.01f;
		// ジンバルロックにならないように垂直回転に限界を設ける
		yaw_ = std::max(yaw_, -1.5f);
		yaw_ = std::min(yaw_, 1.5f);

		// 注視点から視点の距離をベクトルに変換
		DirectX::XMVECTOR vec = XMVectorSet(0, 0, -length_, 0);

		// 距離と回転情報から、カメラの方向ベクトルを求める
		DirectX::XMMATRIX m0 = XMMatrixRotationX(yaw_);
		DirectX::XMMATRIX m1 = XMMatrixRotationY(pitc_);
		m0 = m0 * m1;
		vec = XMVector3TransformCoord(vec, m0);
		//	視点を更新
		XMStoreFloat3(&viewData_.eye, XMLoadFloat3(&viewData_.ref) + vec);
	}
	else if (argInput->IsPressed(eMouseButton::Middle) && !argInput->IsPressed(eMouseButton::Left))
	{//	視点と注視点の移動（Z方向への移動）

		// 視点から注視点のベクトルを求める
		DirectX::XMVECTOR v0 = XMLoadFloat3(&viewData_.eye) - XMLoadFloat3(&viewData_.ref);
		// v0を単位行列にする
		v0 = XMVector3Normalize(v0);

		// マウスの移動量からカメラの移動量を計算をする
		v0 = v0 * 0.01f * static_cast<float>(MoveVal.y);

		// 視点と注視点に移動量を加える
		XMStoreFloat3(&viewData_.eye, XMLoadFloat3(&viewData_.eye) + v0);
		XMStoreFloat3(&viewData_.ref, XMLoadFloat3(&viewData_.ref) + v0);
	}
	else if (argInput->IsPressed(eMouseButton::Left) && argInput->IsPressed(eMouseButton::Middle))
	{// 視点のパン(平行移動)
		
		// 視点から注視点のベクトルを求める
		DirectX::XMVECTOR v0 = XMLoadFloat3(&viewData_.eye) - XMLoadFloat3(&viewData_.ref);
		// カメラの上方向とv0の外積から、右方向のベクトルを求める
		DirectX::XMVECTOR v1 = XMVector3Cross(v0, XMLoadFloat3(&viewData_.up));
		// v0とv1の外積から、右方向のベクトルを求める
		DirectX::XMVECTOR v2 = XMVector3Cross(v0, v1);
		// v1とv2のベクトルを単位ベクトルにする
		v1 = XMVector3Normalize(v1);
		v2 = XMVector3Normalize(v2);

		// マウスの移動量から、平行移動のベクトルを求める
		v0 = (v1 * 0.01f * static_cast<float>(MoveVal.x)) + (v2 * 0.01f * static_cast<float>(MoveVal.y));	// 左手座標の場合
		//v0 = (v1 * 0.01f * static_cast<float>(-MoveVal.x)) + (v2 * 0.01f * static_cast<float>(MoveVal.y));	// 右手座標の場合
		// 視点と注視点に移動量を加える
		XMStoreFloat3(&viewData_.eye, XMLoadFloat3(&viewData_.eye) + v0);
		XMStoreFloat3(&viewData_.ref, XMLoadFloat3(&viewData_.ref) + v0);
	}//	argInput->IsPressed

	Camera::Update();

}//	OperationMouse