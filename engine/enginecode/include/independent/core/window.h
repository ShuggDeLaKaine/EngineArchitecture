/** \file window.h */
#pragma once
#include "events/eventHandler.h"

namespace Engine
{

	struct WindowProperties
	{
		char * m_windowTitle;
		unsigned int m_width;
		unsigned int m_height;
		bool m_isFullScreen;
		bool m_isVSync;
	};

	/** \class Window
	* Abstract base class for all implemented windows, which will code to this interface.
	*/
	class Window
	{
	public:
		virtual void initWindow(const WindowProperties& properties) = 0;	//!< func to initiate a window.
		virtual void closeWindow() = 0;		//!< func to close a window.
		virtual ~Window() {};				//!< deconstructor
		virtual void onUpdate(float timestep) = 0;		//!< func to update the window with each timestep.
		virtual void setVSync(bool VSync) = 0;			//!< func to set the VSync to the window.
		virtual unsigned int getWidth() const = 0;		//!< func to get the width of the window.
		virtual unsigned int getHeight() const = 0;		//!< func to get the height of the window.
		virtual void* getNativeWindow() const = 0;		//!< func to get the native window.
		virtual bool isFullScreenWindow() const = 0;	//!< func to check whether the window is in fullscreen.
		virtual bool isVSync() const = 0;				//!< func to check whether VSync-ed.

		virtual EventHandler& getEventHandler() = 0;	//!< func to get an instance of the event handler.
		static Window* createWindow(const WindowProperties& properties = WindowProperties());		//!< create a window using the properties definied in WindowProperties.
	protected:
		//std::shared_ptr<GraphicsContext> m_graphicsContext;		//!< 
	};

}
