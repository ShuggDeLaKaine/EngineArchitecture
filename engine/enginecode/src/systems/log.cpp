/** file system.cpp */

#include "engine_pch.h"
#include "systems/log.h"


namespace Engine {

	//initialised to have a nullptr
	std::shared_ptr<spdlog::logger> Log::s_consolelogger = nullptr;


	void Log::start(SystemSignal init, ...)
	{

		spdlog::set_pattern("%^{%T]: %v%$");
		spdlog::set_level(spdlog::level::trace);

		s_consolelogger = spdlog::stdout_color_mt("Console");
	}

	void Log::stop(SystemSignal close, ...)
	{
		s_consolelogger->info("Console Logger STOP");
		s_consolelogger.reset();


	}

}