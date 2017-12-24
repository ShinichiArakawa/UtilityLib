/**
 *	@file	Event.h
 *	@date	2017 / 11 / 21
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <iostream>
#include <memory>
#include <functional>

namespace Utility
{

	template <typename T, typename U, typename ...Args>
	class Event 
	{

	private:
		T* listener_;
		std::function<U(Args...)> func_;
	public:
		Event(T* argObj, std::function<U(Args...)> argFunc)
		{
			listener_ = argObj;
			func_ = argFunc;
		};

		virtual U Invoke(Args... arg)
		{
			(listener_->*func_)(arg);
		};
	};

}