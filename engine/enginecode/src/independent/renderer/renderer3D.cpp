/** renderer3D.cpp */

#include "engine_pch.h"
#include "renderer/renderer3D.h"

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

	}

	void Renderer3D::end()
	{
		s_data->sceneWideUniforms.clear();
	}
}