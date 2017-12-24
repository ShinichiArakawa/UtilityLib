/**
 *	@file	Singleton.cpp
 *	@brief	mozc式Singleton
 *	@date	2017 / 7 / 05
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 *	@note	http://qiita.com/kikuuuty/items/fcf5f7df2f0493c437dc
 */
#include "Singleton.h"
#include <cstdlib>

namespace
{
	const size_t MaxFinalizerNum = 256;
	size_t finalizerSize = 0;

	Utility::SingletonFinalizer::FinalizerFunc finalizers[MaxFinalizerNum];
}

void Utility::SingletonFinalizer::AddFinalizer(FinalizerFunc finalizer)
{
	if (finalizerSize >= MaxFinalizerNum)
	{
		exit(EXIT_FAILURE);
	}

	finalizers[finalizerSize] = finalizer;
	finalizerSize++;
}

void Utility::SingletonFinalizer::Finalize()
{
	for (size_t i = finalizerSize; 0 < i; i--)
	{//	生成と逆順に開放するため引き算
		(finalizers[i - 1])();
	}
	finalizerSize = 0;
}

