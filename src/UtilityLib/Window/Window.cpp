/**
 *	@file	Window.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "Window.h"
#include "../Debug/Debug.h"

#include <assert.h>

namespace
{//	TODO	:	デフォルトのウィンドウサイズを動的に決められるようにする
	static const SIZE MiniSize = { 640, 480 };
	static const SIZE MediumSize = { 960, 720 };
	static const SIZE LargeSize = { 1280, 960 };
}

Utility::Window *Utility::Window::instance_ = nullptr;

class Utility::Window::Impl
{
public:

	Impl(LPCWSTR argWindowTitleName, HICON argIcon, SIZE argWindowSize)
		:hInstance_(GetModuleHandle(nullptr)), hWnd_(nullptr), windowRect_(RECT()),
		command_(nullptr), windowName_(argWindowTitleName)
	{
		if (FAILED(Create(argWindowTitleName, argIcon, argWindowSize)))
			exit(EXIT_FAILURE);

		STARTUPINFO si;
		si.dwFlags = 0;
		GetStartupInfo(&si);
		const int CmdShow = (si.dwFlags & STARTF_USESHOWWINDOW) ? si.wShowWindow : SW_SHOWDEFAULT;

		ShowWindow(hWnd_, CmdShow);
	}

	~Impl()
	{
		BOOL result;
		result = DestroyWindow(hWnd_);
		result = UnregisterClass(windowName_, hInstance_);
	}
private:

	/**
	 *	@brief		ウインドウの登録
	 *	@param[in]	argIcon		!<	アイコン
	 *	@retval		S_OK		!<	成功
	 *	@retval		E_FAIL		!<	失敗
	 */
	HRESULT Register(HICON argIcon)
	{
		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = CallBackWndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance_;
		wcex.hIcon = (argIcon) ? argIcon : LoadIcon(hInstance_, MAKEINTRESOURCE(NULL));
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = windowName_;
		wcex.hIconSm = (argIcon) ? argIcon : LoadIcon(hInstance_, MAKEINTRESOURCE(NULL));
		
		if (!RegisterClassEx(&wcex))
			return E_FAIL;
		
		return S_OK;
	}

	/**
	 *	@brief	ウインドウの作成
	 *	@retval	S_OK	!<	成功または作成済み
	 *	@retval	E_FAIL	!<	失敗
	 */
	HRESULT Create(LPCWSTR argWindowTitleName, HICON argIcon, SIZE argWindowSize)
	{
		if (hWnd_)
			return S_OK;

		if (FAILED(Register(argIcon)))
			return E_FAIL;

		return Create(argWindowTitleName, argWindowSize);
	}

	HRESULT Create(LPCWSTR argWindowTitleName,  SIZE argWindowSize)
	{
		const unsigned long WindowStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
		windowRect_ = { 0, 0, argWindowSize.cx, argWindowSize.cy };

		AdjustWindowRect(&windowRect_, WindowStyle, FALSE);

		// ウインドウを作成する
		//	@note	!<	https://msdn.microsoft.com/ja-jp/library/cc410714.aspx
		hWnd_ = CreateWindowEx(NULL, windowName_, argWindowTitleName, WindowStyle, CW_USEDEFAULT, CW_USEDEFAULT,
			windowRect_.right - windowRect_.left, windowRect_.bottom - windowRect_.top,
			nullptr, nullptr, hInstance_, this);

		if (!hWnd_)
			return E_FAIL;

		return S_OK;
	}


	/**
	 *	@fn			CallBackWndProc
	 *	@brief		コールバック用ウインドウプロシージャー
	 *	@prame[in]	hWnd	!<	ウィンドウハンドル
	 *	@param[in]	message	!<	ウィンドウメッセージ
	 *	@param[in]	wParam	!<	16bitメッセージパラメータ
	 *	@param[in]	lParam	!<	32bitメッセージパラメータ
	 *	@return		ウィンドウメッセージ
	 *	@note		http://miffysora.blogspot.jp/2013/04/blog-post_10.html
	 */
	static LRESULT CALLBACK CallBackWndProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam)
	{
		auto impl = reinterpret_cast<Impl*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

		if (impl)
		{
			return impl->LoadWndProc(hWnd, message, wParam, lParam);
		}
		else
		{
			if (message == WM_CREATE)
			{
				impl = reinterpret_cast<Impl*>(reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams);
				Utility::Debug::ChackLastErrorBegin();
				SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<unsigned long>(impl));
				Utility::Debug::ChackLastErrorEnd();
				impl->CreatePopupMenu(hWnd, lParam);
			}
		}
		return DefWindowProc(hWnd, message, wParam, lParam); 
	}
	/**
	 *	@fn			LoadWndProc
	 *	@brief		ウインドウプロシージャー本体
	 *	@prame[in]	hWnd	!<	ウィンドウハンドル
	 *	@param[in]	message	!<	ウィンドウメッセージ
	 *	@param[in]	wParam	!<	16bitメッセージパラメータ
	 *	@param[in]	lParam	!<	32bitメッセージパラメータ
	 *	@return		ウィンドウメッセージ
	 *	@note		http://www-higashi.ist.osaka-u.ac.jp/~k-maeda/vcpp/sec1-4wndproc.html
	 */
	LRESULT CALLBACK LoadWndProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_DESTROY:	// ウインドウが破棄されたメッセージ
			if(dammy_)
				DestroyMenu(dammy_);
			if(hmenu_)
				DestroyMenu(hmenu_);
			PostQuitMessage(EXIT_SUCCESS);
			break;
		case WM_PAINT:		//	描画
			PAINTSTRUCT ps;
			HDC hdc;
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;
		case WM_COMMAND:	//	ポップアップメニューの処理
			if(command_)
				command_(LOWORD(wParam));
			break;
		default:			// 自分で処理しないメッセージはDefWindowProc関数に任せる
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		
		return 0;
	}
	/**
	 *	@fn			CreatePopupMenu
	 *	@brief		ポップアップメニューの作成
	 *	@param[in]	hWnd	!<	生成されたウィンドウハンドル
	 *	@param[in]	lParam	!<	(LPCREATESTRUCT) CREATESTRUCT構造体のポインタ。CreateWindow(Ex)の最後の引数。MDIとかで使われる
	 */
	void CreatePopupMenu(HWND hWnd, LPARAM lParam)
	{
#ifdef IDR_MENU1
		//	メニューの作成
		dammy_ = LoadMenu(reinterpret_cast<LPCREATESTRUCT>(lParam)->hInstance, MAKEINTRESOURCE(IDR_MENU1));
		assert(dammy_ && "Create Menu Filed...");
		hmenu_ = GetSubMenu(dammy_, 0);
		popupMenu_ = std::bind(&TrackPopupMenu, hmenu_, TPM_LEFTALIGN | TPM_BOTTOMALIGN, std::placeholders::_1, std::placeholders::_2, 0, hWnd, nullptr);
#else
		OutputDebugString(L"Menu Resources Can't Found...\n");
#endif
	}
	
