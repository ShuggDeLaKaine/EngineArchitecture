/** \file eventHandler.h */
#pragma once

#include "events.h"
#include "eventHeaders.h"
#include <functional>

namespace Engine
{
	class EventHandler
	{
	public:
		void setOnCloseCallback(const std::function<bool(WindowCloseEvent&)>& fn) { m_onCloseCallback = fn; }	//!< accessor func - pass it bound function and it sets it.
		std::function<bool(WindowCloseEvent&)>& getOnCloseFunction() { return m_onCloseCallback; }				//!< accessor func - passes a reference to bound function.
	private:
		std::function<bool(WindowCloseEvent&)> m_onCloseCallback = std::bind(&EventHandler::defaultOnClose, this, std::placeholders::_1);	//!< binding ... function to this instance, wrapped with 1 param passed.
		bool defaultOnClose(WindowCloseEvent& event) { return false; }		//!< plain reference/function, returns false as default and says hasn't handled it.
	};



}