/** \file renderCommands.cpp */

#include "engine_pch.h"
#include "renderer/renderCommands.h"
#include "rendering/renderAPI.h"
#include <glad/glad.h>

namespace Engine
{
	std::function<void(void)> RenderCommandsFactory::getClearColourAndDepthBufferCommand()
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::OpenGL:
			return []() 
			{ 
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
			};
		case RenderAPI::API::Direct3D:
			return std::function<void(void)>();
		case RenderAPI::API::Vulkan:
			return std::function<void(void)>();
		case RenderAPI::API::None:
			return std::function<void(void)>();
		default:
			return std::function<void(void)>();
		}
	}

	std::function<void(void)> RenderCommandsFactory::setClearColourCommand(float r, float g, float b, float a)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::OpenGL:
			return [r, g, b, a]()
			{ 
				glClearColor(r, g, b, a); 
			};
		case RenderAPI::API::Direct3D:
			return std::function<void(void)>();
		case RenderAPI::API::Vulkan:
			return std::function<void(void)>();
		case RenderAPI::API::None:
			return std::function<void(void)>();
		default:
			return std::function<void(void)>();
		}
	}
}