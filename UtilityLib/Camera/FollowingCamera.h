/**
 *	@file	FollowingCamera.h
 *	@date	2017 / 8 / 15
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "Camera.h"

namespace Utility
{
	/**
	 *  @class	FollowingCamera 
	 *  @brief	追跡カメラ
	 */
	class FollowingCamera : public Camera
	{
	private:
		float delay_;
	public:
		FollowingCamera();
		~FollowingCamera();
	public:

		/**
		 *  @fn			Following
		 *  @brief		指定の座標に追跡
		 *  @param[in]	argTrans	!<	追跡したい座標
		 *  @param[in]	argRot		!<	回転
		 */
		void Following(DirectX::XMFLOAT3 argTrans, DirectX::XMFLOAT3 argRot);
	public:
		inline float Delay()const { return delay_; }
		inline void Delay(float argDelay){ delay_ = argDelay; }

	};

};