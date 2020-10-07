/* \file GLFW_OpenGL_GC.cpp */

#include "engine_pch.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "platform/GLFW/GLFW_OpenGL_GC.h"
#include "systems/log.h"

namespace Engine
{

	void GLFW_OpenGL_GC::init()
	{
		glfwMakeContextCurrent(m_window);

		//now load openGL... <glad/glad.h> will help.
		auto result = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

		//check that it has loaded, error if not.
		if (!result)
		{
			Log::error("Could not create OpenGL contrxt for this current GFLW window: {0}", result);
		}
	}

	void GLFW_OpenGL_GC::swapBuffers()
	{
		glfwSwapBuffers(m_window);
	}
}