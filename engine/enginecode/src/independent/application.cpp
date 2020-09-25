/** \file application.cpp
*/

#include "engine_pch.h"
#include "core/application.h"


#ifdef  NG_PLATFORM_WINDOWS
	#include "platform/windows/winTimer.h"
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

		//start the random number syste,
		m_ranNumSytem.reset(new RandomNumberGenerator);
		m_ranNumSytem->start();

		//reset & start the timer.
		//if on a windows platform then make a windows timer, otherwise make a chrono timer.
#ifdef  NG_PLATFORM_WINDOWS
		m_timer.reset(new WinTimer);
#else
		m_timer.reset(new ChronoTimer);
#endif //  NG_PLATFORM_WINDOWS
		m_timer->start();
	}

	Application::~Application()
	{
		//stop the systems in the REVERSE ORDER to how they start.

		//stop the random number system.
		m_ranNumSytem->stop();

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
			timeStep = m_timer->getElapsedTime();
			m_timer->reset();


			//Log::trace("Hey Hey HEY! {0} {1}", 42, "How long is piece of string");
			//Log::file("Hey Hey HEY! {0} {1}", 42, "How long is piece of string");
			//Log::trace("FPS {0}", 1.0f / timeStep);


			//random number generator tests.
			//Log::trace("{0}", RandomNumberGenerator::uniformIntBetween(-10, 10));
			Log::trace("{0}", RandomNumberGenerator::uniformFloatBetween(-10, 10));


			//things to do in the frame...

			
			
		};
	}

}
