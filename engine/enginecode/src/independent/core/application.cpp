/** \file application.cpp */

#include "engine_pch.h"

#include <glad/glad.h>

#include "core/application.h"

#ifdef  NG_PLATFORM_WINDOWS
/*
	#include "platform/windows/win32System.h"
	#include "platform/windows/winTimer.h"
#else
*/
	#include "platform/GLFW/GLFWSystem.h"
#endif //  NG_PLATFORM_WINDOWS

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "platform/OpenGL/OpenGLVertexArray.h"
#include "platform/OpenGL/OpenGLShader.h"
#include "platform/OpenGL/OpenGLTexture.h"

namespace Engine {
	// Set static vars
	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		if (s_instance == nullptr)
		{
			s_instance = this;
		}

		//start the systems.

		//start the log system.
		m_logSystem.reset(new Log);
		m_logSystem->start();

		//start the windows system.
#ifdef NG_PLATFORM_WINDOWS
		/*
		m_windowsSystem.reset(new Win32System);
#else
*/
		m_windowsSystem.reset(new GLFWSystem);
#endif
		m_windowsSystem->start();

		//reset & start the timer.
		//if on a windows platform then make a windows timer, otherwise make a chrono timer.
		//NOTE - not a system! Don't need to stop it.
#ifdef  NG_PLATFORM_WINDOWS
		/*
		m_timer.reset(new WinTimer);
#else
*/
		m_timer.reset(new ChronoTimer);
#endif //  NG_PLATFORM_WINDOWS
		m_timer->start();

		//modify the window a little.
		WindowProperties properties("MY FIRST WINDOW", 800, 600, false);
		//create a default window.
		m_window.reset(Window::createWindow(properties));

		//start the random number syste,
		m_ranNumSytem.reset(new RandomNumberGenerator);
		m_ranNumSytem->start();

		//calling function that binds all the callbacks for the event types.
		bindAllEventsTypes();

		InputPoller::setNativeWindow(m_window->getNativeWindow());

