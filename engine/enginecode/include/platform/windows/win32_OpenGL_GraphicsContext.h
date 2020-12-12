/** \file win32_OpenGL_GraphicsContext.h */
#pragma once

#include "core/graphicsContext.h"

//targeting Windows 10 only
#define WINVER 0x0A00		//64 bit version
#define _WIN32_WINNT 0x0A00	//32 bit version
#include <windows.h>

namespace Engine
{
	/*	\class win32_OpenGL_GraphicsContext
	*	\brief Win32/OpenGL specific graphics context, for double buffering.
	*/
	class Win32_OpenGL_GraphicsContxt : public GraphicsContext
	{
	public:
		Win32_OpenGL_GraphicsContxt(HWND hWin);		//!< constructor, takes a window handle for the native window.
		~Win32_OpenGL_GraphicsContxt();				//!< destructor.
		virtual void init() override;				//!< initiate the graphics context for the given windowing API.
		virtual void swapBuffers() override;		//!< to swap the front and back buffers (aka double buffering).

	private:
		HWND m_window = nullptr;					//!< window handle for this native window.
		HDC	 m_deviceContext = nullptr;				//!< device context handle.
		HGLRC m_resourceContext = nullptr;			//!< OpenGL resource context handle.
	};
}