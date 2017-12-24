/**
 *	@file	File.h
 *	@brief	ローダーが読み込んだデータの保管
 *	@date	2017 / 7 / 13
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <string>

namespace Utility
{
	class Loader;

	class File final
	{
	private:
		File(const char *filename);

		friend Loader;

		std::string filename_;
		char *data_;
		wchar_t *wData_;
		int size_;

	public:

		~File();
		bool IsReady() const;
		bool IsReadyW() const;
		int Size() const;
		const char *Data() const;
		const wchar_t *DataW() const;

	};

};