/** \file renderer2d.h */
#pragma once
#include "renderer/rendererCommons.h"

namespace Engine
{
	/* \class Quad
	*  \brief 
	*/
	class Quad
	{
	public:
		Quad() = default;			//!< default constructor.
		static Quad createCentreHalfExtents(const glm::vec2& centre, const glm::vec2& halfExtents);			//!< create a quad with from centre, half extents as a vec2.
		static Quad createCentreHalfExtents(const glm::vec2& centre, float halfExtents);					//!< create a quad with from centre, half extents as a single float.
		static Quad createCreateTopLeftSize(const glm::vec2& topLeft, const glm::vec2& size);				//!< create a quad with from top left, half extents as a vec2.
		static Quad createCreateTopLeftSize(const glm::vec2& topLeft, float size);							//!< create a quad with from top left, half extents as a single float.
		static Quad createCreateTopLeftBottomRight(const glm::vec2& topLeft, const glm::vec2& bottomRight);	//!< create a quad with from top left, this with a vec2 as the bottom right.

	private:
		glm::vec3 m_translate = glm::vec3(0.0f);	//!< translation vector.
		glm::vec3 m_scale = glm::vec3(1.0f);		//!< scale vector.
		friend class Renderer2D;					//!< linking as 'friend' with Renderer2D class, this is because Quad shouldn't be happening without Renderer2D, they are absolutely linked, exist together.
	};

	/* \class Renderer2D
	*  \brief A renderer rendering simple 2D primitives.
	*/
	class Renderer2D
	{
	public:
		static void init();					//!< initiate the internal data of the renderer.
		static void	begin(const SceneWideUniforms& swu);				//!< begin a 2D scene.
		static void submit(const Quad& quad, const glm::vec4& tint);	//!< render a tinted (coloured) quad.
		static void end();					//!< end of the current 2D scene.

	private:
		struct InternalData
		{
			std::shared_ptr<Textures> defaultTexture;	//!< empty white texture.
			std::shared_ptr<Shaders> shader;			//!< the shader used.
			std::shared_ptr<VertexArray> VAO;			//!< prototypical quad for submitting (don't use geometry/VAO in 2d)
			glm::vec4 defaultTint;						//!< default white tint.
			glm::mat4 model;							//!< transform the the model.
		};
		static std::shared_ptr<InternalData> s_data;	//!< data internal to the renderer.
	};

}