/** \file win32Window.cpp */

#include "engine_pch.h"
#include "platform/windows/win32Window.h"
#include "platform/windows/win32_OpenGL_GraphicsContext.h"
#include "systems/log.h"

namespace Engine 
{


	void Win32Window::initWindow(const WindowProperties & properties)
	{
		//array of wide characters.
		wchar_t wideStr[256];

		//func to convert normal character string to a wide character string.
		//below name means multibyte string to wide character string
		mbstowcs(wideStr, properties.windowTitle, 256);

		//starting position of window variables.
		int32_t startXPos, startYPos;

		//if fullscreen, then start top left.
		if (properties.isFullScreen)
		{
			startXPos = 0;
			startYPos = 0;
		}
		else
		{
			//otherwise put it in the middle.
			startXPos = GetSystemMetrics(SM_CXSCREEN) / 2 - properties.width / 2;
			startYPos = GetSystemMetrics(SM_CYSCREEN) / 2 - properties.height / 2;
		}

		//create a winow.
		m_native = CreateWindow(
			L"NGWin32Window",		//class name of the window.
			wideStr,				//title name of the window.
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

		//create the graphics context.
		m_graphicsContext.reset(new Win32_OpenGL_GraphicsContxt(m_native));

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

		//poll the message, start with peeking windows message queue, using 'Peek' will run in the background.
		if (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
		{
			//translate the message from windows language.
			TranslateMessage(&message);
			//then display on windows message pump.
			DispatchMessage(&message);
		}

		//swap the buffers.
		m_graphicsContext->swapBuffers();
	}

	LRESULT Win32Window::onWin32Message(HWND hWin, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_DESTROY : 
		{
			WindowCloseEvent event;

			auto& onClose = m_eventHandler.getOnWindowCloseCallback();
			onClose(event);
			break;
		}
		case WM_SIZE :
		{
			int32_t width = LOWORD(lParam);
			int32_t height = HIWORD(lParam);
			WindowResizeEvent event(width, height);

			auto& onResize = m_eventHandler.getOnWindowResizeCallback();
			onResize(event);
			break;
		}
		case WM_KEYDOWN :
		{
			int32_t keycode = static_cast<int32_t>(wParam);
			int32_t repeatCount = LOWORD(lParam);
			KeyPressedEvent event(keycode, repeatCount);

			auto& onKeyPress = m_eventHandler.getOnKeyPressCallback();
			onKeyPress(event);
			break;
		}
		case WM_KEYUP :
		{
			int32_t keycode = static_cast<int32_t>(wParam);
			KeyReleasedEvent event(keycode);

			auto& onKeyRelease = m_eventHandler.getOnKeyReleaseCallback();
			onKeyRelease(event);
			break;
		}
		default:
			return DefWindowProc(hWin, msg, wParam, lParam);
			break;
		}
		return 0;
	}

}