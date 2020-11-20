/** \file application.cpp */

#include "engine_pch.h"
#include "core/application.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef  NG_PLATFORM_WINDOWS
/*
	#include "platform/windows/win32System.h"
	#include "platform/windows/winTimer.h"
#else
*/
	#include "platform/GLFW/GLFWSystem.h"
#endif //  NG_PLATFORM_WINDOWS

#include "rendering/subTexture.h"
#include "rendering/indexBuffer.h"
#include "rendering/vertexBuffer.h"
#include "rendering/vertexArray.h"
#include "rendering/shaders.h"
#include "rendering/textures.h"
#include "rendering/uniformBuffer.h"
#include "rendering/textureUnitManager.h"


namespace Engine {

//temporary class for vertex
/* when done properly need three classes for final engine;
* one for 2d rendering - 2d vertices,
* one for static 3d vertices,
* one for animated 3d vertices. */
#pragma region TEMP_CLASS
	//flat colour vertex class
	class FCVertex
	{
	public:
		FCVertex() : 
			m_position(glm::vec3(0.0f)), 
			m_colour(0) 
		{};		//default constructor, initalising values to 0.
		FCVertex(const glm::vec3& position, const uint32_t& colour) :
			m_position(position), 
			m_colour(colour) 
		{};		//constructor with params for position and colour.
		inline VertexBufferLayout static getBufferLayout() { return s_BufferLayout; };	//!< accessor function to get the static buffer layout.
		glm::vec3 m_position;		//!< vec3 to take position of vertex.
		uint32_t m_colour;			//!< int to take RGB colour of vertex.
	private:
		static VertexBufferLayout s_BufferLayout;
	};

	//textured phong normalised vertex class
	class TPVertexNormalised
	{
	public:
		TPVertexNormalised() :
			m_position(glm::vec3(0.0f)),
			m_normalise({ 0, 0, 0 }),
			m_UVcoords({ 0, 0 })
		{};		//!< default constructor, initalising values to 0.
		TPVertexNormalised(const glm::vec3& position, const std::array<int16_t, 3> normal, const std::array<int16_t, 2> uv) :
			m_position(position),
			m_normalise(normal),
			m_UVcoords(uv)
		{};		//!< constructor with params for a textured phong, position, normal values and UV coordinates.

		glm::vec3 m_position;						//!< vec3 to take position of vertex.
		std::array<int16_t, 3> m_normalise; 		//!< short var to take normalised values. Array used as very easy to pass back arrays from functions.
		std::array<int16_t, 2> m_UVcoords;			//!< short var to take UV coordinate values. Array used as very easy to pass back arrays from functions.

		inline VertexBufferLayout static getBufferLayout() { return s_BufferLayout; }		//!< accessor function to get the static buffer layout.
	private:
		static VertexBufferLayout s_BufferLayout;
	};

#pragma endregion

	//setting temp static vars 
	VertexBufferLayout FCVertex::s_BufferLayout = { ShaderDataType::Float3, { ShaderDataType::Byte4, true } };
	VertexBufferLayout TPVertexNormalised::s_BufferLayout = {{ShaderDataType::Float3, { ShaderDataType::Short3, true }, { ShaderDataType::Short2, true }}, 24 };

	// Set static vars
	Application* Application::s_instance = nullptr;

