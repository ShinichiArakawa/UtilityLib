/**
 *	@file	Singleton.h
 *	@brief	mozc式Singleton
 *	@date	2017 / 7 / 05
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 *	@note	http://qiita.com/kikuuuty/items/fcf5f7df2f0493c437dc
 */
#pragma once
#include <functional>
#include <mutex>
#include <assert.h>

namespace Utility
{
	class SingletonFinalizer
	{
	public:
		using FinalizerFunc = std::function<void()>;

		static void AddFinalizer(FinalizerFunc finalizer);
		static void Finalize();
	};

	template <typename T>
	class Singleton final
	{
	private:
		static std::once_flag	isInit_;
		static T*				instance_;

	private:
		static void Create()
		{
			instance_ = new T();
			SingletonFinalizer::AddFinalizer(&Singleton<T>::Destroy);
		}

		static void Destroy()
		{
			delete instance_;
			instance_ = nullptr;
		}

	public:
		static T* Get()
		{
			std::call_once(isInit_, Create);
			return instance_;
		}
	};
	template <typename T> std::once_flag Singleton<T>::isInit_;
	template <typename T> T* Singleton<T>::instance_ = nullptr;
};