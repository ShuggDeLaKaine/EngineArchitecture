/** \file GLFW_OpenGL_GC.h */
#pragma once

#include "core/graphicsContext.h"

namespace Engine
{
	/* \class GLFW_OpenGL_GC
	*
	*/
	class GLFW_OpenGL_GC : public GraphicsContext
	{
	public:
		GLFW_OpenGL_GC(GLFWwindow * window) : m_window(window) {}		//!< constructor
		virtual void init() override;			//!< initiate the graphics context for the given windowing API
		virtual void swapBuffers() override;	//!< to swap the front and back buffers (aka double buffering).
	private:
		GLFWwindow * m_window;		//!< pointer to GLFW window, no ownership of this so that's why a raw pointer.
	};
}