		m_timer->reset();
	}

	void Application::bindAllEventsTypes()
	{
		//window events.
		m_window->getEventHandler().setOnWindowCloseCallback(std::bind(&Application::onWindowClose, this, std::placeholders::_1));
		m_window->getEventHandler().setOnWindowResizeCallback(std::bind(&Application::onWindowResize, this, std::placeholders::_1));
		m_window->getEventHandler().setOnWindowMoveCallback(std::bind(&Application::onWindowMove, this, std::placeholders::_1));
		m_window->getEventHandler().setOnWindowFocusCallback(std::bind(&Application::onWindowFocus, this, std::placeholders::_1));
		m_window->getEventHandler().setOnWindowLostFocusCallback(std::bind(&Application::onWindowLostFocus, this, std::placeholders::_1));

		//key events.
		m_window->getEventHandler().setOnKeyPressCallback(std::bind(&Application::onKeyPressed, this, std::placeholders::_1));
		m_window->getEventHandler().setOnKeyReleaseCallback(std::bind(&Application::onKeyReleased, this, std::placeholders::_1));

		//mouse events.
		m_window->getEventHandler().setOnMouseMoveCallback(std::bind(&Application::onMouseMove, this, std::placeholders::_1));
		m_window->getEventHandler().setOnMouseButtonPressCallback(std::bind(&Application::onMouseButtonPress, this, std::placeholders::_1));
		m_window->getEventHandler().setOnMouseButtonReleaseCallback(std::bind(&Application::onMouseButtonRelease, this, std::placeholders::_1));
		m_window->getEventHandler().setOnMouseScrollCallback(std::bind(&Application::onMouseScroll, this, std::placeholders::_1));
	}

	bool Application::onWindowClose(WindowCloseEvent & event)
	{
		event.handleEvent(true);
		//Log::info("Window Closed Event: {0}");
		m_running = false;
		return event.isEventHandled();
	}

	bool Application::onWindowResize(WindowResizeEvent & event)
	{
		event.handleEvent(true);
		//Log::info("Window Resized Event: ({0}, {1})", event.getWindowSize().x, event.getWindowSize().y);
		return event.isEventHandled();
	}

	bool Application::onWindowMove(WindowMoveEvent & event)
	{
		event.handleEvent(true);
		//Log::info("Window Move Event: ({0}, {1})", event.getWindowPosition().x, event.getWindowPosition().y);
		return event.isEventHandled();
	}

	bool Application::onWindowFocus(WindowFocusEvent & event)
	{
		event.handleEvent(true);
		//Log::info("Window Focus Event: {0}");
		return event.isEventHandled();
	}

	bool Application::onWindowLostFocus(WindowLostFocusEvent & event)
	{
		event.handleEvent(true);
		//Log::info("Window Lost Focus Event: {0}");
		return event.isEventHandled();
	}

	bool Application::onKeyPressed(KeyPressedEvent & event)
	{
		event.handleEvent(true);
		//Log::info("Key Pressed Event: key: {0}, repeat: {1}", event.getKeycode(), event.getRepeatCount());
		if (event.getKeycode() == NG_KEY_SPACE)
		{
			Log::error("SPACEBAR PRESSED");
		}
		return event.isEventHandled();
	}

	bool Application::onKeyReleased(KeyReleasedEvent & event)
	{
		event.handleEvent(true);
		//Log::info("Key Released Event: key: {0}", event.getKeycode());
		if (event.getKeycode() == NG_KEY_UP)
		{
			Log::error("UP RELEASED");
		}
		return event.isEventHandled();
	}

	bool Application::onMouseMove(MouseMovementEvent & event)
	{
		event.handleEvent(true);
		//Log::info("Mouse Move Event: ({0}, {1})", event.getMousePosition().x, event.getMousePosition().y);
		return event.isEventHandled();
	}

	bool Application::onMouseButtonPress(MouseButtonPressEvent & event)
	{
		event.handleEvent(true);
		//Log::info("Mouse Button Pressed Event: {0}", event.getButtonPressed());
		return event.isEventHandled();
	}

	bool Application::onMouseButtonRelease(MouseButtonReleaseEvent & event)
	{
		event.handleEvent(true);
		//Log::info("Mouse Button Released Event: {0}", event.getButtonReleased());
		return event.isEventHandled();
	}

	bool Application::onMouseScroll(MouseScrollEvent & event)
	{
		event.handleEvent(true);
		//Log::info("Mouse Scroll Event: ({0}, {1})", static_cast<float>(event.getXMouseScroll()), static_cast<float>(event.getYMouseScroll()));
		return event.isEventHandled();
	}


	Application::~Application()
	{
		//stop the systems in the REVERSE ORDER to how they start.
		//stop the random number system.
		m_ranNumSytem->stop();

		//stop the windows system.
		m_windowsSystem->stop();

		//stop the log system.
		m_logSystem->stop();
	}


	void Application::run()
	{

#pragma region RAW_DATA

		float cubeVertices[8 * 24] = {
			//	 <------ Pos ------>  <--- normal --->  <-- UV -->
				 0.5f,  0.5f, -0.5f,  0.f,  0.f, -1.f,  0.f,   0.f,
				 0.5f, -0.5f, -0.5f,  0.f,  0.f, -1.f,  0.f,   0.5f,
				-0.5f, -0.5f, -0.5f,  0.f,  0.f, -1.f,  0.33f, 0.5f,
				-0.5f,  0.5f, -0.5f,  0.f,  0.f, -1.f,  0.33f, 0.f,

				-0.5f, -0.5f, 0.5f,   0.f,  0.f,  1.f,  0.33f, 0.5f,
				 0.5f, -0.5f, 0.5f,   0.f,  0.f,  1.f,  0.66f, 0.5f,
				 0.5f,  0.5f, 0.5f,   0.f,  0.f,  1.f,  0.66f, 0.f,
				-0.5f,  0.5f, 0.5f,   0.f,  0.f,  1.f,  0.33,  0.f,

				-0.5f, -0.5f, -0.5f,  0.f, -1.f,  0.f,  1.f,   0.f,
				 0.5f, -0.5f, -0.5f,  0.f, -1.f,  0.f,  0.66f, 0.f,
				 0.5f, -0.5f, 0.5f,   0.f, -1.f,  0.f,  0.66f, 0.5f,
				-0.5f, -0.5f, 0.5f,   0.f, -1.f,  0.f,  1.0f,  0.5f,

				 0.5f,  0.5f, 0.5f,   0.f,  1.f,  0.f,  0.f,   0.5f,
				 0.5f,  0.5f, -0.5f,  0.f,  1.f,  0.f,  0.f,   1.0f,
				-0.5f,  0.5f, -0.5f,  0.f,  1.f,  0.f,  0.33f, 1.0f,
				-0.5f,  0.5f, 0.5f,   0.f,  1.f,  0.f,  0.3f,  0.5f,

				-0.5f,  0.5f, 0.5f,  -1.f,  0.f,  0.f,  0.66f, 0.5f,
				-0.5f,  0.5f, -0.5f, -1.f,  0.f,  0.f,  0.33f, 0.5f,
				-0.5f, -0.5f, -0.5f, -1.f,  0.f,  0.f,  0.33f, 1.0f,
				-0.5f, -0.5f, 0.5f,  -1.f,  0.f,  0.f,  0.66f, 1.0f,

				 0.5f, -0.5f, -0.5f,  1.f,  0.f,  0.f,  1.0f,  1.0f,
				 0.5f,  0.5f, -0.5f,  1.f,  0.f,  0.f,  1.0f,  0.5f,
				 0.5f,  0.5f, 0.5f,   1.f,  0.f,  0.f,  0.66f, 0.5f,
				 0.5f, -0.5f, 0.5f,   1.f,  0.f,  0.f,  0.66f, 1.0f
		};

		float pyramidVertices[6 * 16] = {
			//	 <------ Pos ------>  <--- colour ---> 
				-0.5f, -0.5f, -0.5f,  0.8f, 0.2f, 0.8f, //  square Magneta
				 0.5f, -0.5f, -0.5f,  0.8f, 0.2f, 0.8f,
				 0.5f, -0.5f,  0.5f,  0.8f, 0.2f, 0.8f,
				-0.5f, -0.5f,  0.5f,  0.8f, 0.2f, 0.8f,

				-0.5f, -0.5f, -0.5f,  0.2f, 0.8f, 0.2f,  //triangle Green
				-0.5f, -0.5f,  0.5f,  0.2f, 0.8f, 0.2f,
				 0.0f,  0.5f,  0.0f,  0.2f, 0.8f, 0.2f,

				-0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.f, //triangle Red
				 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.f,
				 0.0f,  0.5f,  0.0f,  1.0f, 0.0f, 0.f,

				 0.5f, -0.5f,  0.5f,  0.8f, 0.8f, 0.2f, //  triangle Yellow
				 0.5f, -0.5f, -0.5f,  0.8f, 0.8f, 0.2f,
				 0.0f,  0.5f,  0.0f,  0.8f, 0.8f, 0.2f,

				 0.5f, -0.5f, -0.5f,  0.f, 0.2f, 1.0f,//  triangle Blue
				-0.5f, -0.5f, -0.5f,  0.f, 0.2f, 1.0f,
				 0.0f,  0.5f,  0.0f,  0.f, 0.2f, 1.0f
		};

		uint32_t pyramidIndices[3 * 6] =
		{
			0, 1, 2,
			2, 3, 0,
			4, 5, 6,
			7, 8, 9,
			10, 11, 12,
			13, 14, 15
		};

		uint32_t cubeIndices[3 * 12] = {
			0, 1, 2,
			2, 3, 0,
			4, 5, 6,
			6, 7, 4,
			8, 9, 10,
			10, 11, 8,
			12, 13, 14,
			14, 15, 12,
			16, 17, 18,
			18, 19, 16,
			20, 21, 22,
			22, 23, 20
		};
#pragma endregion

#pragma region GL_BUFFERS
		//SETTING UP THE CUBE.
		std::shared_ptr<OpenGLVertexArray> cubeVAO;
		std::shared_ptr<OpenGLVertexBuffer> cubeVBO;
		std::shared_ptr<OpenGLIndexBuffer> cubeIBO;
		//creating a buffer layout with its initialiser list (check out bufferLayout.h)
		BufferLayout cubeBL = {
			{ ShaderDataType::Float3, false },
			{ ShaderDataType::Float3, false },
			{ ShaderDataType::Float2, false }
		};
		//create/reset the VAO, VBO & IBO.
		cubeVAO.reset(new OpenGLVertexArray());
		cubeVBO.reset(new OpenGLVertexBuffer(cubeVertices, sizeof(cubeVertices), cubeBL));
		cubeIBO.reset(new OpenGLIndexBuffer(cubeIndices, 36));
		//set the vertex and index buffers.
		cubeVAO->addVertexBuffer(cubeVBO);
		cubeVAO->setIndexBuffer(cubeIBO);


		//SETTING UP THE PYRAMID.
		std::shared_ptr<OpenGLVertexArray> pyramidVAO;
		std::shared_ptr<OpenGLVertexBuffer> pyramidVBO;
		std::shared_ptr<OpenGLIndexBuffer> pyramidIBO;
		//creating a buffer layout with its initialiser list for a pyramid.
		BufferLayout pyramidBL = {
			{ ShaderDataType::Float3, false },
			{ ShaderDataType::Float3, false }
		};
		//create/reset the VAO, VBO & IBO.
		pyramidVAO.reset(new OpenGLVertexArray());
		pyramidVBO.reset(new OpenGLVertexBuffer(pyramidVertices, sizeof(pyramidVertices), pyramidBL));
		pyramidIBO.reset(new OpenGLIndexBuffer(pyramidIndices, 18));
		//set the vertex and index buffers. 
		pyramidVAO->addVertexBuffer(pyramidVBO);
		pyramidVAO->setIndexBuffer(pyramidIBO);
#pragma endregion

#pragma region SHADERS

		std::shared_ptr<OpenGLShader> FCShader;
		FCShader.reset(new OpenGLShader("assets/shaders/flatColour.glsl"));

		std::shared_ptr<OpenGLShader> TPShader;
		TPShader.reset(new OpenGLShader("assets/shaders/texturedPhong.glsl"));

#pragma endregion 

#pragma region TEXTURES

		std::shared_ptr<OpenGLTexture> letterTexture;
		letterTexture.reset(new OpenGLTexture("assets/textures/letterCube.png"));

		std::shared_ptr<OpenGLTexture> numberTexture;
		numberTexture.reset(new OpenGLTexture("assets/textures/numberCube.png"));

#pragma endregion


		//need a view, a projection (for camera) and a model matrix.
		//two mat4s for the camera.
		glm::mat4 view = glm::lookAt(
			glm::vec3(0.0f, 0.0f, 0.0f),	//eye: aka the position; 0.0f, 0.0f, 0.0f, is origin.
			glm::vec3(0.0f, 0.0f, -1.0f),	//centre: aka which way we're looking, convention is to look down the Z axis
			glm::vec3(0.0f, 1.0f, 0.0f)		//up: make up, up (if that makes sense...)
		);			//matrix for position and orientation.
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);	//matrix for how the camera views the world orthographic or perspective. first param field of view, so the camera ratio.

		//for the transofrm of the models, array as can have a pyramid and a cube.
		glm::mat4 models[3];
		models[0] = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.5f, -5.0f));
		models[1] = glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, -0.5f, -5.0f));
		models[2] = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, -0.5f, -5.0f));


		//create a float for the time step and initialise at 0.
		float timeStep = 0.0f;

		glEnable(GL_DEPTH_TEST);
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);

		while (m_running)
		{
			//update the time step with the timer function getElapsedTime()
			timeStep = m_timer->getElapsedTime();
			m_timer->reset();

			//get the model to rotate (easier to see whether it is a 3d shape)
			for (auto& model: models) model = glm::rotate(model, timeStep, glm::vec3(0.0f, 1.0f, 0.5f));

			//things to do in the frame...
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//draw a PYRAMID.
			//bind the shader FCproram (Flat Coloured shader)
			glUseProgram(FCShader->getRenderID());
			//bind the correct buffers, vertex array and index buffer.
			glBindVertexArray(pyramidVAO->getRenderID());
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pyramidIBO->getRenderID());

			//need a location to upload it, represents a place in the shader we are going to use.
			GLuint location;
			
			//upload all relevant uniforms for projectionm, view and model.
			location = glGetUniformLocation(FCShader->getRenderID(), "u_projection");
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projection));
			location = glGetUniformLocation(FCShader->getRenderID(), "u_view");
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(view));
			location = glGetUniformLocation(FCShader->getRenderID(), "u_model");
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(models[0]));

			//draw the PYRAMID!
			glDrawElements(GL_TRIANGLES, pyramidVAO->getDrawCount(), GL_UNSIGNED_INT, nullptr);

			
			//draw a CUBE.
			//bind the shader (textured phong shader)
			glUseProgram(TPShader->getRenderID());
			//binc the buffers, vertex array and index buffer.
			glBindVertexArray(cubeVAO->getRenderID());
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIBO->getRenderID());

			//upload all relevant uniforms for projectionm, view and model.
			location = glGetUniformLocation(TPShader->getRenderID(), "u_projection");
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projection));
			location = glGetUniformLocation(TPShader->getRenderID(), "u_view");
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(view));
			location = glGetUniformLocation(TPShader->getRenderID(), "u_model");
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(models[1]));

			//now the uniforms for lightColour, lightPos and viewPos; all needed for the Textured Phong shader (otherwise no light, just a black unlit cube). 
			location = glGetUniformLocation(TPShader->getRenderID(), "u_lightColour");
			glUniform3f(location, 1.0f, 1.0f, 1.0f);
			location = glGetUniformLocation(TPShader->getRenderID(), "u_lightPos");
			glUniform3f(location, 1.0f, 4.0f, 6.0f);		//back, up and off to the right.
			location = glGetUniformLocation(TPShader->getRenderID(), "u_viewPos");
			glUniform3f(location, 0.0f, 0.0f, 0.0f);		//just at origin here (0,0,0).
			location = glGetUniformLocation(TPShader->getRenderID(), "u_texData");
			glUniform1i(location, 0);

			//bind the texture that is wanted.
			glBindTexture(GL_TEXTURE_2D, letterTexture->getID());

			//draw the CUBE!
			glDrawElements(GL_TRIANGLES, cubeVAO->getDrawCount(), GL_UNSIGNED_INT, nullptr);
			
			//draw ANOTHER CUBE!
			location = glGetUniformLocation(TPShader->getRenderID(), "u_model");
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(models[2]));
			glBindTexture(GL_TEXTURE_2D, numberTexture->getID());
			glDrawElements(GL_TRIANGLES, cubeVAO->getDrawCount(), GL_UNSIGNED_INT, nullptr);
			
			m_window->onUpdate(timeStep);
		}

		//clean up all the shader & texture stuff.
		//glDeleteTextures(1, &letterTexture);
		//glDeleteTextures(1, &numberTexture);
	}

}

