/** \file graphicsContext.h */
#pragma once

namespace
{
	/* \class GraphicsContext
	*  \brief Short class to initiate which graphics context it is and swap the buffers for double buffering.
	*/
	class GraphicsContext
	{
	public:
		virtual void init() = 0;		//!< initiate the graphics context for the given windowing API
		virtual void swapBuffers() = 0;	//!< to swap the front and back buffers (aka double buffering).
	private:

	};
}
