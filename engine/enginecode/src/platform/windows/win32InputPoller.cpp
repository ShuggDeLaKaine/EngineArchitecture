/** \file win32InputPoller.cpp */

#include "engine_pch.h"
#include "platform/windows/win32InputPoller.h"
#include <Windows.h>

namespace Engine
{

	bool Win32InputPoller::isKeyPressed(int32_t keycode)
	{
		int32_t result = GetAsyncKeyState(keycode);
		return result & NG_KEY_PRESS;
	}

	bool Win32InputPoller::isMouseButtonPressed(int32_t button)
	{
		return GetAsyncKeyState(button);
	}

	glm::vec2 Win32InputPoller::getMousePosition()
	{
		LPPOINT point;
		if(GetCursorPos(point))
		{
			return glm::vec2(static_cast<float>(point->x), static_cast<float>(point->y));
		}
		return glm::vec2(-1.0f, -1.0f);
	}

}