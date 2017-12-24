/**
 *	@file	Sound.cpp
 *	@date	2017 / 10 / 04
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "Sound.h"
#include "OrgSound.h"
#include <fstream>
#include <vector>

#ifdef __cplusplus_cli
#pragma unmanaged
#endif // __cplusplus_cli

static Utility::OrgSound sound;

class Utility::Sound::Impl
{
public:
	Impl() = default;
	~Impl() = default;

public:
	bool Create(HWND argHwnd)
	{
		return sound_.Create(argHwnd);
	}
	
	bool Load(uint32_t argIndex,const wchar_t* argFilename)
	{
		// ファイルを開く
		std::ifstream in(argFilename, std::ios::in | std::ios::binary);

		if (!in) 
			return false;

		in.seekg(0, std::ios::end);
		auto size = in.tellg();
		in.clear();
		in.seekg(0, std::ios::beg);

		std::vector<char> buff((uint32_t)size);

		in.read(buff.data(), size);

		return sound_.CreateBuffer(buff.data(), argIndex);
	}

	bool Load(uint32_t argIndex, void *argBuffer)
	{
		return sound_.CreateBuffer(argBuffer, argIndex);
	}
public:
	Utility::OrgSound sound_;

};

Utility::Sound::Sound() 
	: pImpl(new Impl())
{
}

Utility::Sound::~Sound() = default;

bool Utility::Sound::Load(uint32_t argIndex,const wchar_t* argFilename)
{
	return pImpl->Load(argIndex, argFilename);
}
bool Utility::Sound::Load(uint32_t argIndex, void *argBuffer)
{
	return pImpl->sound_.CreateBuffer(argBuffer, argIndex);
}
void Utility::Sound::Play(uint32_t argIndex)
{
	pImpl->sound_.Play(argIndex, 0, false);
}
void Utility::Sound::PlayLoop(uint32_t argIndex)
{
	pImpl->sound_.Play(argIndex, 0, true);
}
void Utility::Sound::Stop(uint32_t argIndex)
{
	pImpl->sound_.Stop(argIndex);
}
bool Utility::Sound::Create(HWND argHwnd)
{
	return pImpl->Create(argHwnd);
}
void Utility::Sound::Release()
{
	pImpl->sound_.Release();
}

bool Utility::Sound::LoadS(uint32_t argIndex,const char* argFilename)
{
	FILE* fp = nullptr;
	if (fopen_s(&fp, argFilename, "rb") != 0) 
		return false;

	if (!fp) 
		return false;

	// ファイルサイズを調べる
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	// ファイルサイズと同じバッファを確保する
	void* buffer = malloc(size);
	if (!buffer)
	{
		fclose(fp);
		return false;
	}

	if (fread(buffer, size, 1, fp) != 1)
	{
		fclose(fp);
		free(buffer);
		return false;
	}

	// バッファを元に、サウンドバッファを作成する
	bool res = sound.CreateBuffer(buffer, argIndex);

	fclose(fp);
	free(buffer);

	return res;
}

bool Utility::Sound::LoadS(uint32_t argIndex, void *argBuffer)
{
	return sound.CreateBuffer(argBuffer, argIndex);
}

void Utility::Sound::PlayS(int argIndex)
{
	sound.Play(argIndex);
}

void Utility::Sound::PlayLoopS(int argIndex)
{
	sound.Play(argIndex, 0, true);
}

void Utility::Sound::StopS(int argIndex)
{
	sound.Stop(argIndex);
}

void Utility::Sound::CreateS(HWND argHwnd)
{
	sound.Create(argHwnd);
}

void Utility::Sound::ReleaseS()
{
	sound.Release();
}