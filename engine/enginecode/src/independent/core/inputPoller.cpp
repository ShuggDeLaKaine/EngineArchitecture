/* \file inputPoller.cpp */
#include "engine_pch.h"
#include "core/inputPoller.h"

#ifdef NG_PLATFORM_WINDOWS
#include "platform/GLFW/GLFWInputPoller.h"
#include <GLFW/glfw3.h>
#endif

namespace Engine
{
#ifdef NG_PLATFORM_WINDOWS

	bool InputPoller::isKeyPressed(int32_t keycode)
	{
		return GLFWInputPoller::isKeyPressed(keycode);
	}

	bool InputPoller::isMouseButtonPressed(int32_t button)
	{
		return GLFWInputPoller::isMouseButtonPressed(button);
	}

	glm::vec2 InputPoller::getMousePosition()
	{
		return GLFWInputPoller::getMousePosition();
	}

	void InputPoller::setNativeWindow(void * nativeWin)
	{
		GLFWInputPoller::setCurrentWindow(reinterpret_cast<GLFWwindow*>(nativeWin));
	}
#endif

}