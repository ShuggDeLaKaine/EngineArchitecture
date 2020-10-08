/** \file graphicsContext.h */
#pragma once


namespace
{
	/* \class GraphicsContext
	* 
	*/
	class GraphicsContext
	{
	public:
		virtual void init() = 0;		//!< initiate the graphics context for the given windowing API
		virtual void swapBuffers() = 0;	//!< to swap the front and back buffers (aka double buffering).

	private:


	};


}
