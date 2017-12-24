/**
 *	@file	TaskManager.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <memory>

namespace Utility
{
	class Task;

	class TaskManager final
	{
	private:
		class Impl;
		std::unique_ptr<Impl>pImpl;
	public:

		TaskManager();
		~TaskManager();

	public:
		void Add(Task *argTask);
		void Join();
		void Clear();
		void Update();
		void Draw();
		void DelayDraw();
		void IsDelay(bool argIsDelay);
		bool IsDelay();
	};
};