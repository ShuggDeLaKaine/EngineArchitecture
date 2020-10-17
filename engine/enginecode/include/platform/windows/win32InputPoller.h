/** \file win32InputPoller.h */
#pragma once

#include <glm/glm.hpp>
#include "platform/windows/win32Codes.h"

namespace Engine
{
	/* \class Win32InputPoller 
	* API agnostic input poller to get current mouse and/or keyboard state, specific to the Win32 platform.
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
