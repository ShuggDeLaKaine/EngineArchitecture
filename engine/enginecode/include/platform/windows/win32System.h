/** \file win32System.h */
#pragma once

#include "systems/system.h"

namespace Engine
{
	/*	\class Win32System
	*	\brief Win32 specific class for all Win32 related systems.
	*/
	class Win32System : public System
	{
	public:
		virtual void start(SystemSignal init = SystemSignal::None, ...) override; //!< Start the system
		virtual void stop(SystemSignal close = SystemSignal::None, ...) override; //!< Stop the system
	private:

	};
}
