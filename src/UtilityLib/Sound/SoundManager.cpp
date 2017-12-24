/**
 *	@file	SoundManager.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "SoundManager.h"
#include "Sound.h"
#include "../Window/Window.h"
#include "../Archive/Archive.h"
#include "../Singleton/Singleton.h"

#include <map>
#include <assert.h>

class Utility::SoundManager::Impl
{
public:
	Impl() 
		: index_(0), currentPlayBgm_(""), canUseArchive_(false)
	{
	}
	~Impl()
	{
		StopCurrentBgm();
		sound_.clear();
	};
public:
	void AddAndLoadSound(std::string argFilename)
	{
		sound_.insert(std::map<std::string, int>::value_type(argFilename, index_));
		
		const std::string SoundName = soundPath_ + argFilename + ".wav";
		(canUseArchive_) ? LoadSound(SoundName.c_str()) : Sound::LoadS(index_, SoundName.c_str());
		index_++;
	}

	void LoadSound(const char *argFileName)
	{
		char* binData = nullptr;
		int length = 0;
		Singleton<Archive>::Get()->Read(argFileName, &binData, &length);

		AddAndLoadSound(argFileName, binData);
	}


	void AddAndLoadSound(std::string argFilename, void * argBuffer)
	{
		sound_.insert(std::map<std::string, int>::value_type(argFilename, index_));
		Sound::LoadS(index_, argBuffer);
		index_++;
	}

	void PlaySounds(std::string argKey, eSoundType argType)
	{
		if (argType == eSoundType::Loop) 
		{
			if (currentPlayBgm_ != "")
			{
				StopCurrentBgm();
			}
			Sound::PlayLoopS(sound_.at(argKey));
			currentPlayBgm_ = argKey;
		}
		else
		{
			Sound::PlayS(sound_.at(argKey));
		}
	}

	void StopCurrentBgm()
	{
		if (currentPlayBgm_.empty())
			return;
		Sound::StopS(sound_.at(currentPlayBgm_));
		currentPlayBgm_.clear();
	}

	std::string CurrentPlayBgm()
	{
		return currentPlayBgm_;
	}

public:
	int							index_;
	std::map<std::string, int>	sound_;
	std::string					currentPlayBgm_;
	std::string					soundPath_;
	bool						canUseArchive_;

};

Utility::SoundManager::SoundManager()
	:pImpl(new Impl())
{
	Sound::CreateS(Utility::Window::Get()->WindowHundle());
}

Utility::SoundManager::~SoundManager()
{
	Sound::ReleaseS();
}
/**
 *	@fn			AddAndLoadSound
 *	@brief		サウンドの読み込み
 *	@param[in]	argBuffer	読み込むファイルのバイナリデータ
 */
void Utility::SoundManager::AddAndLoadSound(std::string argFilename, void * argBuffer)
{
	pImpl->AddAndLoadSound(argFilename, argBuffer);
}

/**
 *	@fn			AddAndLoadSound
 *	@brief		サウンドの読み込み
 *	@param[in]	argFilename	読み込むファイルの名前
 */
void Utility::SoundManager::AddAndLoadSound(std::string argFilename)
{
	pImpl->AddAndLoadSound(argFilename);
}
/**
 *	@fn			PlaySounds
 *	@brief		サウンドの再生
 *	@param[in]	argKey	再生したいサウンドのキー
 *	@param[in]	argType	再生形式
 */
void Utility::SoundManager::PlaySounds(std::string argKey, eSoundType argType)const
{
	pImpl->PlaySounds(argKey, argType);
}
/**
 *	@fn			StopSounds
 *	@brief		サウンドの停止
 *	@param[in]	argKey	停止したいサウンドのキー
 */
void Utility::SoundManager::StopSounds(std::string argKey)const
{
	Sound::StopS(pImpl->sound_.at(argKey));
}
/**
 *	@fn			StopCurrentBgm
 *	@brief		現在再生しているサウンドの停止
 */
void Utility::SoundManager::StopCurrentBgm()const
{
	pImpl->StopCurrentBgm();
}

void Utility::SoundManager::SoundPath(std::string argPath) const
{
	pImpl->soundPath_ = argPath;
}

std::string Utility::SoundManager::SoundPath() const
{
	return pImpl->soundPath_;
}

void Utility::SoundManager::CanUseArchive(bool argCanUseArchive)
{
	pImpl->canUseArchive_ = argCanUseArchive;
}

std::string Utility::SoundManager::CurrentPlayBgm() const
{
	return pImpl->CurrentPlayBgm();
}


