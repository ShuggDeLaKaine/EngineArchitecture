/** \file winTimer.h */
#pragma once

#include <Windows.h>
#include "core/timer.h"

namespace Engine
{
	/** \class WinTimer
	*	\brief A Windows specific timers; using queryPerformanceCount
	*/
	class WinTimer : public ITimer
	{
		virtual void start() override 
		{
			QueryPerformanceFrequency(&m_LI_frequency);
			QueryPerformanceCounter(&m_LI_startTime);
		};		//!< start timer.
		virtual inline void reset() override 
		{
			QueryPerformanceCounter(&m_LI_startTime);
		};		//!< reset timer.
		virtual float getElapsedTime() override 
		{
			QueryPerformanceCounter(&m_LI_endTime);
			float result = (m_LI_endTime.QuadPart - m_LI_startTime.QuadPart) * 1000.0f / m_LI_frequency.QuadPart;
			result /= 1000.0f;		
			return result;
		}		//!< get the elasped time, divided by 1000 to get milliseconds.
	private:
		LARGE_INTEGER m_LI_startTime;	//!< the start time for the timer.
		LARGE_INTEGER m_LI_endTime;		//!< the end time for the timer.
		LARGE_INTEGER m_LI_frequency;	//!< ticks per second of this CPU.
	};
}