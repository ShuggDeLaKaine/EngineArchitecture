/** \file renderer3D.h */
#pragma once
#include "renderer/rendererCommons.h"

namespace Engine
{
	/* \class Material
	*  \brief Holds a shader and all the uniform data that is associated with specific shader.
	*/
	class Material
	{
	public:
		Material(const std::shared_ptr<Shaders>& shader) :
			m_shader(shader),
			m_flags(0),
			m_texture(nullptr),
			m_tint(glm::vec4(0.0f))
		{}			//!< default constructor for materials, initaliser list to initialise all the variables, all to 0s/nullptrs except the shaders which is in the params.
		Material(const std::shared_ptr<Shaders>& shader, const std::shared_ptr<Textures>& texture, const glm::vec4& tint) : 
			m_shader(shader),
			m_texture(texture),
			m_tint(tint)
		{
			setFlag(flag_texture | flag_tint);
		}			//!< constructor will all the variables within the params, all initialised within the initialiser list, flag for texture and tint set within function.
		Material(const std::shared_ptr<Shaders>& shader, const std::shared_ptr<Textures>& texture) :
			m_shader(shader),
			m_texture(texture),
			m_tint(0.0f)
		{
			setFlag(flag_texture);
		}			//!< constructor will shader and texture within within the params, initialised within the initialiser list, flag for texture set within function.
		Material(const std::shared_ptr<Shaders>& shader, const glm::vec4& tint) :
			m_shader(shader),
			m_texture(nullptr),
			m_tint(tint)
		{
			setFlag(flag_tint);
		}			//!< constructor will shader and tint within within the params, initialised within the initialiser list, flag for tint set within function.
		
		inline std::shared_ptr<Shaders> getShader() const { return m_shader; }		//!< accessor function for getting the shader.
		inline std::shared_ptr<Textures> getTexture() const { return m_texture; }	//!< accessor function for getting the texture.
		inline glm::vec4 getTint() const { return m_tint; }		//!< accessor function to get the tint.

		//setShader() removed as shouldn't be able to change a shader once it's been set with material; need a new shader then make a new material.
		//void setShader(const std::shared_ptr<Shaders>& shader) { m_shader = shader; }		//!< function to set the shader.
		void setTexture(const std::shared_ptr<Textures>& texture) { m_texture = texture; }	//!< function to set the texture.
		void setTint(const glm::vec4 tint) { m_tint = tint; }		//!< function to set the tint colour.

		bool isFlagSet(uint32_t flag) { return m_flags & flag; }	//!< function to check whether the flag has been set. If either return anything than 0, flag is set.

		constexpr static uint32_t flag_texture = 1 << 0;	//!< 00000001
		constexpr static uint32_t flag_tint = 1 << 1;		//!< 00000010

	private:
		uint32_t m_flags = 0;						//!< bitfield representation of the shader settings.
		std::shared_ptr<Shaders> m_shader;			//!< the shader.
		std::shared_ptr<Textures> m_texture;		//!< the texture for the material.
		glm::vec4 m_tint;							//!< coloured tint to be applied to the geometry.
		void setFlag(uint32_t flag) { m_flags = m_flags | flag; }		//!< function to set the flag.
	};

	/* \class Renderer3D
	*  \brief A renderer renders 3D geometry instantly (not batched), that uses OpenGL.
	*/
	class Renderer3D
	{
	public:
		static void init();												//!< initiate the renderer.
		static void begin(const SceneWideUniforms& sceneWideUniforms);	//!< begin a new 3D scene.
		static void submit(const std::shared_ptr<VertexArray>& geometry, const std::shared_ptr<Material> material, const glm::mat4& model);		//!< submit a new piece of geometry to be rendered.
		static void end();												//!< end of the current 3D scene.
		static void attachShader(std::shared_ptr<Shaders> shader);				//!< attach the shader.
	private:
		struct InternalData
		{
			SceneWideUniforms sceneWideUniforms;		//!< replace with UBO in the future.
			std::shared_ptr<Textures> defaultTexture;	//!< empty white texture.
			glm::vec4 defaultTint;						//!< default white tint.

			std::shared_ptr<Shaders> shader;			//!< the shader.
			std::shared_ptr<VertexArray> VAO;			//!< the vertex array.

			std::shared_ptr<UniformBuffer> cameraUBO;	//!< UBO for the camera.
			std::shared_ptr<UniformBuffer> lightingUBO;	//!< UBO for the lighting.
		};												//!< to be used a PURE data.
		static std::shared_ptr<InternalData> s_data;	//!< data internal to the renderer.
	};
}