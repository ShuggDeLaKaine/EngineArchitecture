/** \file renderer2D.cpp */

#include "engine_pch.h"
#include "renderer/renderer2D.h"

namespace Engine
{
	//initialise static variables.
	std::shared_ptr<Renderer2D::InternalData> Renderer2D::s_data = nullptr;

	void Renderer2D::init()
	{
		s_data.reset(new InternalData);

		unsigned char whitePixel[4] = { 255, 255, 255, 255 };		//a white pixel to be used in texture..
		s_data->defaultTexture.reset(Textures::create(1, 1, 4, whitePixel));	//set the texture.

		s_data->defaultTint = { 1.0f, 1.0f, 1.0f, 1.0f };		//set the tint.
		s_data->model = glm::mat4(1.0f);						//set the model.

		s_data->shader.reset(Shaders::create("./assets/shaders/quad1.glsl"));		//pass the shader used.
		float vertices[4 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f,
			-0.5f,  0.5f, 0.0f, 1.0f,
			 0.5f,  0.5f, 1.0f, 1.0f,
			 0.5f, -0.5f, 1.0f, 0.0f
		};		//the four vertices of a quad; -0.5 to 0.5 (rather that -1.0 to 1.0) for scaling.

		uint32_t indices[4] = { 0, 1, 2, 3 };				//set the indices.

		//create a local VBO & IBO.
		std::shared_ptr<VertexBuffer> VBO;
		std::shared_ptr<IndexBuffer> IBO;

		s_data->VAO.reset(VertexArray::create());			//create a new vertex array.
		VBO.reset(VertexBuffer::create(vertices, sizeof(vertices), VertexBufferLayout({ ShaderDataType::Float2, ShaderDataType::Float2 })));		//set the VBO with vertices array, its size and a vertexbufferlayout of two float2s.
		IBO.reset(IndexBuffer::create(indices, 4));			//set the IBO wih indices array and its count (4).
		
		//add the VBO & IBO to the VAO.
		s_data->VAO->addVertexBuffer(VBO);
		s_data->VAO->setIndexBuffer(IBO);
	}

