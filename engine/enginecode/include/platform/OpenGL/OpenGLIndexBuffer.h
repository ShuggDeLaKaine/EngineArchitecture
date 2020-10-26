/** \file OpenGLIndexBuffer.h */
#pragma once

#include <cstdint>

namespace Engine
{
	/**
	* \class OpenGLIndexBuffer
	*/
	class OpenGLIndexBuffer //to inherit for abstraction in the future
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);		//!< constructor
		~OpenGLIndexBuffer();										//!< destructor.
		inline uint32_t getRenderID() const { return m_OpenGL_ID; }	//!< gets and returns the renderer ID.
		inline uint32_t getCount() { return m_count; }				//!< gets and returns the count

	private:
		uint32_t m_OpenGL_ID;		//!< OpenGL render identifier 
		uint32_t m_count;			//!< the draw count.

	};

}
