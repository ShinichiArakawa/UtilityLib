/**
 *	@file	BottomViewCamera.h
 *	@date	2017 / 8 / 15
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include "Camera.h"

namespace Utility
{

	/**
	 *  @class	BottomViewCamera 
	 *  @brief	見下ろしカメラ
	 */
	class BottomViewCamera : public Camera
	{
	public:
		BottomViewCamera();
		~BottomViewCamera();

	public:
		void BottomView();

	};
};