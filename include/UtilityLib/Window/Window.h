/**
 *	@file	Window.h
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once
#include <memory>
#include <Windows.h>
#include <functional>

namespace Utility
{
	class Window
	{
	private:
		class Impl;
		std::unique_ptr<Impl> pImpl;
		static Window *instance_;

	public:
		Window();
		Window(LPCWSTR argWindowTitleName);
		Window(LPCWSTR argWindowTitleName, HICON argIcon);
		Window(LPCWSTR argWindowTitleName, HICON argIcon, SIZE argWindowSize);
		~Window();

	public:
		static inline Window *Get() { return instance_; }

		RECT WindowRect()const;
		HWND WindowHundle()const;
		/**
		 *	@fn		Menu
		 *	@brief	ポップアップメニューを取得
		 *	@return	メニュー
		 */
		HMENU Menu()const;
		/**
		 *	@fn			MenuCommand
		 *	@brief		ポップアップメニューのイベント登録
		 *	@param[in]	argCommand	!<	ポップアップメニュのイベント
		 */
		void MenuCommand(std::function<void(unsigned short)> argCommand);
		/**
		 *	@fn			PopupMenu
		 *	@brief		ポップアップメニューの描画
		 *	@param[in]	argPosX	!<	座標x
		 *	@param[in]	argPosY	!<	座標y
		 */
		void PopupMenu(int argPosX, int argPosY)const;

	};
};