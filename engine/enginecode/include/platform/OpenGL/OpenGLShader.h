/** \file OpenGLShader.h */
#pragma once

#include "rendering/shaders.h"

namespace Engine
{
	/** \class OpenGLShader
	* class to take in text, compile into shader program.
	*/
	class OpenGLShader : public Shaders
	{
	public:
		OpenGLShader(const char* vertexFilePath, const char* fragmentFilePath);	//!< constructor, takes the filepaths for text files.
		OpenGLShader(const char* filePath);								//!< constructor, takes a single file path, can put all shaders into a single file.
		virtual ~OpenGLShader();										//!< destructor.
		virtual uint32_t getRenderID() const { return m_OpenGL_ID; };	//!< gets and returns the renderer ID.
		 
		virtual void uploadInt(const char* name, int value) override;			//!< uploading a texture (just an int) 
		virtual void uploadFloat(const char* name, float value) override;				//!< upload single float.
		virtual void uploadFloat2(const char* name, const glm::vec2& value) override;	//!< upload 2 float combination.
		virtual void uploadFloat3(const char* name, const glm::vec3& value) override;	//!< upload 3 float combination.
		virtual void uploadFloat4(const char* name, const glm::vec4& value) override;	//!< upload 4 float combination.
		virtual void uploatMat4(const char* name, const glm::mat4& value) override;		//!< upload model matrices.

	private:
		uint32_t m_OpenGL_ID;		//!< OpenGL render identifier. 
		//TO DO: Below will work fine for these two shaders, for any further shaders (ie tessellation, geometry) this will need EXPANDING.
		void compileAndLink(const char* vertexShaderScr, const char* fragmentShaderScr);		//!< compiles and links shaders, just the two at the moment.
	};


}
