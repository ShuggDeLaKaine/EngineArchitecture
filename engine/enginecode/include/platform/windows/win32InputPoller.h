/** \file win32InputPoller.h */
#pragma once

#include <glm/glm.hpp>
#include "platform/windows/win32Codes.h"

namespace Engine
{
	/*	\class Win32InputPoller 
	*	\brief Win32 specific input poller to get current mouse and/or keyboard state.
	*/
	class Win32InputPoller
	{
	public:
		static bool isKeyPressed(int32_t keycode);			//!< is key pressed?
		static bool isMouseButtonPressed(int32_t button);	//!< is mouse button pressed?
		static glm::vec2 getMousePosition();				//!< get current mouse position.
	private:	 

	};
}
