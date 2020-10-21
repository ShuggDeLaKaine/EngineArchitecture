/** \file OpenGLVertexBuffer.h */
#pragma once

#include "rendering/bufferLayout.h"


namespace Engine
{
	/**
	* \class OpenGLVertexBuffer
	*/
	class OpenGLVertexBuffer	//upcoming inheritance from abstraction coming
	{
	public:
		OpenGLVertexBuffer(void* vertices, uint32_t size, BufferLayout layout); 	//!< constructor - void* for vertices as currently unsure what type they'll be.
		~OpenGLVertexBuffer();			//!< destructor.
		void edit(void* vertices, uint32_t size, uint32_t offset);					//!< edit function, to edit the vertex buffer; same params as constructor. Don't need a BufferLayout as that will be set, will need a uint32_t offset to place the buffer.
		inline uint32_t getRenderID() const { return m_OpenGL_ID; }					//!< gets and returns the renderer ID.
		inline const BufferLayout& const getLayout() { return m_layout; }			//!< gets and returns the buffer layout

	private:
		uint32_t m_OpenGL_ID;		//!< OpenGL render identifier 
		BufferLayout m_layout;		//!< buffer layout
	};



}
