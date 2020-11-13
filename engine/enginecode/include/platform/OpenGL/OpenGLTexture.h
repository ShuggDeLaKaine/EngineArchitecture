/** \file OpenGLTexture.h */
#pragma once
#include <cstdint>
#include <glm/glm.hpp>

namespace Engine
{
	class OpenGLTexture
	{
		/** \class OpenGLTexture
		* class to take in image files for textures.
		*/
	public:
		OpenGLTexture(const char* filepath);	//!< constructor, takes the filepaths for texture.
		~OpenGLTexture();						//!< deconstructor.
		OpenGLTexture(uint32_t width, uint32_t height, uint32_t channel, unsigned char * data);		//!< constructor; width, height and channel plus data in bytes (what unsigned char is).
		void edit(glm::vec2 offset, uint32_t width, uint32_t height, uint32_t channel, unsigned char * data);	//!< edit the texture 
	
		inline uint32_t getID() { return m_OpenGL_ID; }			//!< accessor to get openGL handle.
		inline uint32_t getWidth() { return m_width; }			//!< accessor to get the texture width.		
		inline uint32_t getHeight() { return m_height; }		//!< accessor to get the texture height.
		inline uint32_t getChannel() { return m_channel; }		//!< accessor to get the texture channel.
		inline uint32_t getWidthF() { return static_cast<float>(m_width); }			//!< accessor to get the floating point texture width.
		inline uint32_t getHeightF() { return static_cast<float>(m_height); }		//!< accessor to get the floating point texture height.

	private:
		void init(uint32_t width, uint32_t height, uint32_t channel, unsigned char * data);		//!< an initate function to be used in both of the OpenGLTexture(...) functions, does the bulk of the work for both of these.
	
		uint32_t m_OpenGL_ID;	//!< OpenGL handle.
		uint32_t m_width;		//!< width of texture.
		uint32_t m_height;		//!< height of texture.
		uint32_t m_channel;		//!< channel of texture.
	};	
}