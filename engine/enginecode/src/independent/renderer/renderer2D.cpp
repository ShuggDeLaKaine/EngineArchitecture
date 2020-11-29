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

	void Renderer2D::begin(const SceneWideUniforms & swu)
	{

	}

	void Renderer2D::submit(const Quad & quad, const glm::vec4 & tint)
	{

	}

	void Renderer2D::end()
	{

	}

}