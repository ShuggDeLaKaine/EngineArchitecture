/** \file application.cpp */

#include "engine_pch.h"

#include <glad/glad.h>

#include "core/application.h"

#ifdef  NG_PLATFORM_WINDOWS
	#include "platform/windows/win32System.h"
	#include "platform/windows/winTimer.h"
#else
	#include "platform/GLFW/GLFWSystem.h"
#endif //  NG_PLATFORM_WINDOWS


namespace Engine {
	// Set static vars
	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		if (s_instance == nullptr)
		{
			s_instance = this;
		}

		//start the systems.

		//start the log system.
		m_logSystem.reset(new Log);
		m_logSystem->start();

		//start the windows system.
#ifdef NG_PLATFORM_WINDOWS
		m_windowsSystem.reset(new Win32System);
#else
		m_windowsSystem.reset(new GLFWSystem);
#endif
		m_windowsSystem->start();

		//reset & start the timer.
		//if on a windows platform then make a windows timer, otherwise make a chrono timer.
		//NOTE - not a system! Don't need to stop it.
#ifdef  NG_PLATFORM_WINDOWS
		m_timer.reset(new WinTimer);
#else
		m_timer.reset(new ChronoTimer);
#endif //  NG_PLATFORM_WINDOWS
		m_timer->start();

		//modify the window a little.
		WindowProperties properties("MY FIRST WINDOW", 800, 600, false);
		//create a default window.
		m_window.reset(Window::createWindow(properties));

		//start the random number syste,
		m_ranNumSytem.reset(new RandomNumberGenerator);
		m_ranNumSytem->start();

		//calling function that binds all the callbacks for the event types.
		bindAllEventsTypes();

		InputPoller::setNativeWindow(m_window->getNativeWindow());

