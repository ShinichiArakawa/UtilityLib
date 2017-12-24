/**
 *	@file	File.cpp
 *	@brief	ローダーが読み込んだデータの保管
 *	@date	2017 / 7 / 13
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "File.h"
#include "../Function.h"

#include <assert.h>

Utility::File::File(const char *filename)
: filename_(filename),data_(0),size_(0),wData_(0)
{
}

Utility::File::~File()
{
	if (data_)
	{
		Utility::SafeDeleteArray(data_);
	}
	if (wData_)
	{
		Utility::SafeDeleteArray(wData_);
	}
	size_ = 0;
}

bool Utility::File::IsReady()const
{
	return (data_ != 0);
}

bool Utility::File::IsReadyW()const
{
	return (wData_ != 0);
}

int Utility::File::Size()const
{
	assert(size_ && "Loading is not finished.");
	return size_;
}

const char *Utility::File::Data()const
{
	assert(data_ && "Loading is not finished.");
	return data_;
}

const wchar_t *Utility::File::DataW()const
{
	assert(wData_ && "Loading is not finished.");
	return wData_;
}