/** \file OpenGLShader.h */
#pragma once

#include <cstdint>
#include <glm/glm.hpp>

namespace Engine
{

	/** \class OpenGLShader
	* class to take in text, compile into shader program.
	*/
	class OpenGLShader
	{
	public:
		OpenGLShader(const char* vertexFilePath, const char* fragmentFilePath);	//!< constructor, takes the filepaths for text files.
		OpenGLShader(const char* filePath);						//!< another constructor, just takes a single file path, can put all shaders into a single file.
		~OpenGLShader();										//!< destructor.
		uint32_t getRenderID() const { return m_OpenGL_ID; };	//!< gets and returns the renderer ID.

		void uploadInt(const char* name, int value);			//!< uploading a texture (just an int) 
		void uploadFloat(const char* name, float value);				//!< upload single float.
		void uploadFloat2(const char* name, const glm::vec2& value);	//!< upload 2 float combination.
		void uploadFloat3(const char* name, const glm::vec3& value);	//!< upload 3 float combination.
		void uploadFloat4(const char* name, const glm::vec4& value);	//!< upload 4 float combination.
		void uploatMat4(const char* name, const glm::mat4& value);		//!< upload model matrices.

	private:
		uint32_t m_OpenGL_ID;		//!< OpenGL render identifier. 
		//TO DO: Below will work fine for these two shaders, for any further shaders (ie tessellation, geometry) this will need EXPANDING.
		void compileAndLink(const char* vertexShaderScr, const char* fragmentShaderScr);		//!< compiles and links shaders, just the two at the moment.
	};


}
