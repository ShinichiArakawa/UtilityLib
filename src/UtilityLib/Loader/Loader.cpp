/**
 *	@file	Loader.cpp
 *	@date	2017 / 10 / 09
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "Loader.h"
#include "File.h"
#include "../Function.h"

#include <codecvt>
#include <fstream>
#include <assert.h>

Utility::Loader::Loader() = default;

Utility::Loader::~Loader()
{
	for (auto i : files_)
	{
		Utility::SafeDelete(i);
	}
}

#pragma region PrivateFunction

bool Utility::Loader::Load(std::istream *argStream, size_t argSize)
{
	if (!argStream)
		return false;

	buffer_.resize(argSize);

	argStream->read(buffer_.data(), buffer_.size());

	return true;
}

size_t Utility::Loader::FileSize(std::istream* argStream)const
{
	argStream->seekg(0, std::ios::end);
	size_t size = static_cast<size_t>(argStream->tellg());
	argStream->seekg(0, std::ios::beg);

	return size;
}
#pragma endregion
#pragma region PublicFunction
void Utility::Loader::SetFile(File** argFile, const char *argFileName)
{
	assert(!(*argFile) && "pointer must be NULL.");
	*argFile = new File(argFileName);
	files_.push_back(*argFile);
}

void Utility::Loader::DestroyFile(File** argFile)
{
	if (!(*argFile)) 
		return;

	for (auto i = files_.begin(); i != files_.end(); ++i)
	{
		if (*i == *argFile)
		{// ものが見つかる
		 // ものとリストの場所を消す
			Utility::SafeDelete(*argFile);
			files_.erase(i);
			*argFile = 0;
			break;
		}
	}
	assert(!(*argFile) && "can't find!");
}


void Utility::Loader::LoadAtThread()
{
	//	TODO	:	スレッド未対応（＾０＾）
	for(auto file : files_)
	{
		if (!file->IsReady())
		{
			std::ifstream in(file->filename_.c_str(), std::ifstream::binary);
			in.seekg(0, std::ios::end);
			file->size_ = static_cast<int>(in.tellg());
			assert(file->size_ >= 0 && "File Search Failed...");
			in.seekg(0, std::ios::beg);
			file->data_ = new char[file->size_];
			in.read(file->data_, file->size_);
		}
	}
}

void Utility::Loader::LoadAtThreadUTF()
{
	//	TODO	:	スレッド未対応（＾０＾）
	for (auto file : files_)
	{
		if (!file->IsReadyW())
		{
			std::wifstream in(file->filename_.c_str());
			//	入力ストリームのロケールをUTF-8に変更する
			in.imbue(std::locale(in.getloc(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>()));

			in.seekg(0, std::ios::end);
			file->size_ = static_cast<int>(in.tellg());
			assert(file->size_ >= 0 && "File Search Failed...");
			in.seekg(0, std::ios::beg);
			file->wData_ = new wchar_t[file->size_];
			memset(file->wData_, 0x00, file->size_ * 2);
			in.read(file->wData_, file->size_);
		}
	}
}

bool Utility::Loader::Load(const char* argFileName)
{
	auto fs = std::ifstream(argFileName, std::ios::in | std::ios::binary);

	return Load(&fs, FileSize(&fs));
}

bool Utility::Loader::Load(const wchar_t* argFileName)
{
	auto fs = std::ifstream(argFileName, std::ios::in | std::ios::binary);

	return Load(&fs, FileSize(&fs));
}


const char* Utility::Loader::Data()const
{
	return (buffer_.empty()) ? nullptr : buffer_.data();
}

size_t Utility::Loader::Size()const
{
	return buffer_.size();
}
#pragma endregion