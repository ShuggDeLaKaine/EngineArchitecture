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
	}

	Application::~Application()
	{
		//stop the systems.

		//stop the log system.
		m_logSystem->stop();

	}


	void Application::run()
	{
		while (m_running)
		{
			Log::trace("Hey Hey HEY! {0} {1}", 42, "How long is piece of string");
			//Log::file("Hey Hey HEY! {0} {1}", 42, "How long is piece of string");
		};
	}

}
