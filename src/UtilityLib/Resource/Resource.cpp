/**
 *	@file	Resource.h
 *	@date	2017 / 10 / 21
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include <iostream>
#include <assert.h>
#include <sstream>
#include <fstream>
#include <ostream>
#include <time.h>

#include "Resource.h"
#include "../String/String.h"

void Utility::Resource::ExportByteData(std::string argPath, std::string argFilename, std::string argExtention)
{
	std::string in = argPath + argFilename + argExtention;
	std::string out = argPath + argFilename + ".h";
	//	時間取得
	time_t now = time(NULL);
	tm pnow;
	localtime_s(&pnow, &now);

	//	ヘッダー
	std::ostringstream header;
	header << "/**" << std::endl;
	header << " *   @file   " + argFilename + ".h" << std::endl;
	header << " *   @brief   " << "このファイルはツールによって作成されました" << std::endl;
	header << " *   @date   " << pnow.tm_year + 1900 << "/" << pnow.tm_mon + 1 << "/" << pnow.tm_mday << std::endl;
	header << " */" << std::endl;
	std::string inc = "#pragma once";

	std::ostringstream oss;
	oss << header.str() << inc << std::endl << std::endl;
	oss << "static unsigned char BinData" + argFilename + "[] = {" << std::endl;
	unsigned long binSize = 0;

	//	読み込み
	std::ifstream ifs(in, std::ios::in | std::ios::binary);
	while (!ifs.eof())
	{
		unsigned char c;
		(ifs.read((char*)&c, 1));

		oss << "0x" << std::hex << std::uppercase << (int)c << ",";

		++binSize;
		if (binSize % 10 == 0)
		{
			oss << std::endl;
		}
	}
	ifs.close();

	oss << "};" << std::endl;
	//	サイズ取得
	std::ostringstream ossSize;
	ossSize << std::endl << "const int BinSize" + argFilename + " = " << binSize - 1 << ";";

	//	書き出し
	std::ofstream os(out, std::ios::out);
	os.write(oss.str().c_str(), oss.str().size());
	os.write(ossSize.str().c_str(), ossSize.str().size());
	os.close();
}

void Utility::Resource::ExportBinary(std::string argExportPath, std::wstring argSrc)
{
	std::string dest = String::ToString(argSrc);

	std::ofstream os(argExportPath, std::ios::binary);
	os.write(dest.data(), dest.size());
	os.close();
}