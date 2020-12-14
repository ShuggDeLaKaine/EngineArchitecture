/** \file application.cpp */

#include "engine_pch.h"
#include "core/application.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/headerList.h"

#ifdef  NG_PLATFORM_WINDOWS
/*  #include "platform/windows/win32System.h"
	#include "platform/windows/winTimer.h"
	#else */
	#include "platform/GLFW/GLFWSystem.h"
#endif //  NG_PLATFORM_WINDOWS


namespace Engine {
#pragma region TEMP_CLASS
//temporary class for vertex
/* when done properly need three classes for final engine;
* one for 2d rendering - 2d vertices,
* one for static 3d vertices,
* one for animated 3d vertices. */

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
	VertexBufferLayout TPVertexNormalised::s_BufferLayout = {{ShaderDataType::Float3, { ShaderDataType::Short3, true }, { ShaderDataType::Short2, true }}, 24 };

	// Set static vars
	Application* Application::s_instance = nullptr;

	Application::Application() 
	{
		if (s_instance == nullptr)
			s_instance = this;

		//start the systems.
		//start the log system.
		m_logSystem.reset(new Log);
		m_logSystem->start();

		//start the windows system.
#ifdef NG_PLATFORM_WINDOWS
		/* m_windowsSystem.reset(new Win32System);
		#else */
		m_windowsSystem.reset(new GLFWSystem);
#endif
		m_windowsSystem->start();

		//reset & start the timer.
		//if on a windows platform then make a windows timer, otherwise make a chrono timer.
		//NOTE - not a system! Don't need to stop it.
#ifdef  NG_PLATFORM_WINDOWS
		/* m_timer.reset(new WinTimer);
		#else */
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

#pragma region USER_EVENTS
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
#pragma endregion

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

#pragma region GEN_FUNCS
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

#pragma endregion

	void Application::run()
	{
#pragma region RAW_DATA

		std::vector<TPVertexNormalised> cubeVertices(24);
		//										 <------- Pos ------->             <----- normal ----->             <---- UV ---->
		cubeVertices.at(0) = TPVertexNormalised({ 0.5f,  0.5f, -0.5f }, normalise({ 0.0f,  0.0f, -1.0f }), normalise({ 0.0f,  0.0f }));
		cubeVertices.at(1) = TPVertexNormalised({ 0.5f, -0.5f, -0.5f }, normalise({ 0.0f,  0.0f, -1.0f }), normalise({ 0.0f,  0.5f }));
		cubeVertices.at(2) = TPVertexNormalised({ -0.5f, -0.5f, -0.5f }, normalise({ 0.0f,  0.0f, -1.0f }), normalise({ 0.33f, 0.5f }));
		cubeVertices.at(3) = TPVertexNormalised({ -0.5f,  0.5f, -0.5f }, normalise({ 0.0f,  0.0f, -1.0f }), normalise({ 0.33f, 0.0f }));

		cubeVertices.at(4) = TPVertexNormalised({ -0.5f, -0.5f,  0.5f }, normalise({ 0.0f,  0.0f,  1.0f }), normalise({ 0.33f, 0.5f }));
		cubeVertices.at(5) = TPVertexNormalised({ 0.5f, -0.5f,  0.5f }, normalise({ 0.0f,  0.0f,  1.0f }), normalise({ 0.66f, 0.5f }));
		cubeVertices.at(6) = TPVertexNormalised({ 0.5f,  0.5f,  0.5f }, normalise({ 0.0f,  0.0f,  1.0f }), normalise({ 0.66f, 0.0f }));
		cubeVertices.at(7) = TPVertexNormalised({ -0.5f,  0.5f,  0.5f }, normalise({ 0.0f,  0.0f,  1.0f }), normalise({ 0.33f, 0.0f }));

		cubeVertices.at(8) = TPVertexNormalised({ -0.5f, -0.5f, -0.5f }, normalise({ 0.0f, -1.0f,  0.0f }), normalise({ 1.0f,  0.0f }));
		cubeVertices.at(9) = TPVertexNormalised({ 0.5f, -0.5f, -0.5f }, normalise({ 0.0f, -1.0f,  0.0f }), normalise({ 0.66f, 0.0f }));
		cubeVertices.at(10) = TPVertexNormalised({ 0.5f, -0.5f,  0.5f }, normalise({ 0.0f, -1.0f,  0.0f }), normalise({ 0.66f, 0.5f }));
		cubeVertices.at(11) = TPVertexNormalised({ -0.5f, -0.5f,  0.5f }, normalise({ 0.0f, -1.0f,  0.0f }), normalise({ 1.0f,  0.5f }));

		cubeVertices.at(12) = TPVertexNormalised({ 0.5f,  0.5f,  0.5f }, normalise({ 0.0f,  1.0f,  0.0f }), normalise({ 0.0f,  0.5f }));
		cubeVertices.at(13) = TPVertexNormalised({ 0.5f,  0.5f, -0.5f }, normalise({ 0.0f,  1.0f,  0.0f }), normalise({ 0.0f,  1.0f }));
		cubeVertices.at(14) = TPVertexNormalised({ -0.5f,  0.5f, -0.5f }, normalise({ 0.0f,  1.0f,  0.0f }), normalise({ 0.33f, 1.0f }));
		cubeVertices.at(15) = TPVertexNormalised({ -0.5f,  0.5f,  0.5f }, normalise({ 0.0f,  1.0f,  0.0f }), normalise({ 0.3f,  0.5f }));

		cubeVertices.at(16) = TPVertexNormalised({ -0.5f,  0.5f,  0.5f }, normalise({ -1.0f,  0.0f,  0.0f }), normalise({ 0.66f, 0.5f }));
		cubeVertices.at(17) = TPVertexNormalised({ -0.5f,  0.5f, -0.5f }, normalise({ -1.0f,  0.0f,  0.0f }), normalise({ 0.33f, 0.5f }));
		cubeVertices.at(18) = TPVertexNormalised({ -0.5f, -0.5f, -0.5f }, normalise({ -1.0f,  0.0f,  0.0f }), normalise({ 0.33f, 1.0f }));
		cubeVertices.at(19) = TPVertexNormalised({ -0.5f, -0.5f,  0.5f }, normalise({ -1.0f,  0.0f,  0.0f }), normalise({ 0.66f, 1.0f }));

		cubeVertices.at(20) = TPVertexNormalised({ 0.5f, -0.5f, -0.5f }, normalise({ 1.0f,  0.0f,  0.0f }), normalise({ 1.0f,  1.0f }));
		cubeVertices.at(21) = TPVertexNormalised({ 0.5f,  0.5f, -0.5f }, normalise({ 1.0f,  0.0f,  0.0f }), normalise({ 1.0f,  0.5f }));
		cubeVertices.at(22) = TPVertexNormalised({ 0.5f,  0.5f,  0.5f }, normalise({ 1.0f,  0.0f,  0.0f }), normalise({ 0.66f, 0.5f }));
		cubeVertices.at(23) = TPVertexNormalised({ 0.5f, -0.5f,  0.5f }, normalise({ 1.0f,  0.0f,  0.0f }), normalise({ 0.66f, 1.0f }));

		std::vector<TPVertexNormalised> pyramidVertices(16);
		//										     <------- Pos ------->            <--------- normal --------->               <---- UV ---->		
		pyramidVertices.at(0) = TPVertexNormalised({ -0.5f, -0.5f, -0.5f }, normalise({ 0.0f,	-1.0f,     0.0f }), normalise({ 0.0f,  0.0f }));
		pyramidVertices.at(1) = TPVertexNormalised({ 0.5f, -0.5f, -0.5f }, normalise({ 0.0f,	-1.0f,	   0.0f }), normalise({ 0.0f,  0.0f }));
		pyramidVertices.at(2) = TPVertexNormalised({ 0.5f, -0.5f,  0.5f }, normalise({ 0.0f,	-1.0f,     0.0f }), normalise({ 0.0f,  0.0f }));
		pyramidVertices.at(3) = TPVertexNormalised({ -0.5f, -0.5f,  0.5f }, normalise({ 0.0f,	-1.0f,     0.0f }), normalise({ 0.0f,  0.0f }));

		pyramidVertices.at(4) = TPVertexNormalised({ -0.5f, -0.5f, -0.5f }, normalise({ -0.8944f, 0.4472f,  0.0f }), normalise({ 0.0f,  0.0f }));
		pyramidVertices.at(5) = TPVertexNormalised({ -0.5f, -0.5f,  0.5f }, normalise({ -0.8944f, 0.4472f,  0.0f }), normalise({ 0.0f,  0.0f }));
		pyramidVertices.at(6) = TPVertexNormalised({ 0.0f,  0.5f,  0.0f }, normalise({ -0.8944f, 0.4472f,  0.0f }), normalise({ 0.0f,  0.0f }));

		pyramidVertices.at(7) = TPVertexNormalised({ -0.5f, -0.5f,  0.5f }, normalise({ 0.0f,	 0.4472f,  0.8944f }), normalise({ 0.0f,  0.0f }));
		pyramidVertices.at(8) = TPVertexNormalised({ 0.5f, -0.5f,  0.5f }, normalise({ 0.0f,	 0.4472f,  0.8944f }), normalise({ 0.0f,  0.0f }));
		pyramidVertices.at(9) = TPVertexNormalised({ 0.0f,  0.5f,  0.0f }, normalise({ 0.0f,	 0.4472f,  0.8944f }), normalise({ 0.0f,  0.0f }));

		pyramidVertices.at(10) = TPVertexNormalised({ 0.5f, -0.5f,  0.5f }, normalise({ 0.8944f, 0.4472f,  0.0f }), normalise({ 0.0f,  0.0f }));
		pyramidVertices.at(11) = TPVertexNormalised({ 0.5f, -0.5f, -0.5f }, normalise({ 0.8944f, 0.4472f,  0.0f }), normalise({ 0.0f,  0.0f }));
		pyramidVertices.at(12) = TPVertexNormalised({ 0.0f,  0.5f,  0.0f }, normalise({ 0.8944f, 0.4472f,  0.0f }), normalise({ 0.0f,  0.0f }));

		pyramidVertices.at(13) = TPVertexNormalised({ 0.5f, -0.5f, -0.5f }, normalise({ 0.0f,	 0.4472f, -0.8944f }), normalise({ 0.0f,  0.0f }));
		pyramidVertices.at(14) = TPVertexNormalised({ -0.5f, -0.5f, -0.5f }, normalise({ 0.0f,	 0.4472f, -0.8944f }), normalise({ 0.0f,  0.0f }));
		pyramidVertices.at(15) = TPVertexNormalised({ 0.0f,  0.5f,  0.0f }, normalise({ 0.0f,	 0.4472f, -0.8944f }), normalise({ 0.0f,  0.0f }));


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

		/*
		//THIS IS UP IN THE TEMP VERTEX CLASSES
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
		pyramidVBO.reset(VertexBuffer::create(pyramidVertices.data(), sizeof(TPVertexNormalised) * pyramidVertices.size(), TPVertexNormalised::getBufferLayout()));
		pyramidIBO.reset(IndexBuffer::create(pyramidIndices, 18));

		//set the vertex and index buffers. 
		pyramidVAO->addVertexBuffer(pyramidVBO);
		pyramidVAO->setIndexBuffer(pyramidIBO);
#pragma endregion

#pragma region SHADERS
		std::shared_ptr<Shaders> TPShader;
		TPShader.reset(Shaders::create("assets/shaders/texturedPhong.glsl"));
#pragma endregion 

#pragma region TEXTURES

		std::shared_ptr<Textures> letterTexture;
		letterTexture.reset(Textures::create("assets/textures/letterCube.png"));

		std::shared_ptr<Textures> numberTexture;
		numberTexture.reset(Textures::create("assets/textures/numberCube.png"));

		unsigned char whitePixel[4] = { 255, 255, 255, 255 };		//a white pixel.
		std::shared_ptr<Textures> plainWhiteTexture;
		plainWhiteTexture.reset(Textures::create(1, 1, 4, whitePixel));


#pragma endregion

#pragma region MATERIALS
		std::shared_ptr<Material> pyramidMaterial;
		std::shared_ptr<Material> letterCubeMaterial;
		std::shared_ptr<Material> numberCubeMaterial;

		pyramidMaterial.reset(new Material(TPShader, { 0.3f, 0.9f, 0.4f, 1.0f }));
		letterCubeMaterial.reset(new Material(TPShader, letterTexture));
		numberCubeMaterial.reset(new Material(TPShader, numberTexture));

#pragma endregion
		
		//creating and setting the cameras.
		FreeOthroCamController cam2D({ 0.0f, 0.0f, 0.0f }, 0.0f, 0.0f, static_cast<float>(m_window->getWidth()), static_cast<float>(m_window->getHeight()), 0.0f);
		FreeEulerCamController cam3D({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f });

		//for the transofrm of the models, array as can have a pyramid and a cube.
		glm::mat4 models[3];
		models[0] = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.5f, -5.0f));
		models[1] = glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, -0.5f, -5.0f));
		models[2] = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, -0.5f, -5.0f));

		//create the scene wide uniforms for 2D rendering.
		SceneWideUniforms swu2D;
		swu2D["u_view"] = std::pair<ShaderDataType, void *>(ShaderDataType::Mat4, static_cast<void *>(glm::value_ptr(cam2D.m_camera.view)));
		swu2D["u_projection"] = std::pair<ShaderDataType, void *>(ShaderDataType::Mat4, static_cast<void *>(glm::value_ptr(cam2D.m_camera.projection)));

		//create the scene wide uniforms for 3D rendering.
		SceneWideUniforms swu3D;
		//what the scene wide uniforms, what is consistant across the scene.
		swu3D["u_view"] = std::pair<ShaderDataType, void *>(ShaderDataType::Mat4, static_cast<void *>(glm::value_ptr(cam3D.m_camera.view)));
		swu3D["u_projection"] = std::pair<ShaderDataType, void *>(ShaderDataType::Mat4, static_cast<void *>(glm::value_ptr(cam3D.m_camera.projection)));

		//vec3 to initialise the light data.
		glm::vec3 lightData[3] = { { 1.0f, 1.0f, 1.0f }, { 1.0f, 4.0f, 6.0f }, { 0.0f, 0.0f, 0.0f } };
		swu3D["u_lightColour"] = std::pair<ShaderDataType, void *>(ShaderDataType::Float3, static_cast<void *>(glm::value_ptr(lightData[0])));
		swu3D["u_lightPos"] = std::pair<ShaderDataType, void *>(ShaderDataType::Float3, static_cast<void *>(glm::value_ptr(lightData[1])));
		swu3D["u_viewPos"] = std::pair<ShaderDataType, void *>(ShaderDataType::Float3, static_cast<void *>(glm::value_ptr(lightData[2])));

		//create a float for the time step and initialise at 0.
		float timeStep = 0.0f;

		Quad quads[7] =
		{
			Quad::createCentreHalfExtents({ 400.0f, 200.0f }, { 100.0f, 50.0f }),
			Quad::createCentreHalfExtents({ 200.0f, 300.0f }, { 50.0f, 100.0f }),
			Quad::createCentreHalfExtents({ 400.0f, 500.0f }, { 100.0f, 75.0f }),
			Quad::createCentreHalfExtents({ 100.0f, 200.0f }, { 75.0f, 50.0f }),
			Quad::createCentreHalfExtents({ 500.0f, 100.0f }, { 50.0f, 25.0f }),
			Quad::createCentreHalfExtents({ 300.0f, 350.0f }, { 175.0f, 115.0f }),
		};			//TO DO - some testing on the other create quads, using Simons example using paint to check they're working as expected.

		TextureUnitManager textureUnitManager(32);
		uint32_t unit;

		Renderer3D::init();
		Renderer3D::attachShader(TPShader);

		Renderer2D::init();

		std::shared_ptr<RenderCommands> clearCommand;
		clearCommand.reset(RenderCommandsFactory::createCommand(RenderCommands::Commands::clearColourAndDepthBuffer));
		{
			std::shared_ptr<RenderCommands> setClearCommand;
			setClearCommand.reset(RenderCommandsFactory::createCommand(RenderCommands::Commands::setClearColour, 1.0f, 0.0f, 1.0f, 1.0f));
			RendererCommons::actionCommand(setClearCommand);
		}		//scope around as the memory used to create the the .reset() part will go out of scope. 


		while (m_running)
		{
			//update the time step with the timer function getElapsedTime()
			timeStep = m_timer->getElapsedTime();
			m_timer->reset();

			//things to do in the frame...
			RendererCommons::actionCommand(clearCommand);

			//get the model to rotate (easier to see whether it is a 3d shape)
			for (auto& model : models) model = glm::rotate(model, timeStep, glm::vec3(0.0f, 1.0f, 0.5f));

			glEnable(GL_DEPTH_TEST);

			//begin rendering with the scene wide uniforms. 
			Renderer3D::begin(swu3D);
			
			//submit renderer info with vertex array, material and mat4 model of object that needs to be drawn.
			Renderer3D::submit(pyramidVAO, pyramidMaterial, models[0]);		
			Renderer3D::submit(cubeVAO, numberCubeMaterial, models[1]);
			Renderer3D::submit(cubeVAO, letterCubeMaterial, models[2]);

			//end the rendering.
			Renderer3D::end();
			
			glDisable(GL_DEPTH_TEST);

			//enable blending.
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			
			//begin rendering with the scene wide uniforms.		
			Renderer2D::begin(swu2D);
			/*
			//submit renderer info.
			Renderer2D::submit(quads[0], { 0.0f, 0.0f, 1.0f, 1.0f });
			Renderer2D::submit(quads[1], letterTexture);
			Renderer2D::submit(quads[2], { 0.0f, 1.0f, 0.0f, 1.0f }, numberTexture);
			Renderer2D::submit(quads[3], { 0.0f, 1.0f, 1.0f, 1.0f }, numberTexture, 45.0f, true);
			Renderer2D::submit(quads[4], letterTexture, -20.0f, true);
			Renderer2D::submit(quads[5], { 0.0f, 1.0f, 0.0f, 0.5f }, letterTexture, 90.0f, true);
			Renderer2D::submit(quads[5], { 0.0f, 0.0f, 1.0f, 0.5f }, letterTexture, glm::radians(-60.0f));
			*/
			Renderer2D::submit("Welcome", { 230.0f, 75.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
			Renderer2D::submit("To my demo", { 160.0f, 580.0f }, { 0.0f, 1.0f, 0.0f, 1.0f });
			
			//end the rendering.
			Renderer2D::end();
			
			glDisable(GL_BLEND);
			
			//updates on cameras.
			cam2D.onUpdate(timeStep);
			cam3D.onUpdate(timeStep);

			m_window->onUpdate(timeStep);
		}
	}
}