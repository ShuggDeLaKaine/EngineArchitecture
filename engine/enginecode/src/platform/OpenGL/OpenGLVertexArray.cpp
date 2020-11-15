/** \file OpenGLVertexArray.cpp */

#include "engine_pch.h"
#include "glad/glad.h"
#include "platform/OpenGL/OpenGLVertexArray.h"

namespace Engine
{
	namespace SDT
	{
		static GLenum toGLType(ShaderDataType type)
		{
			//to make this faster can make an array.
			switch (type)
			{
			case ShaderDataType::Float: return GL_FLOAT;
			case ShaderDataType::Float2: return GL_FLOAT;
			case ShaderDataType::Float3: return GL_FLOAT;
			case ShaderDataType::Float4: return GL_FLOAT;
			default: return GL_INVALID_ENUM;
			}
		}
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_OpenGL_ID);
		glBindVertexArray(m_OpenGL_ID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_OpenGL_ID);
	}

	void OpenGLVertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		//need to bind first.
		glBindVertexArray(m_OpenGL_ID);
		//then bind the buffer.
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->getRenderID());

		//get layout and then iterate over it.
		const auto& layout = vertexBuffer->getLayout();
		for (const auto& element : layout)
		{
			uint32_t normalised = GL_FALSE;
			if (element.m_normalised)
			{
				normalised = GL_TRUE;
			}

			//FUTURE - if loading mat4s will need changing.
			//OK for now
			glEnableVertexAttribArray(m_verArrAttributeIndex);
			glVertexAttribPointer(
				m_verArrAttributeIndex,	
				SDT::componentCount(element.m_dataType),	
				SDT::toGLType(element.m_dataType),		
				normalised,				
				layout.getStride(),		
				(void*) element.m_offset
			);
			m_verArrAttributeIndex++;
		}

	}

	void OpenGLVertexArray::setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		m_indexBuffer = indexBuffer;
	}

	uint32_t OpenGLVertexArray::getDrawCount()
	{
		if (m_indexBuffer)
		{
			return m_indexBuffer->getCount();
		}
		else
		{
			return 0;
		}
	}
}