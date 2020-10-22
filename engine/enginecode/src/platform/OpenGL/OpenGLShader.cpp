/** \file OpenGLShader.cpp */

#include "engine_pch.h"
#include <fstream>
#include <string>
#include <array>
#include "platform/OpenGL/OpenGLShader.h"
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
				vertexSource += (line + "\n");
			}
		}
		else
		{
			//otherwise throw error and return.
			Log::error("NOT able to open VERTEX SHADER source: {0}", vertexFilePath);
			return;
		}
		//close the file.
		handle.close();

		//fragment shader file path.
		std::fstream handle(fragmentFilePath, std::ios::in);
		//check that it has fragment filepath
		if (handle.is_open())
		{
			//opens fine, so need to read it...
			//whilst open, read it in line by line, adding it to source.
			while (getline(handle, line))
			{
				fragmentSource += (line + "\n");
			}
		}
		else
		{
			//otherwise throw error and return.
			Log::error("NOT able to open FRAGMENT SHADER source: {0}", fragmentFilePath);
			return;
		}
		//close the file.
		handle.close();

		//got source for each, so compile and link them, converting them to c_strings.
		compileAndLink(vertexSource.c_str(), fragmentSource.c_str());
	}

	OpenGLShader::OpenGLShader(const char * filePath)
	{
		//enum Region for different types of shaders.
		//NOTE - not the best to only declare locally.
		//TO DO - will need to move elsewhere soon.
		enum Region { None = -1, Vertex = 0, Fragment, Geometry, TessellationControl, TessellationEvalution, Compute };

		//declarations.
		std::string line;
		std::array<std::string, Region::Compute + 1> source;		//adding one to Compute (which 5 in the enum) will make an array of 6 elements.
		uint32_t region = Region::None;		// index for which region we are in, initialised to None(-1).

		//the single file path.
		std::fstream handle(filePath, std::ios::in);
		//check that it has the file path.
		if (handle.is_open())
		{
			//opens fine, so need to read it...
			//whilst open, read it in line by line, adding it to source.
			while (getline(handle, line))
			{
				//which region are we in? 
				//continue is to skip of the #vertex /#fragment etc part of the text document.
				if (line.find("#region Vertex") != std::string::npos) {
					region = Region::Vertex; continue;
				}
				if (line.find("#region Fragment") != std::string::npos) {
					region = Region::Fragment; continue;
				}
				if (line.find("#region Geometry") != std::string::npos) {
					region = Region::Geometry; continue;
				}
				if (line.find("#region TessellationControl") != std::string::npos) {
					region = Region::TessellationControl; continue;
				}
				if (line.find("#region TessellationEvalution") != std::string::npos) {
					region = Region::TessellationEvalution; continue;
				}
				if (line.find("#region Compute") != std::string::npos) {
					region = Region::Compute; continue;
				}

				//if region not = none (it has a value), then stick it into region.
				if(region != Region::None)
				{
					source[region] += (line + "\n");
				}
			}
		}
		else
		{
			//otherwise throw error and return.
			Log::error("NOT able to open SINGLE FILE source: {0}", source);
			return;
		}
		//close the file.
		handle.close();

		//got source, so compile and link, converting them to c_strings.
		//TODO: this below will only compile Vertex and Fragment shaders, need expansion to include other shaders.
		//HOW: pass it an array maybe, integar of flags to say which is present, flag system could work.
		compileAndLink(source[Region::Vertex].c_str(), source[Region::Fragment].c_str());
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