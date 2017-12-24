/**
 *	@file	InputManager.h
 *	@date	2017 / 8 / 31
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#if 0

#include <wrl/client.h>

struct IWICStream;

class AutoDeleteFileWic
{
public:
	AutoDeleteFileWic(Microsoft::WRL::ComPtr<IWICStream>& hFile, const wchar_t* szFile) 
		: handle_(hFile), filename_(szFile) 
	{}
	~AutoDeleteFileWic()
	{
		if (filename_)
		{
			handle_.Reset();
			DeleteFileW(filename_);
		}
	}

	void clear() { filename_ = 0; }

private:
	const wchar_t* filename_;
	Microsoft::WRL::ComPtr<IWICStream>& handle_;

	AutoDeleteFileWic(const AutoDeleteFileWic&) = delete;
	AutoDeleteFileWic& operator=(const AutoDeleteFileWic&) = delete;
};

extern HRESULT ScreenShot();
#endif