//RAW DATA STUFF
/*
			//shaders MUST match your raw data and buffers.
			//for the pyramid, matches with 3 by 3 (the vertex position and colour)
std::string FCvertSrc = R"(
				#version 440 core
			
				layout(location = 0) in vec3 a_vertexPosition;
				layout(location = 1) in vec3 a_vertexColour;
				out vec3 fragmentColour;
				uniform mat4 u_model;
				uniform mat4 u_view;
				uniform mat4 u_projection;
				void main()
				{
					fragmentColour = a_vertexColour;
					gl_Position =  u_projection * u_view * u_model * vec4(a_vertexPosition,1);
				}
			)";

std::string FCFragSrc = R"(
				#version 440 core
			
				layout(location = 0) out vec4 colour;
				in vec3 fragmentColour;
				void main()
				{
					colour = vec4(fragmentColour, 1.0);
				}
		)";

//textured phong shader, matches with cube (3, 3, 2 of data... vertice positions, normals and then UV)
std::string TPvertSrc = R"(
				#version 440 core
			
				layout(location = 0) in vec3 a_vertexPosition;
				layout(location = 1) in vec3 a_vertexNormal;
				layout(location = 2) in vec2 a_texCoord;
				out vec3 fragmentPos;
				out vec3 normal;
				out vec2 texCoord;
				uniform mat4 u_model;
				uniform mat4 u_view;
				uniform mat4 u_projection;
				void main()
				{
					fragmentPos = vec3(u_model * vec4(a_vertexPosition, 1.0));
					normal = mat3(transpose(inverse(u_model))) * a_vertexNormal;
					texCoord = vec2(a_texCoord.x, a_texCoord.y);
					gl_Position =  u_projection * u_view * u_model * vec4(a_vertexPosition,1.0);
				}
			)";

std::string TPFragSrc = R"(
				#version 440 core
			
				layout(location = 0) out vec4 colour;
				in vec3 normal;
				in vec3 fragmentPos;
				in vec2 texCoord;
				uniform vec3 u_lightPos; 
				uniform vec3 u_viewPos; 
				uniform vec3 u_lightColour;
				uniform sampler2D u_texData;
				void main()
				{
					float ambientStrength = 0.4;
					vec3 ambient = ambientStrength * u_lightColour;
					vec3 norm = normalize(normal);
					vec3 lightDir = normalize(u_lightPos - fragmentPos);
					float diff = max(dot(norm, lightDir), 0.0);
					vec3 diffuse = diff * u_lightColour;
					float specularStrength = 0.8;
					vec3 viewDir = normalize(u_viewPos - fragmentPos);
					vec3 reflectDir = reflect(-lightDir, norm);  
					float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
					vec3 specular = specularStrength * spec * u_lightColour;  
					
					colour = vec4((ambient + diffuse + specular), 1.0) * texture(u_texData, texCoord);
				}
		)";

		//compiling the shaders, 3 stages; two shaders, vertex and fragment.
		//NEED to compile each of these then link to the shader program, 3 stages.

		//OpenGL IDs for the shader program, handles into the shader.
		uint32_t FCprogram, TPprogram;

		//vertex shader stuff...
		//create the vertex shader.
		GLuint FCVertShader = glCreateShader(GL_VERTEX_SHADER);

		//get the source and compile it.
		const GLchar* source = FCvertSrc.c_str();
		glShaderSource(FCVertShader, 1, &source, 0);
		glCompileShader(FCVertShader);

		GLint isCompiled = 0;
		glGetShaderiv(FCVertShader, GL_COMPILE_STATUS, &isCompiled);
		//check it has compiled, error message if not and delete it.
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(FCVertShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(FCVertShader, maxLength, &maxLength, &infoLog[0]);
			Log::error("Shader compile error: {0}", std::string(infoLog.begin(), infoLog.end()));

			//deleting it here if it has failed to compile.
			glDeleteShader(FCVertShader);
			return;
		}

		//fragment shader stuff...
		//create the fragment shader.
		GLuint FCFragShader = glCreateShader(GL_FRAGMENT_SHADER);

		//give it the source and compile it.
		source = FCFragSrc.c_str();
		glShaderSource(FCFragShader, 1, &source, 0);
		glCompileShader(FCFragShader);

		glGetShaderiv(FCFragShader, GL_COMPILE_STATUS, &isCompiled);
		//check that it has been compiled, delete this AND vertex shader if failed to so.
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(FCFragShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(FCFragShader, maxLength, &maxLength, &infoLog[0]);
			Log::error("Shader compile error: {0}", std::string(infoLog.begin(), infoLog.end()));

			//deleting the fragment AND vertex shaders if it has failed to compile.
			glDeleteShader(FCFragShader);
			glDeleteShader(FCVertShader);

			return;
		}

		//got to link them up with the program.
		//all compile fined, so create the final shader program.
		FCprogram = glCreateProgram();
		//attach the vertex and fragment shaders and link them.
		glAttachShader(FCprogram, FCVertShader);
		glAttachShader(FCprogram, FCFragShader);
		glLinkProgram(FCprogram);

		GLint isLinked = 0;
		glGetProgramiv(FCprogram, GL_LINK_STATUS, (int*)&isLinked);
		//check whether they have linked, if not delete all three shaders.
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(FCprogram, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(FCprogram, maxLength, &maxLength, &infoLog[0]);
			Log::error("Shader linking error: {0}", std::string(infoLog.begin(), infoLog.end()));

			glDeleteProgram(FCprogram);
			glDeleteShader(FCVertShader);
			glDeleteShader(FCFragShader);

			return;
		}

		//now linked, can deattach shaders as done with them, just need the final FCprogram.
		glDetachShader(FCprogram, FCVertShader);
		glDetachShader(FCprogram, FCFragShader);


		//below is pretty much the exact SAME as the shader above, minus some specific Log information.
		GLuint TPVertShader = glCreateShader(GL_VERTEX_SHADER);

		source = TPvertSrc.c_str();
		glShaderSource(TPVertShader, 1, &source, 0);
		glCompileShader(TPVertShader);

		isCompiled = 0;
		glGetShaderiv(TPVertShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(TPVertShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(TPVertShader, maxLength, &maxLength, &infoLog[0]);
			Log::error("Shader compile error: {0}", std::string(infoLog.begin(), infoLog.end()));

			glDeleteShader(TPVertShader);
			return;
		}

		GLuint TPFragShader = glCreateShader(GL_FRAGMENT_SHADER);

		source = TPFragSrc.c_str();
		glShaderSource(TPFragShader, 1, &source, 0);
		glCompileShader(TPFragShader);

		glGetShaderiv(TPFragShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(TPFragShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(TPFragShader, maxLength, &maxLength, &infoLog[0]);
			Log::error("Shader compile error: {0}", std::string(infoLog.begin(), infoLog.end()));

			glDeleteShader(TPFragShader);
			glDeleteShader(TPVertShader);

			return;
		}

		TPprogram = glCreateProgram();
		glAttachShader(TPprogram, TPVertShader);
		glAttachShader(TPprogram, TPFragShader);
		glLinkProgram(TPprogram);

		isLinked = 0;
		glGetProgramiv(TPprogram, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(TPprogram, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(TPprogram, maxLength, &maxLength, &infoLog[0]);
			Log::error("Shader linking error: {0}", std::string(infoLog.begin(), infoLog.end()));

			glDeleteProgram(TPprogram);
			glDeleteShader(TPVertShader);
			glDeleteShader(TPFragShader);

			return;
		}

		glDetachShader(TPprogram, TPVertShader);
		glDetachShader(TPprogram, TPFragShader);
		*/


