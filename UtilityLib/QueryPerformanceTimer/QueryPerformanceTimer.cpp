/**
 *	@file	QueryPerformanceTimer.cpp
 *	@date	2017 / 10 / 04
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "QueryPerformanceTimer.h"
#include <windows.h>
#include <assert.h>

static float sFrameRate = 0.0f;

class Utility::QueryPerformanceTimer::Impl
{
public:
	Impl()
	{
		Reset();
	}
	~Impl() = default;

private:
	void Check()
	{
		oldTick_ = lastTick_;
		QueryPerformanceCounter(&lastTick_);

		totalTick_.QuadPart += lastTick_.QuadPart - oldTick_.QuadPart;
		if (--checkTimeCnt_ <= 0)
		{//	フレームレートの更新
			checkTimeCnt_ = checkFrame_;
			frameRate_ = checkFrame_ / (static_cast<float>(totalTick_.QuadPart) / frequency_.QuadPart);
			totalTick_.QuadPart = 0;
			sFrameRate = frameRate_;
		}
	}
public:
	void Wait()
	{
		LARGE_INTEGER now;

		QueryPerformanceCounter(&now);

		while ((frequency_.QuadPart / static_cast<long long>(checkFrame_)) > (now.QuadPart - lastTick_.QuadPart))
		{//	現在時間の更新
			QueryPerformanceCounter(&now);
		}
		deltaTime_ = (now.QuadPart - lastTick_.QuadPart) / static_cast<float>(frequency_.QuadPart);
		Check();
	}

	void Reset(int argFrameRate = 60)
	{
		deltaTime_ = 0.0;
		BOOL canUse = QueryPerformanceFrequency(&frequency_);
		assert(canUse && "Can't use QueryPerformanceFrequency...");
		QueryPerformanceCounter(&lastTick_);
		oldTick_ = lastTick_;

		totalTick_.QuadPart = 0;
		frameRate_ = 0.0f;
		checkFrame_ = argFrameRate;
		checkTimeCnt_ = checkFrame_;
	}

private:
	LARGE_INTEGER lastTick_;
	LARGE_INTEGER oldTick_;
	LARGE_INTEGER frequency_;
	LARGE_INTEGER totalTick_;
	int checkTimeCnt_;
	int checkFrame_;
public:
	float frameRate_;
	float deltaTime_;
};

Utility::QueryPerformanceTimer::QueryPerformanceTimer()
	:pImpl(std::make_unique<Impl>())
{
}

Utility::QueryPerformanceTimer::~QueryPerformanceTimer() = default;

void Utility::QueryPerformanceTimer::Wait()
{
	pImpl->Wait();
}

void Utility::QueryPerformanceTimer::Reset(int argFrameRate)
{
	pImpl->Reset(argFrameRate);
}

float Utility::QueryPerformanceTimer::FramePerSecond() const
{
	return pImpl->frameRate_;
}

float Utility::QueryPerformanceTimer::DeltaTime() const
{
	return pImpl->deltaTime_;
}

float Utility::QueryPerformanceTimer::sFramePerSecond()
{
	return sFrameRate;
}
