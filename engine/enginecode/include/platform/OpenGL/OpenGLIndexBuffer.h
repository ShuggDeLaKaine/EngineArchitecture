/** \file OpenGLIndexBuffer.h */
#pragma once
#include "rendering/indexBuffer.h"

namespace Engine
{
	/**
	* \class OpenGLIndexBuffer
	*/
	class OpenGLIndexBuffer : public IndexBuffer //inherit for abstraction.
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);		//!< constructor
		virtual ~OpenGLIndexBuffer();								//!< destructor.
		virtual inline uint32_t getRenderID() const override { return m_OpenGL_ID; }	//!< gets and returns the renderer ID.
		virtual inline uint32_t getCount() const override { return m_count; }			//!< gets and returns the count

	private:
		uint32_t m_OpenGL_ID;		//!< OpenGL render identifier 
		uint32_t m_count;			//!< the draw count.

	};

}
