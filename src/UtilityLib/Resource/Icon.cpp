/**
 *	@file   Icon.h
 *	@date   2017 / 10 / 25
 *	@author Katsumi Takei
 *	@note	https://github.com/KurzedMetal/Jaangle/blob/c2695abeab25eaa9710be4fdbc71e12f052bd7f9/CommonSources/IconSerialization.cpp
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */

#include "Icon.h"
#include "../Archive/Archive.h"

#include <assert.h>
#include <fstream>

HICON Utility::Icon::GetLargeIcon(HWND argHwnd)
{
	return reinterpret_cast<HICON>(GetClassLong(argHwnd, GCL_HICON));
}

HICON Utility::Icon::GetSmallIcon(HWND argHwnd)
{
	return reinterpret_cast<HICON>(GetClassLong(argHwnd, GCL_HICONSM));
}

HICON Utility::Icon::Load(unsigned short argIconId)
{
	return LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(argIconId));
}

HICON Utility::Icon::Load(Archive *argArchive, const char* argFileName)
{
	char* binData = nullptr;
	int length = 0;
	argArchive->Read(argFileName, &binData, &length);
	
	HICON hIcon = nullptr;
	unsigned char* data = reinterpret_cast<unsigned char*>(binData);
	int offset = LookupIconIdFromDirectoryEx(data, TRUE, 0, 0, LR_DEFAULTCOLOR);
	if (offset != 0)
	{
		hIcon = CreateIconFromResourceEx(data + offset, 0, TRUE, 0x00030000, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	}
	return hIcon;

}

HICON Utility::Icon::Load(const char *argIconDescription)
{
	const unsigned char *memory = reinterpret_cast<const unsigned char *>(argIconDescription);
	assert(memory && "memory cast failed...");

	unsigned int pos = 0;

	ICONINFO iInfo;
	memcpy(&iInfo, &memory[pos], sizeof(ICONINFO));
	pos += sizeof(ICONINFO);

	BITMAP bmpColor;
	memcpy(&bmpColor, &memory[pos], sizeof(BITMAP));
	pos += sizeof(BITMAP);

	BITMAP bmpMask;
	memcpy(&bmpMask, &memory[pos], sizeof(BITMAP));
	pos += sizeof(BITMAP);

	iInfo.hbmColor = CreateBitmap(bmpColor.bmWidth, bmpColor.bmHeight, bmpColor.bmPlanes, bmpColor.bmBitsPixel, &memory[pos]);
	assert(iInfo.hbmColor && "CreateBitmap Failed...");
	pos += bmpColor.bmWidthBytes * bmpColor.bmHeight;

	iInfo.hbmMask = CreateBitmap(bmpMask.bmWidth, bmpMask.bmHeight, bmpMask.bmPlanes, bmpMask.bmBitsPixel, &memory[pos]);
	assert(iInfo.hbmMask && "CreateBitmap Failed...");
	pos += bmpMask.bmWidthBytes * bmpMask.bmHeight;

	HICON hIcon = CreateIconIndirect(&iInfo);
	assert(hIcon && "CreateIconIndirect Failed...");

	return hIcon;
}

Utility::IconByteInfo Utility::Icon::SaveIconData(HICON argIcon)
{
	ICONINFO iInfo;
	BOOL res = GetIconInfo(argIcon, &iInfo);
	assert(res && "GetIconInfo Failed...");

	BITMAP bmpColor;
	GetObject(iInfo.hbmColor, sizeof(BITMAP), &bmpColor);

	unsigned int bmpColorBitsLen = bmpColor.bmWidthBytes * bmpColor.bmHeight;

	BITMAP bmpMask;
	GetObject(iInfo.hbmMask, sizeof(BITMAP), &bmpMask);

	unsigned int bmpMaskBitsLen = bmpMask.bmWidthBytes * bmpMask.bmHeight;
	int bfByteSize = sizeof(ICONINFO) + 2 * sizeof(BITMAP) + bmpColorBitsLen + bmpMaskBitsLen;

	unsigned char* memory = new unsigned char[bfByteSize];

	unsigned int pos = 0;

	memcpy(&memory[pos], &iInfo, sizeof(ICONINFO));
	pos += sizeof(ICONINFO);

	memcpy(&memory[pos], &bmpColor, sizeof(BITMAP));
	pos += sizeof(BITMAP);

	memcpy(&memory[pos], &bmpMask, sizeof(BITMAP));
	pos += sizeof(BITMAP);

	GetBitmapBits(iInfo.hbmColor, bmpColorBitsLen, &memory[pos]);
	pos += bmpColorBitsLen;

	GetBitmapBits(iInfo.hbmMask, bmpMaskBitsLen, &memory[pos]);
	pos += bmpMaskBitsLen;

	IconByteInfo icon = {};
	icon.Memory = reinterpret_cast<char*>(memory);
	icon.ByteSize  = bfByteSize;

	return icon;

}

void Utility::Icon::Export(const char * argExportPath, unsigned short argIconId)
{
	HICON hIcon = Load(argIconId);

	auto byteInfo = SaveIconData(hIcon);
	std::ofstream ofs(argExportPath);
	ofs.write(byteInfo.Memory, byteInfo.ByteSize);
	ofs.close();

	delete []byteInfo.Memory;
}