	void Renderer2D::begin(const SceneWideUniforms& swu)
	{
		//first bind the shader.
		glUseProgram(s_data->shader->getID());

		//apply scene wide uniforms to the shader. 
		for (auto& dataPair : swu)
		{
			const char* uniformName = dataPair.first;
			ShaderDataType sdt = dataPair.second.first;
			void * addressValue = dataPair.second.second;

			switch (sdt)
			{
			case ShaderDataType::Int:
				s_data->shader->uploadInt(uniformName, *(int *)addressValue);
				break;
			case ShaderDataType::Float3:
				s_data->shader->uploadFloat3(uniformName, *(glm::vec3 *)addressValue);
				break;
			case ShaderDataType::Float4:
				s_data->shader->uploadFloat4(uniformName, *(glm::vec4 *)addressValue);
				break;
			case ShaderDataType::Mat4:
				s_data->shader->uploadMat4(uniformName, *(glm::mat4 *)addressValue);
				break;
			}
		}

		//bind geometry (VAO & IBO).
		glBindVertexArray(s_data->VAO->getID());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_data->VAO->getIndexBuffer()->getID());
	}

	void Renderer2D::submit(const Quad & quad, const glm::vec4 & tint)
	{
		//first bind the default texture.
		glBindTexture(GL_TEXTURE_2D, s_data->defaultTexture->getID());

		//now create the model by translating and scaling the model.
		s_data->model = glm::scale(glm::translate(glm::mat4(1.0f), quad.m_translate), quad.m_scale);

		//upload any per-draw-uniforms.
		s_data->shader->uploadInt("u_texData", 0);				//the texture data.
		s_data->shader->uploadFloat4("u_tint", tint);			//the tint data.
		s_data->shader->uploadMat4("u_model", s_data->model);	//the transform of the model.

		//now draw it; remember using QUADS, not TRIANGLES.
		glDrawElements(GL_QUADS, s_data->VAO->getDrawCount(), GL_UNSIGNED_INT, nullptr);

	}

	void Renderer2D::submit(const Quad & quad, const std::shared_ptr<Textures>& texture)
	{
		//first bind the default texture.
		glBindTexture(GL_TEXTURE_2D, texture->getID());

		//now create the model by translating and scaling the model.
		s_data->model = glm::scale(glm::translate(glm::mat4(1.0f), quad.m_translate), quad.m_scale);

		//upload any per-draw-uniforms.
		s_data->shader->uploadInt("u_texData", 0);						//the texture data.
		s_data->shader->uploadFloat4("u_tint", s_data->defaultTint);	//the tint data.
		s_data->shader->uploadMat4("u_model", s_data->model);			//the transform of the model.

		//now draw it; remember using QUADS, not TRIANGLES.
		glDrawElements(GL_QUADS, s_data->VAO->getDrawCount(), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer2D::submit(const Quad & quad, const glm::vec4 & tint, const std::shared_ptr<Textures>& texture)
	{
		//first bind the default texture.
		glBindTexture(GL_TEXTURE_2D, texture->getID());

		//now create the model by translating and scaling the model.
		s_data->model = glm::scale(glm::translate(glm::mat4(1.0f), quad.m_translate), quad.m_scale);

		//upload any per-draw-uniforms.
		s_data->shader->uploadInt("u_texData", 0);				//the texture data.
		s_data->shader->uploadFloat4("u_tint", tint);			//the tint data.
		s_data->shader->uploadMat4("u_model", s_data->model);	//the transform of the model.

		//now draw it; remember using QUADS, not TRIANGLES.
		glDrawElements(GL_QUADS, s_data->VAO->getDrawCount(), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer2D::submit(const Quad & quad, const glm::vec4 & tint, const std::shared_ptr<Textures>& texture, float angle, bool degrees)
	{
		//check whether bool degrees tue, is so change angle to radians.
		if (degrees)
		{
			angle = glm::radians(angle);
		}

		//first bind the default texture.
		glBindTexture(GL_TEXTURE_2D, texture->getID());

		//now create the model by translating & scaling the model, plus the rotation.
		s_data->model = glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), quad.m_translate), angle, {0, 0, 1}), quad.m_scale);

		//upload any per-draw-uniforms.
		s_data->shader->uploadInt("u_texData", 0);				//the texture data.
		s_data->shader->uploadFloat4("u_tint", tint);			//the tint data.
		s_data->shader->uploadMat4("u_model", s_data->model);	//the transform of the model.

		//now draw it; remember using QUADS, not TRIANGLES.
		glDrawElements(GL_QUADS, s_data->VAO->getDrawCount(), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer2D::submit(const Quad & quad, const glm::vec4 & tint, float angle, bool degrees)
	{
		//check whether bool degrees tue, is so change angle to radians.
		if (degrees)
		{
			angle = glm::radians(angle);
		}

		//first bind the default texture.
		glBindTexture(GL_TEXTURE_2D, s_data->defaultTexture->getID());

		//now create the model by translating & scaling the model, plus the rotation.
		s_data->model = glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), quad.m_translate), angle, { 0, 0, 1 }), quad.m_scale);

		//upload any per-draw-uniforms.
		s_data->shader->uploadInt("u_texData", 0);				//the texture data.
		s_data->shader->uploadFloat4("u_tint", tint);			//the tint data.
		s_data->shader->uploadMat4("u_model", s_data->model);	//the transform of the model.

		//now draw it; remember using QUADS, not TRIANGLES.
		glDrawElements(GL_QUADS, s_data->VAO->getDrawCount(), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer2D::submit(const Quad & quad, const std::shared_ptr<Textures>& texture, float angle, bool degrees)
	{
		//check whether bool degrees tue, is so change angle to radians.
		if (degrees)
		{
			angle = glm::radians(angle);
		}

		//first bind the default texture.
		glBindTexture(GL_TEXTURE_2D, texture->getID());

		//now create the model by translating & scaling the model, plus the rotation.
		s_data->model = glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), quad.m_translate), angle, { 0, 0, 1 }), quad.m_scale);

		//upload any per-draw-uniforms.
		s_data->shader->uploadInt("u_texData", 0);						//the texture data.
		s_data->shader->uploadFloat4("u_tint", s_data->defaultTint);	//the tint data.
		s_data->shader->uploadMat4("u_model", s_data->model);			//the transform of the model.

		//now draw it; remember using QUADS, not TRIANGLES.
		glDrawElements(GL_QUADS, s_data->VAO->getDrawCount(), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer2D::end()
	{

	}

	Quad Quad::createCentreHalfExtents(const glm::vec2& centre, const glm::vec2& halfExtents)
	{
		Quad result;

		result.m_translate = glm::vec3(centre, 0.0f);
		result.m_scale = glm::vec3(halfExtents * 2.0f, 1.0f);

		return result;
	}

	Quad Quad::createCentreHalfExtents(const glm::vec2& centre, float halfExtents)
	{
		Quad result;

		result.m_translate = glm::vec3(centre, 0.0f);
		result.m_scale = glm::vec3(halfExtents * 2.0f, halfExtents * 2.0f, 1.0f);

		return result;
	}

	Quad Quad::createCreateTopLeftSize(const glm::vec2& topLeft, const glm::vec2& size)
	{
		Quad result;

		result.m_translate = glm::vec3(topLeft, 0.0f);
		result.m_scale = glm::vec3(size /** 0.5f*/, 1.0f);

		return result;
	}

	Quad Quad::createCreateTopLeftSize(const glm::vec2& topLeft, float size)
	{
		Quad result;

		result.m_translate = glm::vec3(topLeft, 0.0f);
		result.m_scale = glm::vec3(size * 0.5f, size * 0.5f, 1.0f);

		return result;
	}

	Quad Quad::createCreateTopLeftBottomRight(const glm::vec2& topLeft, const glm::vec2& bottomRight)
	{
		Quad result;

		result.m_translate = glm::vec3(topLeft, 0.0f);
		result.m_scale = glm::vec3((bottomRight.x - topLeft.x), (bottomRight.y - topLeft.y), 1.0f);

		return result;
	}

}