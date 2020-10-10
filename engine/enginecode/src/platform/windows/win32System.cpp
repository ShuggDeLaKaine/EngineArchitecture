/** \file win32System.cpp */

#include "engine_pch.h"
#include "platform/windows/win32System.h"
#include "platform/windows/win32Window.h"
#include "core/application.h"

namespace Engine
{

	void Win32System::start(SystemSignal init, ...)
	{
		WNDCLASSEX wndClass;

		//reset to 0
		memset(&wndClass, 0, sizeof(WNDCLASSEX));

		//tell how big it is
		wndClass.cbSize = sizeof(WNDCLASSEX);
		//tell to re-draw window when resized horizontally and vertically.
		wndClass.style = CS_HREDRAW | CS_VREDRAW;
		//tell what handle to instance it is.
		wndClass.hInstance = GetModuleHandle(NULL);
		//tell what icon (and small icon) when window is running; in this case set to default, can load own icon from own resource file.
		wndClass.hIcon = LoadIcon(0, IDI_APPLICATION);
		wndClass.hIconSm = LoadIcon(0, IDI_APPLICATION);
		//tell it what cursor icon to use.
		wndClass.hCursor = LoadCursor(0, IDC_ARROW);
		//give it a name, can refer to this name (needs to be unique!).
		wndClass.lpszClassName = L"NGWin32Window";

		//need to give it a function when it recieves a message (window message pump).
		//using lambda function to do this inline here; normally usings global functions but no no no globals.
		wndClass.lpfnWndProc = [](HWND hWin, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT CALLBACK
		{
			//***NOTE*** - if more than one window, will need to alter this to take that into account; notes in application.h
			auto& winPtr = Application::getInstance().getWindow();
			if (winPtr)
			{
				auto win = static_cast<Win32Window*>(winPtr.get());
				return win->onWin32Message(hWin, msg, wParam, lParam);
			}
			else
			{
				return DefWindowProc(hWin, msg, wParam, lParam);
			}
		};

		if (!RegisterClassEx(&wndClass))
		{
			Log::error("ERROR! Could not register win32 class");
		}

	}

	void Win32System::stop(SystemSignal close, ...)
	{
		if (!UnregisterClass(L"NGWin32Window", GetModuleHandle(NULL)))
		{
			Log::error("ERROR! Could not UNregister win32 class");
		}

	}

}