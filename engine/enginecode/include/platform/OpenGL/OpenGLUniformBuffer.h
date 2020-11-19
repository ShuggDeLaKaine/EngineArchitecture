/** \file OpenGLUniformBuffer.h */
#pragma once

#include "rendering/uniformBuffer.h"

namespace Engine
{
	/* \class OpenGLUniformBuffer
	* 
	*/
	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(const UniformBufferLayout& layout);						//!< constructor.
		~OpenGLUniformBuffer();			//!< destructor.
		inline uint32_t getRenderID() override { return m_OpenGL_ID; }				//!< accessor for openGL ID.
		inline UniformBufferLayout getLayout() override { return m_BufferLayout; }	//!< accessor for the uniform buffer layout.
		void attachShaderBlock(const std::shared_ptr<Shaders>& shader, const char* blockName) override;	//!< 
		void uploadDataToBlock(const char* uniformName, void * data) override;		//!< 

	private:
		uint32_t m_OpenGL_ID;					//!< OpenGL ID.
		static uint32_t s_blockNumber;			//!< a global block number.

	};


}