/** \file GLFWWindowImplement.cpp */

#include "engine_pch.h"
#include "platform/GLFW/GLFWWindowImplement.h"
#include "systems/log.h"

namespace Engine
{
	//if compiled whilst in Windows, then
#ifdef NG_PLATFORM_WINDOWS
	Window* Window::createWindow(const WindowProperties& properties)
	{
		//everytime we create a window, create a new GLFW Window.
		return new GLFWWindowImplement(properties);
	}
#endif

	GLFWWindowImplement::GLFWWindowImplement(const WindowProperties & properties)
	{
		initWindow(properties);
	}

	void GLFWWindowImplement::initWindow(const WindowProperties & properties)
	{
		m_windowProperties = properties;

		m_aspectRatio = static_cast<float>(m_windowProperties.width) / static_cast<float>(m_windowProperties.height);

		if (m_windowProperties.isFullScreen == true)
		{
			//TO DO - IMPLEMENT FULLSCREEN.
			Log::error("No Fullscreen implemented, you had best do this!");
		}
		else
		{
			m_nativeWindow = glfwCreateWindow(m_windowProperties.width, m_windowProperties.height, m_windowProperties.windowTitle, nullptr, nullptr);
		}

		//set the void pointer for the native window to contain the event handler, stored as void pointer.
		glfwSetWindowUserPointer(m_nativeWindow, static_cast<void*>(&m_eventHandler));

		//using annousmous Lambda functions to set event callbacks.
		//the window close callback.
		glfwSetWindowCloseCallback(m_nativeWindow, 
			[](GLFWwindow * window)
		{
			EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
			auto& onWindowClose = handler->getOnWindowCloseCallback();
			WindowCloseEvent event;
			onWindowClose(event);
		}
		);

		glfwSetWindowSizeCallback(m_nativeWindow, 
			[](GLFWwindow * window, int newWidth, int newHeight)
		{
			EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
			auto& onWindowResize = handler->getOnWindowResizeCallback();
			WindowResizeEvent event(newWidth, newHeight);
			onWindowResize(event);
		}
		);
	}

	void GLFWWindowImplement::closeWindow()
	{
		glfwDestroyWindow(m_nativeWindow);
	}

	void GLFWWindowImplement::onUpdate(float timestep)
	{
		//any registered events in the last loop, if so process them.
		glfwPollEvents();
	}

	void GLFWWindowImplement::setVSync(bool VSync)
	{
		m_windowProperties.isVSync = VSync;

		if (VSync == true)
		{
			//everytime swap buffers, should be syncronised with whatever the display settings are.
			glfwSwapInterval(1);
		}
		else
		{
			//if not, run as fast as you can.
			glfwSwapInterval(0);
		}
	}
}