//BUFFER STUFF
		/*
		uint32_t cubeVAO, cubeVBO, cubeIBO;

		glCreateVertexArrays(1, &cubeVAO);
		glBindVertexArray(cubeVAO);

		glCreateBuffers(1, &cubeVBO);
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

		glCreateBuffers(1, &cubeIBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // position
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // Normal
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // UV co-ords


				uint32_t pyramidVAO, pyramidVBO, pyramidIBO;

		glCreateVertexArrays(1, &pyramidVAO);
		glBindVertexArray(pyramidVAO);

		glCreateBuffers(1, &pyramidVBO);
		glBindBuffer(GL_ARRAY_BUFFER, pyramidVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertices), pyramidVertices, GL_STATIC_DRAW);

		glCreateBuffers(1, &pyramidIBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pyramidIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pyramidIndices), pyramidIndices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // Position
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // Colour
		*/

//THIS WAS ALL IN THE APPLICATION::RUN()
//USED FOR TESTING STUFF.
/*

			//float accumulatedTime = 0.0f;
			//accumulatedTime += timeStep;


			//***event testing***
			//testing that the window closes after a second.
			if (accumulatedTime > 1.0f)
			{
				WindowCloseEvent close;
				auto& callback = m_eventHandler.getOnWindowCloseCallback();
				callback(close);
			}
			
			//***logging system tests***
			//Log::trace("Hey Hey HEY! {0} {1}", 42, "How long is piece of string");
			//Log::file("Hey Hey HEY! {0} {1}", 42, "How long is piece of string");
			//Log::trace("FPS {0}", 1.0f / timeStep);

			//***random number generator system tests***
			//Log::trace("{0}", RandomNumberGenerator::uniformIntBetween(-10, 10));
			//Log::trace("{0}", RandomNumberGenerator::uniformFloatBetween(-10, 10));
			//Log::trace("{0}", RandomNumberGenerator::normalIntBetween(10.0f, 2.5f));
			//Log::trace("{0}", RandomNumberGenerator::normalFloatBetween(5.0f, 1.25f));

			
			//testing key and mouse button press.
			if (InputPoller::isKeyPressed(NG_KEY_W))
			{
				Log::error("W Pressed");
			}
			if (InputPoller::isMouseButtonPressed(NG_MOUSE_BUTTON_1))
			{
				Log::error("LEFT Mouse Button Pressed");
			}
			if (InputPoller::isMouseButtonPressed(NG_MOUSE_BUTTON_2))
			{
				Log::error("RIGHT Mouse Button Pressed");
			}
			if (InputPoller::isMouseButtonPressed(NG_MOUSE_BUTTON_3))
			{
				Log::error("MIDDLE Mouse Button Pressed");
			}

			//Log::trace("Current Mouse Position: ({0}, {1})", InputPoller::getMouseXPos(), InputPoller::getMouseYPos());
			
*/