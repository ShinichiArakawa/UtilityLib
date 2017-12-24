/**
 *	@file   Graphic3DBase.cpp
 *	@date   2017 / 08 / 15
 *	@author Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "Graphic3DBase.h"
#include "../DeviceResources/DeviceResources.h"
#include "../Function.h"

class Utility::Graphic3DBase::Impl
{
public:

	Impl()
		:nextLoadModelName_("")
	{
	}

	~Impl() = default;

#pragma region Extension
public:

	/**
	 *	@fn			Pmd
	 *	@brief		Pmdモデルの読み込み
	 */
	void Pmd()
	{
#ifdef PMD_PARSER_H
	
		assert(nextLoadModelName_ != "" && "拡張子をつけ忘れています");
		const auto PmdName = Constant::ModelPath + nextLoadModelName_;
		auto mmdModel = PMDFileParser::Load(PmdName.c_str());
		mmdModels_.insert(std::map<const std::string, SPtr<PmdModelData::PmdModelDataList>>::value_type(nextLoadModelName_, mmdModel));
		nextLoadModelName_ = "";
#endif	//	PMD_PARSER_H

	}

#pragma endregion Extension
public:
	std::string										nextLoadModelName_;		//	!<	次にロードするモデル名

private:

#ifdef PMD_PARSER_H

	std::map<const std::string,
		SPtr<PmdModelData::PmdModelDataList>>		mmdModels_;				//	!<	モデルキャッシュ
#endif	//	PMD_PARSER_H


};//	class Graphic3DBase::Impl

Utility::Graphic3DBase::Graphic3DBase()
	:pImpl(std::make_unique<Impl>())
{
}

Utility::Graphic3DBase::~Graphic3DBase() = default;

void Utility::Graphic3DBase::Pmd()
{
	pImpl->Pmd();
}

void Utility::Graphic3DBase::Fbx()
{
}

/**
 *	@fn			AddAndLoadModel
 *	@brief		モデルキャッシュにモデルを登録する
 *	@param[in]	argModelFilename	!<	モデルのパス
 *	@return		3Dグラフィックスのマネージャー
 */
Utility::Graphic3DBase *Utility::Graphic3DBase::AddAndLoadModel(const std::string argModelFilename)
{
	assert(pImpl->nextLoadModelName_ == "" && "拡張子をつけ忘れています");
	pImpl->nextLoadModelName_ = argModelFilename;

	return this;
}