	Application::Application() 
	{
		/*examples of how you can check the the byte aligned of raw data.
		//use uint32_t's though.
		int alignVec3 = alignof(glm::vec3);
		int alignFCVertex = alignof(FCVertex);
		int sizeofVec3 = sizeof(glm::vec3);
		int sizeofFCVertex = sizeof(FCVertex);
		*/

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

	//TO DO!!! PUT THESE FUNCTIONS SOMEWHERE BETTER!
	//utility functions, so maybe static functions in some class... maybe the BUFFERLAYOUT CLASS.
	std::array<int16_t, 3> normalise(const glm::vec3& norm) 
	{
		std::array<int16_t, 3> result;

		//do x first.
		if (norm.x == 1.0)
			result.at(0) = INT16_MAX;
		else if (norm.x == -1.0)
			result.at(0) = INT16_MIN;
		else
			result.at(0) = static_cast<int16_t>(norm.x * static_cast<float>(INT16_MAX));

		//now y
		if (norm.y == 1.0)
			result.at(1) = INT16_MAX;
		else if (norm.y == -1.0)
			result.at(1) = INT16_MIN;
		else
			result.at(1) = static_cast<int16_t>(norm.y * static_cast<float>(INT16_MAX));

		//and now z
		if (norm.z == 1.0)
			result.at(2) = INT16_MAX;
		else if (norm.z == -1.0)
			result.at(2) = INT16_MIN;
		else
			result.at(2) = static_cast<int16_t>(norm.z * static_cast<float>(INT16_MAX));

		//finally, return the result.
		return result;
	};

	std::array<int16_t, 2> normalise(const glm::vec2& uv)
	{
		std::array<int16_t, 2> result;

		//do x first.
		if (uv.x == 1.0)
			result.at(0) = INT16_MAX;
		else if (uv.x == -1.0)
			result.at(0) = INT16_MIN;
		else
			result.at(0) = static_cast<int16_t>(uv.x * static_cast<float>(INT16_MAX));

		//now y
		if (uv.y == 1.0)
			result.at(1) = INT16_MAX;
		else if (uv.y == -1.0)
			result.at(1) = INT16_MIN;
		else
			result.at(1) = static_cast<int16_t>(uv.y * static_cast<float>(INT16_MAX));

		//finally, return the result.
		return result;
	};

	uint32_t package(const glm::vec4& colour)
	{
		//int to return the result, initialised to 0.
		uint32_t result = 0;

		//vars for each of the colour channels; R, G, B & A.
		//converting them from 0-1 to 0-255, static casting from float to uint32_T
		uint32_t r = (static_cast<uint32_t>(colour.r * 255.0f)) << 0;	//bytewise - 000R
		uint32_t g = (static_cast<uint32_t>(colour.g * 255.0f)) << 8;	//00B0
		uint32_t b = (static_cast<uint32_t>(colour.b * 255.0f)) << 16;	//0G00
		uint32_t a = (static_cast<uint32_t>(colour.a * 255.0f)) << 24;	//A000

		result = (r | g | b | a);
		return result;
	}

	uint32_t package(const glm::vec3& colour)
	{
		return package({ colour.x, colour.y, colour.z, 1.0f});
	}

	void Application::run()
	{


#pragma region RAW_DATA

		std::vector<TPVertexNormalised> cubeVertices(24);
		//										 <------- Pos ------->             <----- normal ----->             <---- UV ---->
		cubeVertices.at(0)	= TPVertexNormalised({ 0.5f,  0.5f, -0.5f}, normalise({ 0.0f,  0.0f, -1.0f}), normalise({0.0f,  0.0f}));
		cubeVertices.at(1)	= TPVertexNormalised({ 0.5f, -0.5f, -0.5f}, normalise({ 0.0f,  0.0f, -1.0f}), normalise({0.0f,  0.5f}));
		cubeVertices.at(2)	= TPVertexNormalised({-0.5f, -0.5f, -0.5f}, normalise({ 0.0f,  0.0f, -1.0f}), normalise({0.33f, 0.5f}));
		cubeVertices.at(3)	= TPVertexNormalised({-0.5f,  0.5f, -0.5f}, normalise({ 0.0f,  0.0f, -1.0f}), normalise({0.33f, 0.0f}));
		cubeVertices.at(4)	= TPVertexNormalised({-0.5f, -0.5f,  0.5f}, normalise({ 0.0f,  0.0f,  1.0f}), normalise({0.33f, 0.5f}));
		cubeVertices.at(5)	= TPVertexNormalised({ 0.5f, -0.5f,  0.5f}, normalise({ 0.0f,  0.0f,  1.0f}), normalise({0.66f, 0.5f}));
		cubeVertices.at(6)	= TPVertexNormalised({ 0.5f,  0.5f,  0.5f}, normalise({ 0.0f,  0.0f,  1.0f}), normalise({0.66f, 0.0f}));
		cubeVertices.at(7)	= TPVertexNormalised({-0.5f,  0.5f,  0.5f}, normalise({ 0.0f,  0.0f,  1.0f}), normalise({0.33f, 0.0f}));
		cubeVertices.at(8)	= TPVertexNormalised({-0.5f, -0.5f, -0.5f}, normalise({ 0.0f, -1.0f,  0.0f}), normalise({1.0f,  0.0f}));
		cubeVertices.at(9)	= TPVertexNormalised({ 0.5f, -0.5f, -0.5f}, normalise({ 0.0f, -1.0f,  0.0f}), normalise({0.66f, 0.0f}));
		cubeVertices.at(10)	= TPVertexNormalised({ 0.5f, -0.5f,  0.5f}, normalise({ 0.0f, -1.0f,  0.0f}), normalise({0.66f, 0.5f}));
		cubeVertices.at(11)	= TPVertexNormalised({-0.5f, -0.5f,  0.5f}, normalise({ 0.0f, -1.0f,  0.0f}), normalise({1.0f,  0.5f}));
		cubeVertices.at(12)	= TPVertexNormalised({ 0.5f,  0.5f,  0.5f}, normalise({ 0.0f,  1.0f,  0.0f}), normalise({0.0f,  0.5f}));
		cubeVertices.at(13)	= TPVertexNormalised({ 0.5f,  0.5f, -0.5f}, normalise({ 0.0f,  1.0f,  0.0f}), normalise({0.0f,  1.0f}));
		cubeVertices.at(14)	= TPVertexNormalised({-0.5f,  0.5f, -0.5f}, normalise({ 0.0f,  1.0f,  0.0f}), normalise({0.33f, 1.0f}));
		cubeVertices.at(15)	= TPVertexNormalised({-0.5f,  0.5f,  0.5f}, normalise({ 0.0f,  1.0f,  0.0f}), normalise({0.3f,  0.5f}));
		cubeVertices.at(16)	= TPVertexNormalised({-0.5f,  0.5f,  0.5f}, normalise({-1.0f,  0.0f,  0.0f}), normalise({0.66f, 0.5f}));
		cubeVertices.at(17)	= TPVertexNormalised({-0.5f,  0.5f, -0.5f}, normalise({-1.0f,  0.0f,  0.0f}), normalise({0.33f, 0.5f}));
		cubeVertices.at(18)	= TPVertexNormalised({-0.5f, -0.5f, -0.5f}, normalise({-1.0f,  0.0f,  0.0f}), normalise({0.33f, 1.0f}));
		cubeVertices.at(19)	= TPVertexNormalised({-0.5f, -0.5f,  0.5f}, normalise({-1.0f,  0.0f,  0.0f}), normalise({0.66f, 1.0f}));
		cubeVertices.at(20)	= TPVertexNormalised({ 0.5f, -0.5f, -0.5f}, normalise({ 1.0f,  0.0f,  0.0f}), normalise({1.0f,  1.0f}));
		cubeVertices.at(21)	= TPVertexNormalised({ 0.5f,  0.5f, -0.5f}, normalise({ 1.0f,  0.0f,  0.0f}), normalise({1.0f,  0.5f}));
		cubeVertices.at(22)	= TPVertexNormalised({ 0.5f,  0.5f,  0.5f}, normalise({ 1.0f,  0.0f,  0.0f}), normalise({0.66f, 0.5f}));
		cubeVertices.at(23)	= TPVertexNormalised({ 0.5f, -0.5f,  0.5f}, normalise({ 1.0f,  0.0f,  0.0f}), normalise({0.66f, 1.0f}));


		std::vector<FCVertex> pyramidVertices(16);
		//	                              <------- Pos ------->           <---- colour ----> 
		pyramidVertices.at(0)  = FCVertex({-0.5f, -0.5f, -0.5f}, package({0.8f, 0.2f, 0.8f}));	//square Magneta
		pyramidVertices.at(1)  = FCVertex({ 0.5f, -0.5f, -0.5f}, package({0.8f, 0.2f, 0.8f}));
		pyramidVertices.at(2)  = FCVertex({ 0.5f, -0.5f,  0.5f}, package({0.8f, 0.2f, 0.8f}));
		pyramidVertices.at(3)  = FCVertex({-0.5f, -0.5f,  0.5f}, package({0.8f, 0.2f, 0.8f}));
		pyramidVertices.at(4)  = FCVertex({-0.5f, -0.5f, -0.5f}, package({0.2f, 0.8f, 0.2f}));	//triangle Green
		pyramidVertices.at(5)  = FCVertex({-0.5f, -0.5f,  0.5f}, package({0.2f, 0.8f, 0.2f}));
		pyramidVertices.at(6)  = FCVertex({ 0.0f,  0.5f,  0.0f}, package({0.2f, 0.8f, 0.2f}));
		pyramidVertices.at(7)  = FCVertex({-0.5f, -0.5f,  0.5f}, package({1.0f, 0.0f, 0.0f}));	//triangle Red
		pyramidVertices.at(8)  = FCVertex({ 0.5f, -0.5f,  0.5f}, package({1.0f, 0.0f, 0.0f}));
		pyramidVertices.at(9)  = FCVertex({ 0.0f,  0.5f,  0.0f}, package({1.0f, 0.0f, 0.0f}));
		pyramidVertices.at(10) = FCVertex({ 0.5f, -0.5f,  0.5f}, package({0.8f, 0.8f, 0.2f}));	//triangle Yellow
		pyramidVertices.at(11) = FCVertex({ 0.5f, -0.5f, -0.5f}, package({0.8f, 0.8f, 0.2f}));
		pyramidVertices.at(12) = FCVertex({ 0.0f,  0.5f,  0.0f}, package({0.8f, 0.8f, 0.2f}));
		pyramidVertices.at(13) = FCVertex({ 0.5f, -0.5f, -0.5f}, package({0.0f, 0.2f, 1.0f}));	//triangle Blue
		pyramidVertices.at(14) = FCVertex({-0.5f, -0.5f, -0.5f}, package({0.0f, 0.2f, 1.0f}));
		pyramidVertices.at(15) = FCVertex({ 0.0f,  0.5f,  0.0f}, package({0.0f, 0.2f, 1.0f}));														  


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
		std::shared_ptr<VertexArray> cubeVAO;
		std::shared_ptr<VertexBuffer> cubeVBO;
		std::shared_ptr<IndexBuffer> cubeIBO;

		/* THIS IS UP IN THE TEMP VERTEX CLASSES
		//creating a buffer layout with its initialiser list (check out bufferLayout.h)
		BufferLayout cubeBL = {
			{ ShaderDataType::Float3, false },
			{ ShaderDataType::Float3, false },
			{ ShaderDataType::Float2, false }
		};
		*/

		//create/reset the VAO, VBO & IBO.
		cubeVAO.reset(VertexArray::create());
		cubeVBO.reset(VertexBuffer::create(cubeVertices.data(), sizeof(TPVertexNormalised) * cubeVertices.size(), TPVertexNormalised::getBufferLayout()));
		cubeIBO.reset(IndexBuffer::create(cubeIndices, 36));

		//set the vertex and index buffers.
		cubeVAO->addVertexBuffer(cubeVBO);
		cubeVAO->setIndexBuffer(cubeIBO);


		//SETTING UP THE PYRAMID.
		std::shared_ptr<VertexArray> pyramidVAO;
		std::shared_ptr<VertexBuffer> pyramidVBO;
		std::shared_ptr<IndexBuffer> pyramidIBO;

		/* THIS IS UP IN THE TEMP VERTEX CLASSES
		//creating a buffer layout with its initialiser list for a pyramid.
		BufferLayout pyramidBL = {
			{ ShaderDataType::Float3, false },
			{ ShaderDataType::Float3, false }
		};
		*/

		//create/reset the VAO, VBO & IBO.
		pyramidVAO.reset(VertexArray::create());
		pyramidVBO.reset(VertexBuffer::create(pyramidVertices.data(), sizeof(FCVertex) * pyramidVertices.size(), FCVertex::getBufferLayout()));
		pyramidIBO.reset(IndexBuffer::create(pyramidIndices, 18));

		//set the vertex and index buffers. 
		pyramidVAO->addVertexBuffer(pyramidVBO);
		pyramidVAO->setIndexBuffer(pyramidIBO);
#pragma endregion

#pragma region SHADERS

		std::shared_ptr<Shaders> FCShader;
		FCShader.reset(Shaders::create("assets/shaders/flatColour.glsl"));

		std::shared_ptr<Shaders> TPShader;
		TPShader.reset(Shaders::create("assets/shaders/texturedPhong.glsl"));

#pragma endregion 

#pragma region TEXTURES

		std::shared_ptr<Textures> letterTexture;
		letterTexture.reset(Textures::create("assets/textures/letterCube.png"));

		std::shared_ptr<Textures> numberTexture;
		numberTexture.reset(Textures::create("assets/textures/numberCube.png"));

		/*
		//testing the sub texture stuff out.
		letterTexture.reset(new OpenGLTexture("assets/textures/letterAndNumberCube.png"));
		//setting the sub textures in this texture file.
		//example - letterCube = start { 0.0f, 0.0f } / end { 1.0f, 0.5f } && numberCube = start { 0.0f, 0.5f } / end { 1.0f, 1.0f }
		SubTexture letterCube(letterTexture, { 0.0f, 0.0f }, { 1.0f, 0.5f });
		SubTexture numberCube(letterTexture, { 0.0f, 0.5f }, { 1.0f, 1.0f });
		*/
#pragma endregion

		//need a view, a projection (for camera) and a model matrix.
		//two mat4s for the camera.
		glm::mat4 view = glm::lookAt(
			glm::vec3(0.0f, 0.0f, 0.0f),	//eye: aka the position; 0.0f, 0.0f, 0.0f, is origin.
			glm::vec3(0.0f, 0.0f, -1.0f),	//centre: aka which way we're looking, convention is to look down the Z axis
			glm::vec3(0.0f, 1.0f, 0.0f)		//up: make up, up (if that makes sense...)
		);			//matrix for position and orientation.
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);	//matrix for how the camera views the world orthographic or perspective. first param field of view, so the camera ratio.

		//CAMERA UBO
		uint32_t blockNum = 0;								//which block are we using.
		//generate, bind and set UBO for camera.
		UniformBufferLayout cameraLayout = { {"u_projection", ShaderDataType::Mat4 }, {"u_view", ShaderDataType::Mat4 } };
		std::shared_ptr<UniformBuffer> cameraUBO;
		cameraUBO.reset(UniformBuffer::create(cameraLayout));
		//now attach to shaders, FCShader first then TPShader.
		cameraUBO->attachShaderBlock(FCShader, "b_camera");
		cameraUBO->attachShaderBlock(TPShader, "b_camera");
		//now send camera data to uniform buffer object.
		cameraUBO->uploadDataToBlock("u_projection", glm::value_ptr(projection));
		cameraUBO->uploadDataToBlock("u_view", glm::value_ptr(view));

		//LIGHTING UBO	
		blockNum++;											//move block number along one.
		glm::vec3 lightPosition(1.0f, 4.0f, 6.0f);			//vec3 for light position.
		glm::vec3 viewPosition(0.0f, 0.0f, 0.0f);			//vec3 for view position.
		glm::vec3 lightColour(1.0f, 1.0f, 1.0f);			//vec3 for light colour.
		uint32_t lightsUBO_ID;								//openGL ID for lights UBO.
		uint32_t lightsDataSize = sizeof(glm::vec4) * 3;	//how big this is; BUT can't use vec3! OpenGL needs either a 2N or 4N, so must use a vec4 here as a base alignment for a vec3 or WILL NOT WORK.
		
		//generate, bind and set UBO for lights.
		glGenBuffers(1, &lightsUBO_ID);														//generate Buffer for light UBO. 
		glBindBuffer(GL_UNIFORM_BUFFER, lightsUBO_ID);										//bind buffer for light UBO.
		glBufferData(GL_UNIFORM_BUFFER, lightsDataSize, nullptr, GL_DYNAMIC_DRAW);			//send data and size.
		glBindBufferRange(GL_UNIFORM_BUFFER, blockNum, lightsUBO_ID, 0, lightsDataSize);	//bind the range; to UNI_BUFFER, this block, this ubo, from 0 to data siz (ie all of it).
		
		//now attach to shaders, as lights just for the TPShader.
		uint32_t blockIndex = glGetUniformBlockIndex(TPShader->getRenderID(), "b_lights");	//first get the block number off the shader.
		glUniformBlockBinding(TPShader->getRenderID(), blockIndex, blockNum);				//link to binding point.
		
		//now send light data to uniform buffer object. These MUST be in the same order as in the shader file.
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3), glm::value_ptr(lightPosition));					//uploading light position between 0 and sizeof vec3.
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::vec3), glm::value_ptr(viewPosition));		//uploading view position starting at a vec4 (remember vec3 must have base alignment of 4N), then the size of a vec3.
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) * 2, sizeof(glm::vec3), glm::value_ptr(lightColour));	//uploading light colour, same as above viewPosition but offsect vec4 * 2, as two vec4s prior.

		//for the transofrm of the models, array as can have a pyramid and a cube.
		glm::mat4 models[3];
		models[0] = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.5f, -5.0f));
		models[1] = glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, -0.5f, -5.0f));
		models[2] = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, -0.5f, -5.0f));

		//create a float for the time step and initialise at 0.
		float timeStep = 0.0f;

		glEnable(GL_DEPTH_TEST);
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);

		TextureUnitManager textureUnitManager(32);




		while (m_running)
		{
			//update the time step with the timer function getElapsedTime()
			timeStep = m_timer->getElapsedTime();
			m_timer->reset();

			//get the model to rotate (easier to see whether it is a 3d shape)
			for (auto& model: models) model = glm::rotate(model, timeStep, glm::vec3(0.0f, 1.0f, 0.5f));

			//things to do in the frame...
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//DRAW A PYRAMID.
			//bind the shader FCproram (Flat Coloured shader) & bind the correct buffers, vertex array and index buffer.
			glUseProgram(FCShader->getRenderID());
			glBindVertexArray(pyramidVAO->getRenderID());
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pyramidIBO->getRenderID());
			//upload all relevant uniforms for projectionm, view and model, then draw the PYRAMID!
			FCShader->uploatMat4("u_model", models[0]);
			glDrawElements(GL_TRIANGLES, pyramidVAO->getDrawCount(), GL_UNSIGNED_INT, nullptr);

			//DRAW CUBE A.
			//bind the shader (textured phong shader) & bind the buffers, vertex array and index buffer.
			glUseProgram(TPShader->getRenderID()); 
			glBindVertexArray(cubeVAO->getRenderID()); 
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIBO->getRenderID()); 
			//upload all relevant info & bind texture & draw the CUBE!
			TPShader->uploatMat4("u_model", models[1]);


			glBindTexture(GL_TEXTURE_2D, letterTexture->getID());
			TPShader->uploadInt("u_texData", 0);


			glDrawElements(GL_TRIANGLES, cubeVAO->getDrawCount(), GL_UNSIGNED_INT, nullptr);

			//DRAW CUBE B!
			TPShader->uploatMat4("u_model", models[2]);


			glBindTexture(GL_TEXTURE_2D, numberTexture->getID());
			TPShader->uploadInt("u_texData", 0);


			glDrawElements(GL_TRIANGLES, cubeVAO->getDrawCount(), GL_UNSIGNED_INT, nullptr);


			m_window->onUpdate(timeStep);
		}

	}

}






