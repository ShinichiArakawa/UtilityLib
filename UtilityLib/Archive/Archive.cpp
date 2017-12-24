/**
 *	@file	SetArchive.cpp
 *	@date	2017 / 10 / 04
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "Archive.h"
#include <windows.h>
#include <vector>
#include <iostream>
#include <assert.h>

#include "../GraphicManager/TextureImage/TextureImage.h"
#include "../Sound/SoundManager.h"
#include "../Singleton/Singleton.h"

namespace 
{
	int GetInt(std::ifstream* argStream) 
	{
		unsigned char buffer[4];
		argStream->read(reinterpret_cast< char* >(buffer), 4);
		int r = buffer[0];
		r |= (buffer[1] << 8);
		r |= (buffer[2] << 16);
		r |= (buffer[3] << 24);
		return r;
	}


	//int書き込みの関数
	void Write(std::ofstream* argStream, int argInt)
	{
		char str[4];
		str[0] = static_cast<char>((argInt & 0x000000ff) >> 0);
		str[1] = static_cast<char>((argInt & 0x0000ff00) >> 8);
		str[2] = static_cast<char>((argInt & 0x00ff0000) >> 16);
		str[3] = static_cast<char>((argInt & 0xff000000) >> 24);
		argStream->write(str, 4);
	}

	void EnumerateFiles(std::vector<std::string>* argFileNames, const std::string& argDirectoryName)
	{
		auto searchPath = argDirectoryName + "/*.*";
		WIN32_FIND_DATAA fileData;

		//	ファイルのみ書き出す
		HANDLE iterator = FindFirstFileA(searchPath.c_str(), &fileData);
		while (true)
		{
			auto name = fileData.cFileName;
			if (!(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{//ディレクトリは無視
				auto filename = argDirectoryName + '/' + name;
				argFileNames->push_back(filename);
			}
			if (!FindNextFileA(iterator, &fileData))
			{ //次へ。次がなければ抜ける
				break;
			}
		}

		//	ディレクトリのみ書き出す
		iterator = FindFirstFileA(searchPath.c_str(), &fileData);
		while (true)
		{
			std::string name = fileData.cFileName;
			if ((name != ".") && (name != ".."))
			{//	.とか..とかも返すので除外する。でないと無限ループになる

				if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					auto newDirectoryName = argDirectoryName + '/' + name;
					//	再帰呼び出し
					EnumerateFiles(argFileNames, newDirectoryName.c_str());
				}
			}
			if (!FindNextFileA(iterator, &fileData))
			{ //次へ。次がなければ抜ける。
				break;
			}
		}
	}


	void CreateArchive(const std::vector<std::string> argFileNames, int argFileCount, const char* argArchiveName)
	{
		std::ofstream out(argArchiveName, std::ofstream::binary);
		int* fileSizes = new int[argFileCount];

		for (int lFileIndex = 0; lFileIndex < argFileCount; ++lFileIndex)
		{//	ファイル数分バイナリで書き出す
			std::ifstream in(argFileNames[lFileIndex].c_str(), std::ifstream::binary);
			in.seekg(0, std::ifstream::end);
			fileSizes[lFileIndex] = static_cast<int>(in.tellg());
			in.seekg(0, std::ifstream::beg);
			char* data = new char[fileSizes[lFileIndex]];
			in.read(data, fileSizes[lFileIndex]);
			out.write(data, fileSizes[lFileIndex]);
			delete[] data;
		}
		// ファイルの末尾の位置を記録
		int dataEnd = static_cast<int>(out.tellp());
		Write(&out, argFileCount);
		// 表を作るためにサイズをオフセットに直しながら場所、サイズ、名前の文字数、名前を格納する
		int pos = 0;
		for (int lFileIndex = 0; lFileIndex < argFileCount; ++lFileIndex)
		{
			Write(&out, pos);
			Write(&out, fileSizes[lFileIndex]);
			std::string name = argFileNames[lFileIndex].c_str();
			int nameLength = static_cast<int>(name.size());
			Write(&out, nameLength);
			out.write(name.c_str(), nameLength);
			pos += fileSizes[lFileIndex];
		}
		Write(&out, dataEnd);
		delete[] fileSizes;
	}
}

Utility::Archive::Archive()
{
	entries_.clear();
}

Utility::Archive::~Archive()
{
	for (auto lDelete : deleteList_)
	{
		delete[] lDelete;
	}
}

Utility::Archive::Archive(const char* argArchiveName)
{
	entries_.clear();
	Import(argArchiveName);
}

void Utility::Archive::Read(const char *argFileName, char** argFileData, int *argFileSize)
{
	auto it = entries_.find(argFileName);
	assert(it != entries_.end() && "Argment filename Don't found...");

	const Entry& e = it->second;
	*argFileData = new char[e.Size];
	*argFileSize = e.Size;
	stream_->seekg(e.Position, std::ifstream::beg);
	stream_->read(*argFileData, e.Size);
	deleteList_.push_back(*argFileData);

}

const char *Utility::Archive::LoadText(const char * argFileName)
{
	char* binData = nullptr;
	int length = 0;
	Read(argFileName, &binData, &length);

	return binData;
}

void Utility::Archive::Import(const char * argArchiveName)
{
	if (stream_)
		return;

	stream_ = std::make_unique<std::ifstream>(argArchiveName, std::ifstream::binary);
	assert(stream_ && "open stream failed...");

	//末尾から4バイト前へ移動
	stream_->seekg(-4, std::ifstream::end);
	//4バイト読み込んでintを返す
	int tableBegin = GetInt(stream_.get());
	//テーブル先頭へ移動
	stream_->seekg(tableBegin, std::ifstream::beg);
	//4バイト読むとファイル数
	int fileNumber_ = GetInt(stream_.get());

	for (int lFileIndex = 0; lFileIndex < fileNumber_; ++lFileIndex)
	{
		Entry e;
		e.Position = GetInt(stream_.get());
		e.Size = GetInt(stream_.get());
		int nameLength = GetInt(stream_.get());
		char* name = new char[nameLength + 1]; //終端NULLで+1
		stream_->read(name, nameLength);
		name[nameLength] = '\0';
		entries_.insert(std::make_pair(name, e));
		delete[] name;
	}
}

void Utility::Archive::Export(const std::string &argDirectoryName, const char* argArchiveName)
{
	std::vector<std::string> fileList;
	EnumerateFiles(&fileList, argDirectoryName);
	CreateArchive(fileList, fileList.size(), argArchiveName);
}

bool Utility::Archive::IsImported() const
{
	return (stream_ != nullptr);
}
