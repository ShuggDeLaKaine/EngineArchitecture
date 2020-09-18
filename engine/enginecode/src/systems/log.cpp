/** file system.cpp */

#include "engine_pch.h"
#include "systems/log.h"


namespace Engine {

	//initialising to have nullptrs
	std::shared_ptr<spdlog::logger> Log::s_consolelogger = nullptr;
	std::shared_ptr<spdlog::logger> Log::s_filelogger = nullptr;


	void Log::start(SystemSignal init, ...)
	{

		spdlog::set_pattern("%^{%T]: %v%$");
		spdlog::set_level(spdlog::level::trace);

		s_consolelogger = spdlog::stdout_color_mt("Console");


		char filepath[256] = "logs/";
		char time[128];
		
		/*
		try
		{
		std::time_t t = std::time(nullptr); //getting current time.
		std::strftime(time, sizeof(time), "%d_%m_%y %I_%M_%S", std::localtime(&t)); //put the current time into this time string.
		strcat_s(filepath, time);
		strcat_s(filepath, ".txt");
		s_filelogger = spdlog::basic_logger_mt("File", filepath);
		}
		catch (const spdlog::spdlog_ex& e)
		{
			s_consolelogger->error("Couldn't start file logger: {0}", e.what());
			s_filelogger.reset();
		}
		*/
	}

	void Log::stop(SystemSignal close, ...)
	{
		s_consolelogger->info("Console Logger STOP");
		s_consolelogger.reset();


	}

}