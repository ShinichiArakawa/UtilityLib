/**
 *	@file	TaskManager.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "TaskManager.h"
#include "Task.h"

#include <list>

class Utility::TaskManager::Impl
{
public:
	Impl() = default;
	~Impl() = default;

private:

	void SortDrawList()
	{
		drawPlaneAddList_.clear();
		drawPlaneAlphaList_.clear();
		drawSolidList_.clear();

		delayDrawPlaneAddList_.clear();
		delayDrawPlaneAlphaList_.clear();
		delayDrawSolidList_.clear();

		for (auto lTask : taskList_)
		{
			switch (lTask->DrawModeState())
			{
			case eDrawMode::Solid:
			{
				(lTask->IsDelay()) ? delayDrawSolidList_.push_back(lTask) : drawSolidList_.push_back(lTask);
				break;
			}
			case eDrawMode::Alpha:
			{
				(lTask->IsDelay()) ? delayDrawPlaneAlphaList_.push_back(lTask) : drawPlaneAlphaList_.push_back(lTask);
				break;
			}
			case eDrawMode::Add:
			{
				(lTask->IsDelay()) ? delayDrawPlaneAddList_.push_back(lTask) : drawPlaneAddList_.push_back(lTask);
				break;
			}
			default:
				break;
			}
		}
	}

public:
	void Join()
	{
		taskList_.insert(taskList_.end(), nextJoinList_.begin(), nextJoinList_.end());
		taskList_.sort([](const Utility::Task *argLeft, const Utility::Task *argRight) { return (argLeft->Priority() < argRight->Priority()); });
		SortDrawList();
		nextJoinList_.clear();
	}

	void Clear()
	{
		drawPlaneAddList_.clear();
		drawPlaneAlphaList_.clear();
		drawSolidList_.clear();
		taskList_.clear();
		nextJoinList_.clear();
	}

	void Update()
	{
		Join();
		for (auto it = taskList_.begin(); it != taskList_.end();)
		{
			Task *task = *it;

			if (!task->Update())
			{
				it = taskList_.erase(it);
				continue;
			}
			++it;
		}
	}

	void Draw()
	{
		for (auto lTask : drawSolidList_)
		{
			lTask->Draw();
		}
		for (auto lTask : drawPlaneAlphaList_)
		{
			lTask->Draw();
		}

		for (auto lTask : drawPlaneAddList_)
		{
			lTask->Draw();
		}
	}

	void DelayDraw()
	{
		for (auto lTask : delayDrawSolidList_)
		{
			lTask->Draw();
		}
		for (auto lTask : delayDrawPlaneAlphaList_)
		{
			lTask->Draw();
		}

		for (auto lTask : delayDrawPlaneAddList_)
		{
			lTask->Draw();
		}
	}

public:
	std::list<Task*> taskList_;
	std::list<Task*> nextJoinList_;
	std::list<Task*> drawPlaneAlphaList_;
	std::list<Task*> drawPlaneAddList_;
	std::list<Task*> drawSolidList_;
	bool isDelay_ = false;

	std::list<Task*> delayDrawPlaneAlphaList_;
	std::list<Task*> delayDrawPlaneAddList_;
	std::list<Task*> delayDrawSolidList_;
};

Utility::TaskManager::TaskManager()
	: pImpl(std::make_unique<Impl>())
{
}

Utility::TaskManager::~TaskManager()
{
	pImpl->Clear();
}

void Utility::TaskManager::Add(Task *argTask)
{
	pImpl->nextJoinList_.push_back(argTask);
}

void Utility::TaskManager::Join()
{
	pImpl->Join();
}

void Utility::TaskManager::Clear()
{
	pImpl->Clear();
}

void Utility::TaskManager::Update()
{
	pImpl->Update();
}

void Utility::TaskManager::Draw()
{
	pImpl->Draw();
}

void Utility::TaskManager::DelayDraw()
{
	pImpl->DelayDraw();
}

void Utility::TaskManager::IsDelay(bool argIsDelay)
{
	pImpl->isDelay_ = argIsDelay;
}

bool Utility::TaskManager::IsDelay()
{
	return pImpl->isDelay_;
}
