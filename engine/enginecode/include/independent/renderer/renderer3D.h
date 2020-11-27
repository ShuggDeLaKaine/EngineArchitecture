/** \file renderer3D.h */
#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <unordered_map>
#include "rendering/textures.h"
#include "rendering/shaders.h"
#include "rendering/renderAPI.h"
#include "rendering/shaderDataType.h"
#include "rendering/vertexArray.h"

namespace Engine
{
	using SceneWideUniforms = std::unordered_map<const char*, std::pair<ShaderDataType, void *>>;

	class Material;

	/* \class Renderer3D
	* \brief A rendererm renders 3D geometry instantly (not batched), that uses OpenGL.
	*/
	class Renderer3D
	{
	public:
		static void init();			//!< initiate the renderer.
		static void begin(const SceneWideUniforms& sceneWideUniforms);		//!< begin a new 3D scene.
		static void submit(const std::shared_ptr<VertexArray>& geometry, const std::shared_ptr<Material> material, const glm::mat4& model);		//!< submit a new piece of geometry to be rendered.
		static void end();			//!< end of the current 3D scene.

	private:
		struct InternalData
		{
			SceneWideUniforms sceneWideUniforms;		//!< replace with UBO in the future.
			std::shared_ptr<Textures> defaultTexture;	//!< empty white texture.
			glm::vec4 defaultTint;						//!< default white tint.
		};
		static std::shared_ptr<InternalData> s_data;	//!< data internal to the renderer.
	};

}