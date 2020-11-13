/** \file subTexture.h */
#pragma once

#include "platform/OpenGL/OpenGLTexture.h"
#include <memory>
#include <glm/glm.hpp>

namespace Engine
{
	/** \class SubTexture
	* class to deal with sub textures and texture atlas's.
	*/
	class SubTexture
	{
	public:
		SubTexture() {};		//!< default constructor.
		SubTexture(const std::shared_ptr<OpenGLTexture>& texture, const glm::vec2& UVStart, const glm::vec2& UVEnd);	//!< specialised constructor; with shared_ptr to a texture, vec2 for UV start of sub texture and UV end for the end of the sub texture.
		
		inline glm::vec2 getUVStart() { return m_UVStart; }		//!< accessor to return UV start data.
		inline glm::vec2 getUVEnd() { return m_UVEnd; }			//!< accessor to return UV end data.
		inline glm::ivec2 getTextureSize() { return m_size; }	//!< accessor to return pixel size of texture.
		glm::vec2 getTextureSizeF() { return { static_cast<float>(m_size.x), static_cast<float>(m_size.y) }; }			//!< returns a float version of m_size, useful for when doing some maths with this.
	
		inline uint32_t getWidth() { return m_size.x; }			//!< accessor to return x var of m_size.		
		inline uint32_t getHeight() { return m_size.y; }		//!< accessor to return y var of m_size.
		inline uint32_t getWidthF() { return static_cast<float>(m_size.x); }		//!< accessor to return floating point of x var of m_size.
		inline uint32_t getHeightF() { return static_cast<float>(m_size.y); }		//!< accessor to return floating point of y var of m_size.

	private:
		std::shared_ptr<OpenGLTexture> m_texture;		//!< will hold and store texture.
		glm::vec2 m_UVStart;		//!< will hold and store UV start data.
		glm::vec2 m_UVEnd;			//!< will hold and store UV end data.
		glm::ivec2 m_size;			//!< store size in pixels.
	};

}