		m_timer->reset();
	}

	void Application::bindAllEventsTypes()
	{
		//window events.
		m_window->getEventHandler().setOnWindowCloseCallback(std::bind(&Application::onWindowClose, this, std::placeholders::_1));
		m_window->getEventHandler().setOnWindowResizeCallback(std::bind(&Application::onWindowResize, this, std::placeholders::_1));
		m_window->getEventHandler().setOnWindowMoveCallback(std::bind(&Application::onWindowMove, this, std::placeholders::_1));
		m_window->getEventHandler().setOnWindowFocusCallback(std::bind(&Application::onWindowFocus, this, std::placeholders::_1));
		m_window->getEventHandler().setOnWindowLostFocusCallback(std::bind(&Application::onWindowLostFocus, this, std::placeholders::_1));

		//key events.
		m_window->getEventHandler().setOnKeyPressCallback(std::bind(&Application::onKeyPressed, this, std::placeholders::_1));
		m_window->getEventHandler().setOnKeyReleaseCallback(std::bind(&Application::onKeyReleased, this, std::placeholders::_1));

		//mouse events.
		m_window->getEventHandler().setOnMouseMoveCallback(std::bind(&Application::onMouseMove, this, std::placeholders::_1));
		m_window->getEventHandler().setOnMouseButtonPressCallback(std::bind(&Application::onMouseButtonPress, this, std::placeholders::_1));
		m_window->getEventHandler().setOnMouseButtonReleaseCallback(std::bind(&Application::onMouseButtonRelease, this, std::placeholders::_1));
		m_window->getEventHandler().setOnMouseScrollCallback(std::bind(&Application::onMouseScroll, this, std::placeholders::_1));
	}

	bool Application::onWindowClose(WindowCloseEvent & event)
	{
		event.handleEvent(true);
		Log::info("Window Closed Event: {0}");
		m_running = false;
		return event.isEventHandled();
	}

	bool Application::onWindowResize(WindowResizeEvent & event)
	{
		event.handleEvent(true);
		Log::info("Window Resized Event: ({0}, {1})", event.getWindowSize().x, event.getWindowSize().y);
		return event.isEventHandled();
	}

	bool Application::onWindowMove(WindowMoveEvent & event)
	{
		event.handleEvent(true);
		Log::info("Window Move Event: ({0}, {1})", event.getWindowPosition().x, event.getWindowPosition().y);
		return event.isEventHandled();
	}

	bool Application::onWindowFocus(WindowFocusEvent & event)
	{
		event.handleEvent(true);
		Log::info("Window Focus Event: {0}");
		return event.isEventHandled();
	}

	bool Application::onWindowLostFocus(WindowLostFocusEvent & event)
	{
		event.handleEvent(true);
		Log::info("Window Lost Focus Event: {0}");
		return event.isEventHandled();
	}

	bool Application::onKeyPressed(KeyPressedEvent & event)
	{
		event.handleEvent(true);
		Log::info("Key Pressed Event: key: {0}, repeat: {1}", event.getKeycode(), event.getRepeatCount());
		if (event.getKeycode() == NG_KEY_SPACE)
		{
			Log::error("SPACEBAR PRESSED");
		}
		return event.isEventHandled();
	}

	bool Application::onKeyReleased(KeyReleasedEvent & event)
	{
		event.handleEvent(true);
		Log::info("Key Released Event: key: {0}", event.getKeycode());
		if (event.getKeycode() == NG_KEY_UP)
		{
			Log::error("UP RELEASED");
		}
		return event.isEventHandled();
	}

	bool Application::onMouseMove(MouseMovementEvent & event)
	{
		event.handleEvent(true);
		Log::info("Mouse Move Event: ({0}, {1})", event.getMousePosition().x, event.getMousePosition().y);
		return event.isEventHandled();
	}

	bool Application::onMouseButtonPress(MouseButtonPressEvent & event)
	{
		event.handleEvent(true);
		Log::info("Mouse Button Pressed Event: {0}", event.getButtonPressed());
		return event.isEventHandled();
	}

	bool Application::onMouseButtonRelease(MouseButtonReleaseEvent & event)
	{
		event.handleEvent(true);
		Log::info("Mouse Button Released Event: {0}", event.getButtonReleased());
		return event.isEventHandled();
	}

	bool Application::onMouseScroll(MouseScrollEvent & event)
	{
		event.handleEvent(true);
		//Log::info("Mouse Scroll Event: ({0}, {1})", event.getXMouseScroll(), event.getYMouseScroll());
		Log::info("Mouse Scroll Event: ({0}, {1})", static_cast<float>(event.getXMouseScroll()), static_cast<float>(event.getYMouseScroll()));
		return event.isEventHandled();
	}


	Application::~Application()
	{
		//stop the systems in the REVERSE ORDER to how they start.
		//stop the random number system.
		m_ranNumSytem->stop();

		//stop the windows system.
		m_windowsSystem->stop();

		//stop the log system.
		m_logSystem->stop();
	}


	void Application::run()
	{
		//create a float for the time step and initialise at 0.
		float timeStep = 0.0f;
		//float accumulatedTime = 0.0f;

		glEnable(GL_DEPTH);
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);

		while (m_running)
		{
			//update the time step with the timer function getElapsedTime()
			timeStep = m_timer->getElapsedTime();
			m_timer->reset();
			
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//testing key and mouse button press.
			if (InputPoller::isKeyPressed(NG_KEY_W))
			{
				Log::error("W Pressed");
			}
			if (InputPoller::isMouseButtonPressed(NG_MOUSE_BUTTON_LEFT))
			{
				Log::error("LEFT Mouse Button Pressed");
			}
			if (InputPoller::isMouseButtonPressed(NG_MOUSE_BUTTON_RIGHT))
			{
				Log::error("RIGHT Mouse Button Pressed");
			}

			//Log::trace("Current Mouse Position: ({0}, {1})", InputPoller::getMouseXPos(), InputPoller::getMouseYPos());
			

			//
			m_window->onUpdate(timeStep);

			//things to do in the frame...


		};
	}

}


//THIS WAS ALL IN THE APPLICATION::RUN()
//USED FOR TESTING STUFF.
/*
			//accumulatedTime += timeStep;


			//***event testing***
			//testing that the window closes after a second.
			if (accumulatedTime > 1.0f)
			{
				WindowCloseEvent close;
				auto& callback = m_eventHandler.getOnWindowCloseCallback();
				callback(close);
			}
			
			//***logging system tests***
			//Log::trace("Hey Hey HEY! {0} {1}", 42, "How long is piece of string");
			//Log::file("Hey Hey HEY! {0} {1}", 42, "How long is piece of string");
			//Log::trace("FPS {0}", 1.0f / timeStep);

			//***random number generator system tests***
			//Log::trace("{0}", RandomNumberGenerator::uniformIntBetween(-10, 10));
			//Log::trace("{0}", RandomNumberGenerator::uniformFloatBetween(-10, 10));
			//Log::trace("{0}", RandomNumberGenerator::normalIntBetween(10.0f, 2.5f));
			//Log::trace("{0}", RandomNumberGenerator::normalFloatBetween(5.0f, 1.25f));
*/