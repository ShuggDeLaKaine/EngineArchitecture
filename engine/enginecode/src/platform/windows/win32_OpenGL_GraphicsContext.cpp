/** \file win32_OpenGL_GraphicsContext.cpp */

#include "engine_pch.h"
#include "platform/windows/win32_OpenGL_GraphicsContext.h"
#include "systems/log.h"
#include <glad/glad.h>

namespace Engine
{

	Win32_OpenGL_GraphicsContxt::Win32_OpenGL_GraphicsContxt(HWND hWin) : m_window(hWin)
	{
		init();
	}

	Win32_OpenGL_GraphicsContxt::~Win32_OpenGL_GraphicsContxt()
	{
		//first make context void.
		wglMakeCurrent(0, 0);
		//then can delete that context.
		wglDeleteContext(m_resourceContext);
		//release the handle to the device context.
		ReleaseDC(m_window, m_deviceContext);
	}

	void Win32_OpenGL_GraphicsContxt::init()
	{
		//first get device context from the window.
		m_deviceContext = GetDC(m_window);

		//tell it how to render the pixels (what format we what to draw in), so how to set up its buffers.
		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

		//set the Pixel Format Descriptor up to use OpenGL settings.
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		//draw to the window using OpenGl and using Double Buffering.
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		//type of pixel is RGBA
		pfd.iPixelType = PFD_TYPE_RGBA;
		//using 32bit colour bits
		pfd.cColorBits = 32;
		//set depth buffer to a standard OpenGL 24 bits.
		pfd.cDepthBits = 24;
		//set stencil buffer to a standard OpenGL 8 bits.
		pfd.cStencilBits = 8;
		
		//tell device context above is what we want.
		int format = ChoosePixelFormat(m_deviceContext, &pfd);
		//now set this and check been set.
		if (!SetPixelFormat(m_deviceContext, format, &pfd))
		{
			Log::error("Could NOT set the pixel format");
		}

		//make the windows GL context.
		m_resourceContext = wglCreateContext(m_deviceContext);
		//make this the current context and check it has been set.
		if (!wglMakeCurrent(m_deviceContext, m_resourceContext))
		{
			Log::error("Could NOT set the wGL context to current");
		}

		if (!wglGetCurrentContext())
		{
			Log::error("No valid OpenGL context");
		}

		//load glad and check it has been successfully set up.
		if (!gladLoadGL())
		{
			Log::error("Could NOT load GLAD");
		}
	}

	void Win32_OpenGL_GraphicsContxt::swapBuffers()
	{
		SwapBuffers(m_deviceContext);
	}
}