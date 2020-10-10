/** \file win32Window.h */
#pragma once
#include "core/window.h"

//targeting Windows 10 only
#define WINVER 0x0A00		//64 bit version
#define _WIN32_WINNT 0x0A00	//32 bit version

#include <windows.h>
#include <tchar.h>

namespace Engine
{
	/** \class Win32Window
	*
	*/
	class Win32Window : public Window
	{
	public:
		Win32Window(const WindowProperties& properties) { initWindow(properties);}	//!< constructor.
		virtual void initWindow(const WindowProperties& properties) override;		//!< func to initiate a window.
		virtual void closeWindow() override;						//!< func to close a window.
		virtual ~Win32Window() {};									//!< deconstructor
		virtual void onUpdate(float timestep) override;				//!< func to update the window with each timestep.
		virtual void setVSync(bool VSync) override {};				//!< doesn't really work in Win32, func to set the VSync to the refresh rate of monitor.
		virtual inline unsigned int getWidth() const override { return m_properties.width; };		//!< func to get the width of the window.
		virtual inline unsigned int getHeight() const override { return m_properties.height; };		//!< func to get the height of the window.
		virtual inline void* getNativeWindow() const override { return m_native;};					//!< func to get the native window, ie the underlaying platform.
		virtual inline bool isFullScreenWindow() const override { return m_properties.isFullScreen; };			//!< func to check whether the window is in fullscreen.
		virtual inline bool isVSync() const override { return false; };	//!< doesn't really work in Win32, func to check whether VSync-ed.
		LRESULT CALLBACK onWin32Message(HWND hWin, UINT msg, WPARAM wParam, LPARAM lParam);		//!< internal win32 message prompt.
	private:
		HWND m_native;						//!< native win32 window.
		WindowProperties m_properties;		//!< properties of the window.
		float m_aspectRatio;				//!< aspect ratio of the window.
	};

}
