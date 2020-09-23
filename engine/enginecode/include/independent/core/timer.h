/**
\file timer.h
*/
#pragma once

#include <chrono>

namespace Engine
{
	/**
	\class ChronoTimer - a chrono timer class
	*/

	class ChronoTimer
	{
	public:
		inline void start() { m_startPoint = std::chrono::high_resolution_clock::now(); }	//!< start function for timer.
		inline void reset() { m_startPoint = std::chrono::high_resolution_clock::now(); }	//!< reset function for timer.
		float getElaspsedTime() 
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
