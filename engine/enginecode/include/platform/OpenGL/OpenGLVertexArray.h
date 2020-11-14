/** \file OpenGLVertexArray.h */
#pragma once

#include <vector>
#include <memory>
#include "OpenGLVertexBuffer.h"
#include "OpenGLIndexBuffer.h"


namespace Engine
{
	/** \class OpenGLVertexArray
	* 
	*/
	class OpenGLVertexArray
	{
	public:
		OpenGLVertexArray();				//!< constructor.
		~OpenGLVertexArray();				//!< deconstructor.
		void addVertexBuffer(const std::shared_ptr<OpenGLVertexBuffer>& vertexBuffer);	//!< add a vertex buffer.
		void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);			//!< set the index buffer.
		uint32_t getDrawCount();			//!< get the draw count of index buffer.
		inline uint32_t getRenderID() const { return m_OpenGL_ID; }	//!< gets and returns the renderer ID.
	private:
		std::vector<std::shared_ptr<OpenGLVertexArray>> m_vertexBuffer;		//!< 
		std::shared_ptr<IndexBuffer> m_indexBuffer;							//!< 
		uint32_t m_OpenGL_ID;					//!< OpenGL render identifier 
		uint32_t m_verArrAttributeIndex = 0;	//!< attribute index.
	};


}