/*
//need a location to upload uniform.
//GLuint uniformLocation;

//upload all relevant uniforms for projectionm, view and model.
//uniformLocation = glGetUniformLocation(FCShader->getRenderID(), "u_model");
//glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(models[0]));
//draw the PYRAMID!
//glDrawElements(GL_TRIANGLES, pyramidVAO->getDrawCount(), GL_UNSIGNED_INT, nullptr);
*/
/*
//upload all relevant uniforms for projectionm, view and model.
uniformLocation = glGetUniformLocation(TPShader->getRenderID(), "u_model");
glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(models[1]));

//bind the texture that is wanted.
glBindTexture(GL_TEXTURE_2D, letterTexture->getID());
uniformLocation = glGetUniformLocation(TPShader->getRenderID(), "u_texData");
glUniform1i(uniformLocation, 0);

//draw the CUBE!
glDrawElements(GL_TRIANGLES, cubeVAO->getDrawCount(), GL_UNSIGNED_INT, nullptr);
*/
/*
uniformLocation = glGetUniformLocation(TPShader->getRenderID(), "u_model");
glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(models[2]));
glBindTexture(GL_TEXTURE_2D, numberTexture->getID());
glDrawElements(GL_TRIANGLES, cubeVAO->getDrawCount(), GL_UNSIGNED_INT, nullptr);
*/


