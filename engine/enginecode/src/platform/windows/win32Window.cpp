/** \file win32Window.cpp */

#include "engine_pch.h"
#include "platform/windows/win32Window.h"
#include "systems/log.h"

namespace Engine 
{

#ifdef NG_PLATFORM_WINDOWS
	Window* Window::createWindow(const WindowProperties& properties)
	{
		//everytime we create a window, create a new win32 window.
		return new Win32Window(properties);
	}
#endif

	void Win32Window::initWindow(const WindowProperties & properties)
	{
		LPCWSTR title = L"Win32 Window, WOOP WOOP WOOP";
		int32_t startXPos = 100;
		int32_t startYPos = 100;

		//create a winow.
		m_native = CreateWindow(
			L"NGWin32Window",		//class name of the window.
			title,					//title name of the window.
			WS_OVERLAPPEDWINDOW,	//style of window, set to standard overlapped window.
			startXPos, startYPos,	//start x & y position of window on display.
			properties.width, properties.height,	//width and height of window.
			0,						//no parent window, so null.
			0,						//no menu, so null.
			GetModuleHandle(NULL),	//instance of the window.
			0
		);

		if (!m_native)
		{
			Log::error("ERROR : Win32 window NOT created.");
		}

		ShowWindow(m_native, SW_SHOW);
	}

	void Win32Window::closeWindow()
	{
		//destroy the native window on close.
		DestroyWindow(m_native);
	}

	void Win32Window::onUpdate(float timestep)
	{
		//create message.
		MSG message = {};

		//peek windows message queue
		if (PeekMessage(&message, m_native, 0, 0, PM_REMOVE))
		{
			//translate the message from windows language.
			TranslateMessage(&message);
			//then display on windows message pump.
			DispatchMessage(&message);
		}
	}

	LRESULT Win32Window::onWin32Message(HWND hWin, UINT msg, WPARAM wParam, LPARAM lParam)
	{

		return LRESULT();
	}

}