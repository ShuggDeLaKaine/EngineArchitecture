/** \file application.h
*/
#pragma once

#include "systems/log.h"
#include "systems/randomNumberGenerator.h"
#include "events/eventHeaders.h"
#include "core/window.h"
#include "core/timer.h"

namespace Engine {

	/**
	\class Application
	Fundemental class of the engine. A singleton which runs the game loop infinitely.
	Provides ...
	*/

	class Application
	{
	protected:
		Application();	//!< Constructor

		std::shared_ptr<Log> m_logSystem;				//!< the log system.
		std::shared_ptr<RandomNumberGenerator> m_ranNumSytem;	//!< the random number generator system.
		std::shared_ptr<System> m_windowsSystem;		//!< the windows system.

		std::shared_ptr<Window> m_window;				//!< the windows.
		std::shared_ptr<ITimer> m_timer;				//!< the timer.
		bool onWindowClose(WindowCloseEvent& event);	//!< run when the window closes.
		bool onWindowResize(WindowResizeEvent& event);	//!< run when the window resized.
	private:
		static Application* s_instance;					//!< Singleton instance of the application
		bool m_running = true;							//!< Is the application running?
	public:
		virtual ~Application(); //!< Deconstructor
		inline static Application& getInstance() { return *s_instance; } //!< Instance getter from singleton pattern
		void run();		//!< Main loop
	};

	// To be defined in users code
	Application* startApplication();		//!< Function definition which provides an entry hook
}