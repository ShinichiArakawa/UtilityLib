/**
 *	@file	Macro.h
 *	@date	2017 / 8 / 21
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once
/**
 *	@def		SYNTHESIZE
 *	@brief		Cocos2d_xのCC_SYNTHESIZE
 *	@param[in]	argType		!<	プロパティの型
 *	@param[in]	argVarName	!<	プロパティの変数名
 *	@param[in]	argFuncName	!<	プロパティの関数名
 *	@note		http://brbranch.jp/blog/201308/cocos2d-x/cocos2dxmacros/
 */
#define SYNTHESIZE(argType, argVarName, argFuncName) \
protected: \
	argType argVarName; \
public: \
	virtual argType argFuncName() const { return argVarName; } \
	virtual void argFuncName(argType argValue) { argVarName = argValue; }

/**
 *	@def		SYNTHESIZE_READONRY
 *	@brief		Cocos2d_xのCC_SYNTHESIZE_READONRYの改造版
 *	@param[in]	argType		!<	プロパティの型
 *	@param[in]	argVarName	!<	プロパティの変数名
 *	@param[in]	argFuncName	!<	プロパティの関数名
 *	@note		http://brbranch.jp/blog/201308/cocos2d-x/cocos2dxmacros/
 */
#define SYNTHESIZE_READONRY(argType, argVarName, argFuncName) \
protected: \
	argType argVarName; \
protected: \
	virtual void argFuncName(argType argValue) { argVarName = argValue; } \
public: \
	virtual argType argFuncName() const { return argVarName; }


 /**
  *	@def		EXTERNAL
  *	@brief		実装隠蔽用名前空間の外部参照用、boostのdetial
  */
#define EXTERNAL namespace Impl {}; namespace
 /**
  *	@def		INTERNAL
  *	@brief		実装隠蔽用名前空間の実装部分用、boostのdetial
  */
#define INTERNAL namespace Impl