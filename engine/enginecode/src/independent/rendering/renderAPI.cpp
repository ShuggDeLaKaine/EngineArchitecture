/** \file renderAPI.cpp */

#include "engine_pch.h"
#include "systems/log.h"

#include "rendering/renderAPI.h"
#include "rendering/indexBuffer.h"
#include "rendering/vertexBuffer.h"
#include "rendering/vertexArray.h"
#include "rendering/shaders.h"

#include "platform/OpenGL/OpenGLIndexBuffer.h"
#include "platform/OpenGL/OpenGLVertexBuffer.h"
#include "platform/OpenGL/OpenGLVertexArray.h"
#include "platform/OpenGL/OpenGLShader.h"


namespace Engine
{
	//initalise s_API here, as static and needs doing outside of class.
	RenderAPI::API RenderAPI::s_API = RenderAPI::API::OpenGL;

	//
	IndexBuffer* IndexBuffer::create(uint32_t* indices, uint32_t count)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::error("No rendering API; not supported, SORT IT OUT!");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLIndexBuffer(indices, count);

		case RenderAPI::API::Direct3D:
			Log::error("DIRECT3D rendering API is not supported at this time.");
			break;
		case RenderAPI::API::Vulkan:
			Log::error("VULKAN rendering API is not supported at this time.");
			break;
		}

		//otherwise return nullptr.
		return nullptr;
	}

	//
	VertexBuffer* VertexBuffer::create(void* vertices, uint32_t size, BufferLayout layout)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::error("No rendering API; not supported, SORT IT OUT!");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLVertexBuffer(vertices, size, layout);

		case RenderAPI::API::Direct3D:
			Log::error("DIRECT3D rendering API is not supported at this time.");
			break;
		case RenderAPI::API::Vulkan:
			Log::error("VULKAN rendering API is not supported at this time.");
			break;
		}

		//otherwise return nullptr.
		return nullptr;
	}

	//
	VertexArray* VertexArray::create()
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::error("No rendering API; not supported, SORT IT OUT!");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLVertexArray();

		case RenderAPI::API::Direct3D:
			Log::error("DIRECT3D rendering API is not supported at this time.");
			break;
		case RenderAPI::API::Vulkan:
			Log::error("VULKAN rendering API is not supported at this time.");
			break;
		}

		//otherwise return nullptr.
		return nullptr;
	}

	//
	Shaders* Shaders::create(const char* vertexFilePath, const char* fragmentFilePath)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::error("No rendering API; not supported, SORT IT OUT!");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLShader(vertexFilePath, fragmentFilePath);

		case RenderAPI::API::Direct3D:
			Log::error("DIRECT3D rendering API is not supported at this time.");
			break;
		case RenderAPI::API::Vulkan:
			Log::error("VULKAN rendering API is not supported at this time.");
			break;
		}

		//otherwise return nullptr.
		return nullptr;
	}

	//
	Shaders* Shaders::create(const char* filePath)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::error("No rendering API; not supported, SORT IT OUT!");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLShader(filePath);

		case RenderAPI::API::Direct3D:
			Log::error("DIRECT3D rendering API is not supported at this time.");
			break;
		case RenderAPI::API::Vulkan:
			Log::error("VULKAN rendering API is not supported at this time.");
			break;
		}

		//otherwise return nullptr.
		return nullptr;
	}
}