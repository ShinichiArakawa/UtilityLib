/**
 *	@file	Tween.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

namespace Utility
{
	namespace Ease
	{
		/**
		 *	@fn			EaseIn
		 *	@brief		開始が緩やかな2次関数補間
		 *	@param[in]	argTime	!<	経過時間
		 *	@return		補間した値
		 */
		inline float EaseIn(float argTime)
		{
			return argTime * argTime;
		}
		/**
		 *	@fn			EaseOut
		 *	@brief		終了が緩やかな2次関数補間
		 *	@param[in]	argTime	!<	経過時間
		 *	@return		補間した値
		 */
		inline float EaseOut(float argTime)
		{
			return 1.0f - argTime * argTime;
		}
		/**
		 *	@fn			Linear
		 *	@brief		線形補間
		 *	@param[in]	argTime	!<	経過時間
		 *	@return		補間した値
		 */
		inline float Linear(float argTime)
		{
			return argTime;
		}
	}

	using EaseMethod = float(*)(float argTime);

	template <typename T, typename U = T>
	class Tween final
	{
	private:
		T &value_;
		U start_, end_;
		float time_;
		float step_;
		float duration_;
		float frame_;
		EaseMethod ease_;

	public:
		/**
		 *	@constructor	Tween
		 *	@brief			補間クラス
		 *	@param[in]		argVallue	!<	補間する値の参照
		 *	@param[in]		argEnd		!<	変更したい値
		 *	@param[in]		argDuration	!<	終了時間
		 *	@param[in]		argEase		!<	補間の種類
		 */
		Tween(T &argVallue, U argEnd, float argDuration, EaseMethod argEase = Ease::Linear) :
			value_(argVallue), start_(argVallue), end_(argEnd), step_(1.f),
			duration_(argDuration), frame_(0.f), ease_(argEase)
		{
		}
		/**
		 *	@constructor	Tween
		 *	@brief			補間クラス
		 *	@param[in]		argVallue	!<	補間する値の参照
		 *	@param[in]		argStart	!<	開始の値
		 *	@param[in]		argEnd		!<	変更したい値
		 *	@param[in]		argDuration	!<	終了時間
		 *	@param[in]		argEase		!<	補間の種類
		 */
		Tween(T &argVallue, U argStart, U argEnd, float argDuration, EaseMethod argEase = Ease::Linear) :
			value_(argVallue), start_(argStart), end_(argEnd), step_(1.f),
			duration_(argDuration), frame_(0.f), ease_(argEase)
		{
		}

	public:
		bool Update()
		{
			frame_ += step_;
			float t = ease_(frame_ / duration_);
			value_ = start_ * (1.0f - t) + end_ * t;
			return (frame_ <= duration_);
		}
	};
};