/**
 *	@file	QueryPerformanceTimer.h
 *	@date	2017 / 10 / 04
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <memory>

namespace Utility
{
	class QueryPerformanceTimer final
	{
	private:
		class Impl;
		std::unique_ptr<Impl> pImpl;
	public:
		QueryPerformanceTimer();
		~QueryPerformanceTimer();
	public:
		/**
		 *	@fn		Wait
		 *	@brief	指定のFpsまで処理を待つ
		 */
		void Wait();
		/**
		 *	@fn			Reset
		 *	@brief		再初期化
		 *	@param[in]	argFrameRate	!<	フレームレート
		 */
		void Reset(int argFrameRate);
	public:
		/**
		 *	@fn		FramePerSecond
		 *	@brief	フレームレートの取得
		 *	@return	フレームレート
		 */
		float FramePerSecond() const;
		/**
		 *	@fn			DeltaTime
		 *	@brief		経過時間の取得
		 *	@return		経過時間
		 */
		float DeltaTime() const;
		/**
		 *	@fn		sFramePerSecond
		 *	@brief	フレームレートの取得
		 *	@return	フレームレート
		 */
		static float sFramePerSecond();
		
	};
};