/*
uint32_t cameraUBO_ID;								//openGL ID for camera UBO.
UniformBufferLayout cameraLayout = {{"u_projection", ShaderDataType::Mat4 }, {"u_view", ShaderDataType::Mat4 }};

//generate, bind and set UBO for camera.
glGenBuffers(1, &cameraUBO_ID);														//generate Buffer for camera UBO.
glBindBuffer(GL_UNIFORM_BUFFER, cameraUBO_ID);										//bind buffer for camera UBO.
glBufferData(GL_UNIFORM_BUFFER, cameraLayout.getStride(), nullptr, GL_DYNAMIC_DRAW);		//send data and size.
glBindBufferRange(GL_UNIFORM_BUFFER, blockNum, cameraUBO_ID, 0, cameraLayout.getStride());	//bind the range; to UNI_BUFFER, this block, this ubo, from 0 to data siz (ie all of it).

//now attach to shaders, FCShader first then TPShader.
uint32_t blockIndex = glGetUniformBlockIndex(FCShader->getRenderID(), "b_camera");	//first get the block number off the shader.
glUniformBlockBinding(FCShader->getRenderID(), blockIndex, blockNum);				//link to binding point.
blockIndex = glGetUniformBlockIndex(TPShader->getRenderID(), "b_camera");			//same as above but for TPShader.
glUniformBlockBinding(TPShader->getRenderID(), blockIndex, blockNum);				//same as above but for TPShader.

//now send camera data to uniform buffer object.
//TO DO! Ideally a single operation for the two below; would need to be stored sequentially, far more efficient this way.
auto element = *cameraLayout.begin();
glBufferSubData(GL_UNIFORM_BUFFER, element.m_offset, element.m_size, glm::value_ptr(projection));
element = *(cameraLayout.begin() + 1);
glBufferSubData(GL_UNIFORM_BUFFER, element.m_offset, element.m_size, glm::value_ptr(view));

//glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));				//uploading projection between 0 and sizeof mat4 (64bytes).
//glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));	//uploading view but start off the sizeof mat4 and then the sizeof mat4 (64bytes).
*/

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