private:
	HINSTANCE hInstance_;
	LPCWSTR windowName_;
public:
	HWND hWnd_;
	RECT windowRect_;
	std::function<void(unsigned short)> command_;	//	!<	メニューの動作
	std::function<void(int, int)> popupMenu_;		//	!<	TrackPopupMenu
	HMENU dammy_, hmenu_;							//	!<	メニュー構造体
	
};

Utility::Window::Window()
	:pImpl(std::make_unique<Impl>(L"Default Window", nullptr, MiniSize))
{
	instance_ = this;
}

Utility::Window::Window(LPCWSTR argWindowTitleName)
	:pImpl(std::make_unique<Impl>(argWindowTitleName, nullptr, MiniSize))
{
	instance_ = this;
}

Utility::Window::Window(LPCWSTR argWindowTitleName, HICON argIcon)
	: pImpl(std::make_unique<Impl>(argWindowTitleName, argIcon, MiniSize))
{
	instance_ = this;
}

Utility::Window::Window(LPCWSTR argWindowTitleName, HICON argIcon, SIZE argWindowSize)
	: pImpl(std::make_unique<Impl>(argWindowTitleName, argIcon, argWindowSize))
{
	instance_ = this;
}

Utility::Window::~Window() = default;

RECT Utility::Window::WindowRect()const
{ 
	return pImpl->windowRect_; 
}
HWND Utility::Window::WindowHundle()const
{ 
	return pImpl->hWnd_;
}
/**
 *	@fn		Menu
 *	@brief	ポップアップメニューを取得
 *	@return	メニュー
 */
HMENU Utility::Window::Menu() const
{
	return pImpl->hmenu_;
}
/**
 *	@fn			MenuCommand
 *	@brief		ポップアップメニューのイベント登録
 *	@param[in]	argCommand	!<	ポップアップメニュのイベント
 */
void Utility::Window::MenuCommand(std::function<void(unsigned short)> argCommand)
{
	pImpl->command_ = argCommand;
}
/**
 *	@fn			PopupMenu
 *	@brief		ポップアップメニューの描画
 *	@param[in]	argPosX	!<	座標x
 *	@param[in]	argPosY	!<	座標y
 */
void Utility::Window::PopupMenu(int argPosX, int argPosY)const
{
#ifdef IDR_MENU1
	pImpl->popupMenu_(argPosX, argPosY);
#else
#endif
}
