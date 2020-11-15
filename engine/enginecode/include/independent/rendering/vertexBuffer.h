/** \file vertexBuffer.h */
#pragma once

#include <cstdint>
#include "rendering/bufferLayout.h"

namespace Engine
{
	/** \class VertexBuffer
	* class for an API agnostic vertex buffer.
	*/
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;										//!< virtual destructor.
		virtual void edit(void* vertices, uint32_t size, uint32_t offset) = 0;	//!< virtual to edit function, to edit the vertex buffer.
		virtual inline uint32_t getRenderID() const = 0;						//!< virtual to gets and returns the renderer ID.
		virtual inline const BufferLayout& const getLayout() = 0;				//!< virtual to gets and returns the buffer layout.

		static VertexBuffer* create(void* vertices, uint32_t size, BufferLayout layout);	//!< please note, function declared in renderAPI.cpp

	private:
		

	};

}