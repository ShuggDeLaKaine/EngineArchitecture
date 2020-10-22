/** \file OpenGLShader.cpp */

#include "engine_pch.h"
#include "platform/OpenGL/OpenGLShader.h"
#include <fstream>
#include <string>
#include "systems/log.h"

namespace Engine
{
	OpenGLShader::OpenGLShader(const char * vertexFilePath, const char * fragmentFilePath)
	{
		//declarations.
		std::string line;
		std::string vertexSource;
		std::string fragmentSource;
		
		//vertex shader file path.
		std::fstream handle(vertexFilePath, std::ios::in);
		//check that it has vertex file path.
		if (handle.is_open())
		{
			//opens fine, so need to read it...
			//whilst open, read it in line by line, adding it to source.
			while (getline(handle, line))
			{
				vertexSource += line;
			}
		}
		else
		{
			//otherwise throw error and return.
			Log::error("NOT able to open VERTEX SHADER source: {0}", vertexFilePath);
			return;
		}

		//fragment shader file path.
		std::fstream handle(fragmentFilePath, std::ios::in);
		//check that it has fragment filepath
		if (handle.is_open())
		{
			//opens fine, so need to read it...
			//whilst open, read it in line by line, adding it to source.
			while (getline(handle, line))
			{
				fragmentSource += line;
			}
		}
		else
		{
			//otherwise throw error and return.
			Log::error("NOT able to open FRAGMENT SHADER source: {0}", fragmentFilePath);
			return;
		}

		//got source for each, so compile and link them, converting them to c_strings
		compileAndLink(vertexSource.c_str(), fragmentSource.c_str());

	}

	OpenGLShader::OpenGLShader(const char * filePath)
	{
		//enum for different types of shaders.
		//NOTE - not the best to only declare locally, will need to move elsewhere soon.
		enum{None = -1, Vertex = 0, Fragment, Geometry, TessellationControl, TessellationEvalution, Compute };

		//declarations.
		std::string line;
		std::string source;

		//the single file path.
		std::fstream handle(filePath, std::ios::in);
		//check that it has the file path.
		if (handle.is_open())
		{
			//opens fine, so need to read it...
			//whilst open, read it in line by line, adding it to source.
			while (getline(handle, line))
			{
				source += line;
			}
		}
		else
		{
			//otherwise throw error and return.
			Log::error("NOT able to open SINGLE FILE source: {0}", source);
			return;
		}
	}

	OpenGLShader::~OpenGLShader()
	{

	}

	void OpenGLShader::uploadInt(const char * name, int value)
	{

	}

	void OpenGLShader::uploadFloat(const char * name, float value)
	{

	}

	void OpenGLShader::uploadFloat2(const char * name, const glm::vec2 & value)
	{

	}

	void OpenGLShader::uploadFloat3(const char * name, const glm::vec3 & value)
	{

	}

	void OpenGLShader::uploadFloat4(const char * name, const glm::vec4 & value)
	{

	}

	void OpenGLShader::uploatMat4(const char * name, const glm::mat4 & value)
	{

	}

	void OpenGLShader::compileAndLink(const char * vertexShader, const char * fragmentShader)
	{

	}

}