/** \file log.h */
#pragma once

#include "system.h"
#include <spdlog/spdlog.h>


namespace Engine
{
	class Log : public System
	{
	public:
		virtual void start(SystemSignal init = SystemSignal::None, ...);		//!< Start the logger.
		virtual void stop(SystemSignal close = SystemSignal::None, ...);		//!< Stop the logger.

		template<class ...Args>
		static void info(Args&&... args);	//!<double reference for a paramater pack

		template<class ...Args>
		static void debug(Args&&... args);

		template<class ...Args>
		static void error(Args&&... args);

		template<class ...Args>
		static void trace(Args&&... args);

		template<class ...Args>
		static void warn(Args&&... args);

	private:
		static std::shared_ptr<spdlog::logger> s_consolelogger;		//!< Console logger.

	};

	template<class ...Args>
	static void Log::info(Args&&... args)
	{
		//need if statement to make sure it has been initialised 
		
		//perfect forwarding to forward to the logger
		s_consolelogger->info(std::forward<Args>(args) ...);
	}

	template<class ...Args>
	static void Log::debug(Args&&... args)
	{
		//need if statement to make sure it has been initialised 

		//perfect forwarding to forward to the logger
		s_consolelogger->debug(std::forward<Args>(args) ...);
	}

	template<class ...Args>
	static void Log::error(Args&&... args)
	{
		//need if statement to make sure it has been initialised 

		//perfect forwarding to forward to the logger
		s_consolelogger->error(std::forward<Args>(args) ...);
	}

	template<class ...Args>
	static void Log::trace(Args&&... args)
	{
		//need if statement to make sure it has been initialised 

		//perfect forwarding to forward to the logger
		s_consolelogger->trace(std::forward<Args>(args) ...);
	}

	template<class ...Args>
	static void Log::warn(Args&&... args)
	{
		//need if statement to make sure it has been initialised 

		//perfect forwarding to forward to the logger
		s_consolelogger->warn(std::forward<Args>(args) ...);
	}
}