/**
 *	@file	DebugCamera.h
 *	@date	2017 / 8 / 15
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "Camera.h"

namespace Utility
{
	class InputManager;
	/**
	 *  @class	DebugCamera 
	 *  @brief	デバックカメラ
	 */
	class DebugCamera : public Camera
	{
	public:
		DebugCamera();
		~DebugCamera();
	public:
		void OperationMouse(InputManager* argInput);
	};
};