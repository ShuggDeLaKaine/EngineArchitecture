/** \file application.cpp
*/

#include "engine_pch.h"
#include "core/application.h"

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

		//reset & start the timer.
		m_timer.reset(new ChronoTimer);
		m_timer->start();
	}

	Application::~Application()
	{
		//stop the systems.

		//stop the log system.
		m_logSystem->stop();

	}


	void Application::run()
	{
		//create a float for the time step and initialise at 0.
		float timeStep = 0.0f;

		while (m_running)
		{
			//update the time step with the timer function getElapsedTime()
			timeStep = m_timer->getElaspsedTime();
			m_timer->reset();


			//Log::trace("Hey Hey HEY! {0} {1}", 42, "How long is piece of string");
			//Log::file("Hey Hey HEY! {0} {1}", 42, "How long is piece of string");
			Log::trace("FPS {0}", 1.0f / timeStep);

			//things to do in the frame...

			
			
		};
	}

}
