/** \file renderer2d.h */
#pragma once
#include "renderer/rendererCommons.h"

namespace Engine
{

	class Quad;

	/* \class Renderer2D
	*  \brief A renderer rendering simple 2D primitives.
	*/
	class Renderer2D
	{
	public:
		static void init();					//!< initiate the internal data of the renderer.
		static void	begin(const SceneWideUniform& swu);						//!< begin a 2D scenen.
		static void submit(const Quad& quad, const glm::vec4& tint);		//!< render a tinted (coloured) quad.
		static void end();					//!< end of the current 2D scene.

	private:
		//TO DO: move the below to the rendererCommon.h
		struct InternalData
		{
			//SceneWideUniforms sceneWideUniforms;		//!< replace with UBO in the future. - NOT NEEDED HERE, don't need to keep a reference to them.
			std::shared_ptr<Textures> defaultTexture;	//!< empty white texture.
			std::shared_ptr<Shaders> defaultShaders;	//!< the shader used.
			std::shared_ptr<VertexArray> VAO;			//!< prototypical quad for submitting (don't use geometry/VAO in 2d)
			glm::vec4 defaultTint;						//!< default white tint.
			glm::vec4 model;							//!< transform the the model.
		};
		static std::shared_ptr<InternalData> s_data;	//!< data internal to the renderer.

	};

}