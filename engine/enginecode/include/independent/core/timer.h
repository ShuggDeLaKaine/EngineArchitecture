/**
\file timer.h
*/
#pragma once

#include <chrono>

namespace Engine
{

	/**
	\class ITimer - an interface class for all timers.
	*/

	class ITimer
	{
	public:
		virtual void start() = 0;	//!< start function for timer.
		virtual void reset() = 0;	//!< reset function for timer.
		virtual float getElaspsedTime() = 0;	//!< function to get the elapsed time between last start or restart.
	};


	/**
	\class ChronoTimer - a chrono timer class
	*/

	class ChronoTimer : public ITimer
	{
	public:
		virtual inline void start() override { m_startPoint = std::chrono::high_resolution_clock::now(); }	//!< start function for timer.
		virtual inline void reset() override { m_startPoint = std::chrono::high_resolution_clock::now(); }	//!< reset function for timer.
		virtual float getElaspsedTime() override
		{
			m_endPoint = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float, std::milli> elapsed = m_endPoint - m_startPoint;
			return elapsed.count() / 1000.0f;
		}		//!< function to get the elapsed time between the start and end points; using 'chrono::duration' in milliseconds. 

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_startPoint;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_endPoint;
	};


}
