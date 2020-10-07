/** \file application.cpp */

#include "engine_pch.h"
#include "core/application.h"

#ifdef  NG_PLATFORM_WINDOWS
	#include "platform/windows/winTimer.h"
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

		//start the system.
		//start the log system.
		m_logSystem.reset(new Log);
		m_logSystem->start();

		//start the windows system.
#ifdef NG_PLATFORM_WINDOWS
		m_windowsSystem.reset(new GLFWSystem);
//#else
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

		//***TO DO*** CARRY ON WITH THIS FOR THE REST OF THE EVENT HANDLING POSSIBLE EVENTS
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



		m_timer->reset();
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
		auto&  newSize = event.getWindowSize();
		Log::info("Window Resized Event: ({0}, {1})", newSize.x, newSize.y);
		return event.isEventHandled();
	}

	bool Application::onWindowMove(WindowMoveEvent & event)
	{
		event.handleEvent(true);
		auto& newPos = event.getWindowPosition();
		Log::info("Window Move Event: ({0}, {1})", newPos.x, newPos.y);
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
		return event.isEventHandled();
	}

	bool Application::onKeyReleased(KeyReleasedEvent & event)
	{
		event.handleEvent(true);
		Log::info("Key Released Event: key: {0}", event.getKeycode());
		return event.isEventHandled();
	}

	bool Application::onKeyType(KeyTypedEvent & event)
	{
		return false;
	}

	bool Application::onMouseMove(MouseMovementEvent & event)
	{
		return false;
	}

	bool Application::onMouseButtonPress(MouseButtonPressEvent & event)
	{
		return false;
	}

	bool Application::onMouseButtonRelease(MouseButtonReleaseEvent & event)
	{
		return false;
	}

	bool Application::onMouseScroll(MouseScrollEvent & event)
	{
		return false;
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

		while (m_running)
		{
			//update the time step with the timer function getElapsedTime()
			timeStep = m_timer->getElapsedTime();
			m_timer->reset();
			
			//accumulatedTime += timeStep;

			/*
			//***event testing***
			//testing that the window closes after a second.
			if (accumulatedTime > 1.0f)
			{
				WindowCloseEvent close;
				auto& callback = m_eventHandler.getOnWindowCloseCallback();
				callback(close);
			}
			*/

			//***logging system tests***
			//Log::trace("Hey Hey HEY! {0} {1}", 42, "How long is piece of string");
			//Log::file("Hey Hey HEY! {0} {1}", 42, "How long is piece of string");
			//Log::trace("FPS {0}", 1.0f / timeStep);

			//***random number generator system tests***
			//Log::trace("{0}", RandomNumberGenerator::uniformIntBetween(-10, 10));
			//Log::trace("{0}", RandomNumberGenerator::uniformFloatBetween(-10, 10));
			//Log::trace("{0}", RandomNumberGenerator::normalIntBetween(10.0f, 2.5f));
			//Log::trace("{0}", RandomNumberGenerator::normalFloatBetween(5.0f, 1.25f));

			//
			m_window->onUpdate(timeStep);

			//things to do in the frame...

			
			
		};
	}

}
