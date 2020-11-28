/** renderer3D.cpp */

#include "engine_pch.h"
#include "renderer/renderer3D.h"
#include <glad/glad.h>

namespace Engine
{
	//instaniate static variables.
	std::shared_ptr<Renderer3D::InternalData> Renderer3D::s_data = nullptr;

	void Renderer3D::init()
	{
		s_data.reset(new InternalData);

		unsigned char whitePixel[4] = { 255, 255, 255, 255 };		//a white pixel.
		s_data->defaultTexture.reset(Textures::create(1, 1, 4, whitePixel));

		s_data->defaultTint = { 1.0f, 1.0f, 1.0f, 1.0f };
	}

	void Renderer3D::begin(const SceneWideUniforms & sceneWideUniforms)
	{
		s_data->sceneWideUniforms = sceneWideUniforms;
	}

	void Renderer3D::submit(const std::shared_ptr<VertexArray>& geometry, const std::shared_ptr<Material> material, const glm::mat4 & model)
	{
		//TO DO - to make API agnostic (below isn't, using opengl function), just put the bind functions into my classes. Need a shader bind function and VAO bind function, maybe submit render function too.
		//first bind the shader.
		glUseProgram(material->getShader()->getID());

		//apply scene wide uniforms; iterate over them.
		//TO DO - later replace with UBOS
		for (auto& dataPair : s_data->sceneWideUniforms)
		{
			const char* uniformName = dataPair.first;
			ShaderDataType& sdt = dataPair.second.first;
			void * addressValue = dataPair.second.second;

			switch (sdt)
			{
			case ShaderDataType::Int:
				material->getShader()->uploadInt(uniformName, *(int *)addressValue);
				break;
			case ShaderDataType::Float3:
				material->getShader()->uploadFloat3(uniformName, *(glm::vec3 *)addressValue);
				break;
			case ShaderDataType::Float4:
				material->getShader()->uploadFloat4(uniformName, *(glm::vec4 *)addressValue);
				break;
			case ShaderDataType::Mat4:
				material->getShader()->uploadMat4(uniformName, *(glm::mat4 *)addressValue);
				break;
			}
		}

		//apply the material uniforms (per draw uniforms).
		material->getShader()->uploadMat4("u_model", model);		//everything will have a model applied, so do this here before checking.

		if (material->isFlagSet(Material::flag_texture))			//check whether the flags are set; firstly with the texture.
		{
			glBindTexture(GL_TEXTURE_2D, material->getTexture()->getID());
		} else
		{
			glBindTexture(GL_TEXTURE_2D, s_data->defaultTexture->getID());
		}
		material->getShader()->uploadInt("u_texData", 0);


		if (material->isFlagSet(Material::flag_tint))				//now check whether the tint flag is set.
		{
			material->getShader()->uploadFloat4("u_tint", material->getTint());
		} else
		{
			material->getShader()->uploadFloat4("u_tint", s_data->defaultTint);
		}

		//bind geometry (VAO & IBO).
		glBindVertexArray(geometry->getID());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->setIndexBuffer()->getID());

		//finally, submit the draw call.
		glDrawElements(GL_TRIANGLES, geometry->getDrawCount(), GL_UNSIGNED_INT, nullptr);

	}

	void Renderer3D::end()
	{
		s_data->sceneWideUniforms.clear();
	}
}