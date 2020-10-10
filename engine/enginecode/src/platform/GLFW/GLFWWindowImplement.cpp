/** \file GLFWWindowImplement.cpp */

#include "engine_pch.h"
#include "platform/GLFW/GLFWWindowImplement.h"
#include "platform/GLFW/GLFW_OpenGL_GC.h"
#include "systems/log.h"

namespace Engine
{


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

		//
		m_graphicsContext.reset(new GLFW_OpenGL_GC(m_nativeWindow));
		m_graphicsContext->init();



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

		//set the window resize callback.
		glfwSetWindowSizeCallback(m_nativeWindow, 
			[](GLFWwindow * window, int newWidth, int newHeight)
		{
			EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
			auto& onWindowResize = handler->getOnWindowResizeCallback();
			WindowResizeEvent event(newWidth, newHeight);
			onWindowResize(event);
		}
		);
		//set the window movement callback.
		glfwSetWindowPosCallback(m_nativeWindow,
			[](GLFWwindow * window, int32_t xPos, int32_t yPos)
		{
			EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
			auto& onWindowMove = handler->getOnWindowMoveCallback();
			WindowMoveEvent event(xPos, yPos);
			onWindowMove(event);
		}
		);
		//set the window in focus / lost focus callback.
		glfwSetWindowFocusCallback(m_nativeWindow,
			[](GLFWwindow * window, int focused)
		{
			EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));

			focused = glfwGetWindowAttrib(window, GLFW_FOCUSED);

			if (focused)
			{
				auto& onWindowFocus = handler->getOnWindowFocusCallback();
				WindowFocusEvent event;
				onWindowFocus(event);
			}
			else
			{
				auto& onWindowLostFocus = handler->getOnWindowLostFocusCallback();
				WindowLostFocusEvent event;
				onWindowLostFocus(event);
			}
		}
		);

		//set the key callback; press, release & repeat.
		glfwSetKeyCallback(m_nativeWindow,
			[](GLFWwindow * window, int keycode, int scancode, int action, int mods)
		{
			EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));

			if (action == GLFW_PRESS)
			{
				auto& onKeyPress = handler->getOnKeyPressCallback();
				KeyPressedEvent event(keycode, 0);
				onKeyPress(event);
			}
			else if (action == GLFW_RELEASE)
			{
				auto& onKeyRelease = handler->getOnKeyReleaseCallback();
				KeyReleasedEvent event(keycode);
				onKeyRelease(event);
			}
			else if (action == GLFW_REPEAT)
			{
				auto& onKeyPress = handler->getOnKeyPressCallback();
				KeyPressedEvent event(keycode, 1);
				onKeyPress(event);
			}
		}
		);

		//set the mouse movement callback.
		glfwSetCursorPosCallback(m_nativeWindow,
			[](GLFWwindow * window, double xPos, double yPos)
		{
			EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
			auto& onMouseMove = handler->getOnMouseMoveCallback();
			MouseMovementEvent event(xPos, yPos);
			onMouseMove(event);
		}
		);

		//set the mouse button press / release callback.
		glfwSetMouseButtonCallback(m_nativeWindow,
			[](GLFWwindow * window, int button, int action, int mods)
		{
			EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
			
			if (action == GLFW_PRESS)
			{
				auto& onMousePress = handler->getOnMouseButtonPressCallback();
				MouseButtonPressEvent event(button);
				onMousePress(event);
			}
			else if (action == GLFW_RELEASE)
			{
				auto& onMouseRelease = handler->getOnMouseButtonReleaseCallback();
				MouseButtonReleaseEvent event(button);
				onMouseRelease(event);
			}
		}
		);

		//set the mouse scroll callback.
		glfwSetScrollCallback(m_nativeWindow,
			[](GLFWwindow * window, double xOffset, double yOffset)
		{
			EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
			auto& onMouseScroll = handler->getOnMouseScrollCallback();
			MouseScrollEvent event(xOffset, yOffset);
			onMouseScroll(event);
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
		m_graphicsContext->swapBuffers();
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