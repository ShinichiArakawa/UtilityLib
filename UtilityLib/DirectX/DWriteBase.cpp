/**
 *	@file   DWriteBase.cpp
 *	@date   2017 / 07 / 15
 *	@author Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "DWriteBase.h"
#include <wrl/client.h>

#pragma comment(lib,"DWrite.lib")

class Utility::DWriteBase::Impl
{
public:
	Impl()
	{
		DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(factory.GetAddressOf()));
	}
	~Impl() = default;

public:
	Microsoft::WRL::ComPtr<IDWriteFactory> factory;
};

Utility::DWriteBase::DWriteBase()
	: pImpl(std::make_unique<Impl>())
{
}

Utility::DWriteBase::~DWriteBase() = default;


IDWriteFactory *Utility::DWriteBase::ImageFactory()
{
	return pImpl->factory.Get();
}

