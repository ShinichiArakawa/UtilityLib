/**
*	@ifs	ConfigManager.cpp
*	@date	2017 / 10 / 09
*	@author	Katsumi Takei
*	Copyright (c) Kastumi Takei. All rights reserved.
*/
#include "ConfigManager.h"
#include "../Encode.h"
#include "../String/String.h"
#include "../Archive/Archive.h"
#include "../Singleton/Singleton.h"

#include <fstream>
#include <sstream>
#include <assert.h>

std::string Utility::ConfigManager::dataPath_ = "";

void Utility::ConfigManager::LoadArchive(const char * argFileName, bool argIsDecode)
{
	char* binData = nullptr;
	int length = 0;
	Singleton<Archive>::Get()->Read(argFileName, &binData, &length);

	Encode decode(Key_);

	std::string buf = binData;
	if(argIsDecode)
		buf = decode(buf);

	std::string::size_type commentStart = 0;
	// '#'以降はコメントとして無視
	commentStart = buf.find('#');
	if (commentStart != std::string::size_type(-1))
		buf = buf.substr(0, commentStart);

	// 空行は無視
	if (buf.empty())
		return;

	// 行文字列を使った処理
	buf = String::DeleteSpace(buf);
	buf = String::Replace(buf, ":", " ");

	std::stringstream ss;
	ss.str(buf);

	while (true)
	{
		Constant temp;
		ss >> temp.name_;
		ss >> temp.data_;
		if (strcmp(temp.name_, "") == 0)
			break;
		data_.push_back(temp);
		checker_.insert(std::make_pair(temp.name_, dataIndex_++));
	}
}

void Utility::ConfigManager::Import(const char * argFilename, bool argIsDecode)
{
	std::string ConfigPath = (dataPath_ + argFilename);

	if (canUseArchive_) 
	{
		LoadArchive(ConfigPath.c_str(), argIsDecode);
		return;
	}

	std::ifstream ifs(ConfigPath.c_str());
	assert(ifs || ifs.is_open() || !ifs.bad() || !ifs.fail() && "file open failed...");
	Encode decode(Key_);

	while (!ifs.eof())
	{
		std::string buf;
		getline(ifs, buf);
		if (argIsDecode)
			buf = decode(buf);

		std::string::size_type commentStart = 0;
		// '#'以降はコメントとして無視
		commentStart = buf.find('#');
		if (commentStart != std::string::size_type(-1))
			buf = buf.substr(0, commentStart);

		// 空行は無視
		if (buf.empty())
			continue;

		// 行文字列を使った処理
		buf = String::DeleteSpace(buf);
		buf = String::Replace(buf, ":", " ");

		std::stringstream ss;
		ss.str(buf);

		while (true)
		{
			Constant temp;
			ss >> temp.name_;
			ss >> temp.data_;
			if (strcmp(temp.name_, "") == 0)
				break;
			data_.push_back(temp);
			checker_.insert(std::make_pair(temp.name_, dataIndex_++));
		}

	}

	ifs.close();
}

void Utility::ConfigManager::Export(const char *argFilename, bool argIsEncode)
{
	std::ofstream ofs(argFilename);
	assert(ofs && "file open failed...");

	Encode encode(Key_);

	for (auto data : data_)
	{
		std::string buf = data.name_;
		buf += " : ";
		buf += reinterpret_cast<char*>(data.data_);
		buf += "\n";
		if (argIsEncode)
			buf = encode(buf);

		ofs.write(buf.c_str(), buf.size());
	}

	ofs.close();
}


void Utility::ConfigManager::CanUseArchive(bool argCanUseArchive)
{
	canUseArchive_ = argCanUseArchive;
}


const int Utility::ConfigManager::IntData(std::string argKey)
{
	const int Res = checker_.at(argKey);
	assert(CanGetData(Res, argKey.c_str()) && "Can't found Argment key...");
	return IntData(Res);
}

void Utility::ConfigManager::DataPath(std::string argDataPath)
{
	dataPath_ = argDataPath;
}

std::string Utility::ConfigManager::DataPath()
{
	return dataPath_;
}
