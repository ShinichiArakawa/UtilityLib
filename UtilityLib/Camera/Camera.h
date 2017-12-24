/**
 *	@file	Camera.h
 *	@date	2017 / 8 / 15
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "../Math/Math.h"

namespace Utility
{
	struct ViewProperty
	{
		using Eye = DirectX::XMFLOAT3;
		using Ref = DirectX::XMFLOAT3;
		using Up = DirectX::XMFLOAT3;
		/**
		 *	@construct	ViewProperty
		 *	@brief		ビュー行列に必要なデータをXMFLOAT3で作成する
		 *	@param[in]	argEye	!<	視点
		 *	@param[in]	argRef	!<	注視点（対象点）
		 *	@param[in]	argUp	!<	軸
		 */
		ViewProperty(Eye argEye, Ref argRef, Up argUp)
			:eye(argEye), ref(argRef), up(argUp)
		{
		}
		/**
		 *	@construct	ViewProperty
		 *	@brief		ビュー行列に必要なデータをXMVECTORで作成する
		 *	@param[in]	argEye	!<	視点
		 *	@param[in]	argRef	!<	注視点（対象点）
		 *	@param[in]	argUp	!<	軸
		 */
		ViewProperty(DirectX::XMVECTOR argEye, DirectX::XMVECTOR argRef, DirectX::XMVECTOR argUp)
		{
			using DirectX::XMStoreFloat3;
			XMStoreFloat3(&ref, argRef);
			XMStoreFloat3(&eye, argEye);
			XMStoreFloat3(&up, argUp);
		}

		Eye eye;	//	!<	カメラ座標
		Ref ref;	//	!<	ターゲット座標
		Up	up;		//	!<	上方向ベクトル
	};//	struct ViewProperty

	struct ProjProperty
	{

		ProjProperty(float argFovY, float argAspect, float argNearClip, float argFarClip)
			:fovY(argFovY), aspect(argAspect), nearClip(argNearClip), farClip(argFarClip)
		{
		}

		float fovY;		//	!<	縦方向視野角
		float aspect;	//	!<	アスペクト比
		float nearClip;	//	!<	ニアクリップ
		float farClip;	//	!<	ファーグリップ
	};//	struct ProjProperty

	class Camera
	{
	protected:
		float yaw_;    // ヨー
		float pitc_;   // ピッチ
		float length_; //  距離
	protected:
		ViewProperty viewData_;
		ProjProperty projData_;
		DirectX::XMFLOAT4X4 view_;
		DirectX::XMFLOAT4X4 proj_;

	public:
		Camera();
		virtual ~Camera();
	public:
		void Update();

#pragma region		Property
	public:
		//	構造体
		virtual ProjProperty ProjData() const { return projData_; }
		virtual void ProjData(ProjProperty argValue) { projData_ = argValue; }
		virtual ViewProperty ViewData() const { return viewData_; }
		virtual void ViewData(ViewProperty argValue) { viewData_ = argValue; }
		//	行列	
		virtual DirectX::XMFLOAT4X4 Proj() const { return proj_; }
		virtual void Proj(DirectX::XMFLOAT4X4 argValue) { proj_ = argValue; }
		virtual DirectX::XMFLOAT4X4 View() const { return view_; }
		virtual void View(DirectX::XMFLOAT4X4 argValue) { view_ = argValue; }
		//	要素	
		inline void Eye(const DirectX::XMFLOAT3 argEye) { viewData_.eye = argEye; }
		inline void Ref(const DirectX::XMFLOAT3 argRef) { viewData_.ref = argRef; }
		inline void Up(const DirectX::XMFLOAT3 argUp) { viewData_.up = argUp; }
		inline void Aspect(const float argAspect) { projData_.aspect = argAspect; }
		inline void FovY(const float argFovY) { projData_.fovY = argFovY; }
		inline void NearCrip(const float argNearclip) { projData_.nearClip = argNearclip; }
		inline void FarCrip(const float argFarclip) { projData_.farClip = argFarclip; }
		inline const DirectX::XMFLOAT3& Eye()const { return viewData_.eye; }
		inline const DirectX::XMFLOAT3& Ref()const { return viewData_.ref; }
		inline const DirectX::XMFLOAT3& Up()const { return viewData_.up; }
#pragma endregion	Property

	};//	class Camera
};