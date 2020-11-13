/** \file OpenGLTexture.cpp */

#include "engine_pch.h"
#include "platform/OpenGL/OpenGLTexture.h"
#include <glad/glad.h>
#include "systems/log.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Engine
{

	OpenGLTexture::OpenGLTexture(const char * filepath)
	{
		int width, height, channel;
		unsigned char *data = stbi_load(filepath, &width, &height, &channel, 0);

		//check whether there is data, then call init(). 
		if (data)
		{
			init(width, height, channel, data);
		}
		else
		{
			//if no data, kick an error.
			Log::error("Cannot load texture data!");
			return;
		}

		//data passed on so can take off the CPU.
		stbi_image_free(data);
	}

	OpenGLTexture::OpenGLTexture(uint32_t width, uint32_t height, uint32_t channel, unsigned char * data)
	{
		//check whether there is data, then call init(). 
		if (data)
		{
			init(width, height, channel, data);
		}
		else
		{
			//if no data, kick an error.
			Log::error("Cannot load texture data!");
			return;
		}
	}

	OpenGLTexture::~OpenGLTexture()
	{
		glDeleteTextures(1, &m_OpenGL_ID);
	}

	void OpenGLTexture::edit(glm::vec2 offset, uint32_t width, uint32_t height, uint32_t channel, unsigned char * data)
	{
		//first bind the texture.
		glBindTexture(GL_TEXTURE_2D, m_OpenGL_ID);

		//check whether there is any data.
		if (data)
		{
			if (channel == 3)
			{
				glTextureSubImage2D(GL_TEXTURE_2D, 0, offset.x, offset.y, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
				//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			}
			else if (channel == 4)
			{
				glTextureSubImage2D(GL_TEXTURE_2D, 0, offset.x, offset.y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
				//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			}
		}
	}

	void OpenGLTexture::init(uint32_t width, uint32_t height, uint32_t channel, unsigned char * data)
	{
		//generate and bind the texture.
		glGenTextures(1, &m_OpenGL_ID);


		//tell it how to wrap, here is clamp to the edge.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//min or magnify, just using linear filtering here.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//TO DO!!!
		//NEED: a channel == 1, especially for dealing with monochromatic bitmaps.

		if (channel == 3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else if (channel == 4)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else return;
		glGenerateMipmap(GL_TEXTURE_2D);

		//data passed on so can take off the CPU.
		//stbi_image_free(data);

		//set the members of the class to the function variables of the same name.
		m_width = width;
		m_height = height;
		m_channel = channel;
	}


}