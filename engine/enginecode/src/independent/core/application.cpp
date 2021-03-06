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
	// setting the static vars
	VertexBufferLayout TPVertexNormalised::s_BufferLayout = {{ShaderDataType::Float3, { ShaderDataType::Short3, true }, { ShaderDataType::Short2, true }}, 24 };
	Application* Application::s_instance = nullptr;

	Application::Application() 
	{
		if (s_instance == nullptr)
			s_instance = this;

		//start the systems.
		//starting with the log system.
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
		WindowProperties properties("IT'S AN ENGINE!", 800, 600, false);
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
		m_ranNumSytem->stop();
		m_windowsSystem->stop();
		m_logSystem->stop();
	}
	
	void Application::run()
	{
#pragma region RAW_DATA

		std::vector<TPVertexNormalised> cubeVertices(24);
		//										 <------- Pos ------->             <----- normal ----->             <---- UV ---->
		cubeVertices.at(0) = TPVertexNormalised({ 0.5f,  0.5f, -0.5f },  GenFuncs::normalise({ 0.0f,  0.0f, -1.0f }), GenFuncs::normalise({ 0.0f,  0.0f }));
		cubeVertices.at(1) = TPVertexNormalised({ 0.5f, -0.5f, -0.5f },  GenFuncs::normalise({ 0.0f,  0.0f, -1.0f }), GenFuncs::normalise({ 0.0f,  0.5f }));
		cubeVertices.at(2) = TPVertexNormalised({ -0.5f, -0.5f, -0.5f }, GenFuncs::normalise({ 0.0f,  0.0f, -1.0f }), GenFuncs::normalise({ 0.33f, 0.5f }));
		cubeVertices.at(3) = TPVertexNormalised({ -0.5f,  0.5f, -0.5f }, GenFuncs::normalise({ 0.0f,  0.0f, -1.0f }), GenFuncs::normalise({ 0.33f, 0.0f }));

		cubeVertices.at(4) = TPVertexNormalised({ -0.5f, -0.5f,  0.5f }, GenFuncs::normalise({ 0.0f,  0.0f,  1.0f }), GenFuncs::normalise({ 0.33f, 0.5f }));
		cubeVertices.at(5) = TPVertexNormalised({ 0.5f, -0.5f,  0.5f },  GenFuncs::normalise({ 0.0f,  0.0f,  1.0f }), GenFuncs::normalise({ 0.66f, 0.5f }));
		cubeVertices.at(6) = TPVertexNormalised({ 0.5f,  0.5f,  0.5f },  GenFuncs::normalise({ 0.0f,  0.0f,  1.0f }), GenFuncs::normalise({ 0.66f, 0.0f }));
		cubeVertices.at(7) = TPVertexNormalised({ -0.5f,  0.5f,  0.5f }, GenFuncs::normalise({ 0.0f,  0.0f,  1.0f }), GenFuncs::normalise({ 0.33f, 0.0f }));

		cubeVertices.at(8) = TPVertexNormalised({ -0.5f, -0.5f, -0.5f }, GenFuncs::normalise({ 0.0f, -1.0f,  0.0f }), GenFuncs::normalise({ 1.0f,  0.0f }));
		cubeVertices.at(9) = TPVertexNormalised({ 0.5f, -0.5f, -0.5f },  GenFuncs::normalise({ 0.0f, -1.0f,  0.0f }), GenFuncs::normalise({ 0.66f, 0.0f }));
		cubeVertices.at(10) = TPVertexNormalised({ 0.5f, -0.5f,  0.5f }, GenFuncs::normalise({ 0.0f, -1.0f,  0.0f }), GenFuncs::normalise({ 0.66f, 0.5f }));
		cubeVertices.at(11) = TPVertexNormalised({ -0.5f, -0.5f,  0.5f },GenFuncs::normalise({ 0.0f, -1.0f,  0.0f }), GenFuncs::normalise({ 1.0f,  0.5f }));

		cubeVertices.at(12) = TPVertexNormalised({ 0.5f,  0.5f,  0.5f },  GenFuncs::normalise({ 0.0f,  1.0f,  0.0f }), GenFuncs::normalise({ 0.0f,  0.5f }));
		cubeVertices.at(13) = TPVertexNormalised({ 0.5f,  0.5f, -0.5f },  GenFuncs::normalise({ 0.0f,  1.0f,  0.0f }), GenFuncs::normalise({ 0.0f,  1.0f }));
		cubeVertices.at(14) = TPVertexNormalised({ -0.5f,  0.5f, -0.5f }, GenFuncs::normalise({ 0.0f,  1.0f,  0.0f }), GenFuncs::normalise({ 0.33f, 1.0f }));
		cubeVertices.at(15) = TPVertexNormalised({ -0.5f,  0.5f,  0.5f }, GenFuncs::normalise({ 0.0f,  1.0f,  0.0f }), GenFuncs::normalise({ 0.3f,  0.5f }));

		cubeVertices.at(16) = TPVertexNormalised({ -0.5f,  0.5f,  0.5f }, GenFuncs::normalise({ -1.0f,  0.0f,  0.0f }), GenFuncs::normalise({ 0.66f, 0.5f }));
		cubeVertices.at(17) = TPVertexNormalised({ -0.5f,  0.5f, -0.5f }, GenFuncs::normalise({ -1.0f,  0.0f,  0.0f }), GenFuncs::normalise({ 0.33f, 0.5f }));
		cubeVertices.at(18) = TPVertexNormalised({ -0.5f, -0.5f, -0.5f }, GenFuncs::normalise({ -1.0f,  0.0f,  0.0f }), GenFuncs::normalise({ 0.33f, 1.0f }));
		cubeVertices.at(19) = TPVertexNormalised({ -0.5f, -0.5f,  0.5f }, GenFuncs::normalise({ -1.0f,  0.0f,  0.0f }), GenFuncs::normalise({ 0.66f, 1.0f }));

		cubeVertices.at(20) = TPVertexNormalised({ 0.5f, -0.5f, -0.5f }, GenFuncs::normalise({ 1.0f,  0.0f,  0.0f }), GenFuncs::normalise({ 1.0f,  1.0f }));
		cubeVertices.at(21) = TPVertexNormalised({ 0.5f,  0.5f, -0.5f }, GenFuncs::normalise({ 1.0f,  0.0f,  0.0f }), GenFuncs::normalise({ 1.0f,  0.5f }));
		cubeVertices.at(22) = TPVertexNormalised({ 0.5f,  0.5f,  0.5f }, GenFuncs::normalise({ 1.0f,  0.0f,  0.0f }), GenFuncs::normalise({ 0.66f, 0.5f }));
		cubeVertices.at(23) = TPVertexNormalised({ 0.5f, -0.5f,  0.5f }, GenFuncs::normalise({ 1.0f,  0.0f,  0.0f }), GenFuncs::normalise({ 0.66f, 1.0f }));

		std::vector<TPVertexNormalised> pyramidVertices(16);
		//										     <------- Pos ------->            <--------- normal --------->               <---- UV ---->		
		pyramidVertices.at(0) = TPVertexNormalised({ -0.5f, -0.5f, -0.5f }, GenFuncs::normalise({ 0.0f,	-1.0f,     0.0f }), GenFuncs::normalise({ 0.0f,  0.0f }));
		pyramidVertices.at(1) = TPVertexNormalised({ 0.5f, -0.5f, -0.5f },  GenFuncs::normalise({ 0.0f,	-1.0f,	   0.0f }), GenFuncs::normalise({ 0.0f,  0.0f }));
		pyramidVertices.at(2) = TPVertexNormalised({ 0.5f, -0.5f,  0.5f },  GenFuncs::normalise({ 0.0f,	-1.0f,     0.0f }), GenFuncs::normalise({ 0.0f,  0.0f }));
		pyramidVertices.at(3) = TPVertexNormalised({ -0.5f, -0.5f,  0.5f }, GenFuncs::normalise({ 0.0f,	-1.0f,     0.0f }), GenFuncs::normalise({ 0.0f,  0.0f }));
																			
		pyramidVertices.at(4) = TPVertexNormalised({ -0.5f, -0.5f, -0.5f }, GenFuncs::normalise({ -0.8944f, 0.4472f,  0.0f }), GenFuncs::normalise({ 0.0f,  0.0f }));
		pyramidVertices.at(5) = TPVertexNormalised({ -0.5f, -0.5f,  0.5f }, GenFuncs::normalise({ -0.8944f, 0.4472f,  0.0f }), GenFuncs::normalise({ 0.0f,  0.0f }));
		pyramidVertices.at(6) = TPVertexNormalised({ 0.0f,  0.5f,  0.0f },  GenFuncs::normalise({ -0.8944f, 0.4472f,  0.0f }), GenFuncs::normalise({ 0.0f,  0.0f }));
																			
		pyramidVertices.at(7) = TPVertexNormalised({ -0.5f, -0.5f,  0.5f }, GenFuncs::normalise({ 0.0f,	 0.4472f,  0.8944f }), GenFuncs::normalise({ 0.0f,  0.0f }));
		pyramidVertices.at(8) = TPVertexNormalised({ 0.5f, -0.5f,  0.5f },  GenFuncs::normalise({ 0.0f,	 0.4472f,  0.8944f }), GenFuncs::normalise({ 0.0f,  0.0f }));
		pyramidVertices.at(9) = TPVertexNormalised({ 0.0f,  0.5f,  0.0f },  GenFuncs::normalise({ 0.0f,	 0.4472f,  0.8944f }), GenFuncs::normalise({ 0.0f,  0.0f }));
																			
		pyramidVertices.at(10) = TPVertexNormalised({ 0.5f, -0.5f,  0.5f }, GenFuncs::normalise({ 0.8944f, 0.4472f,  0.0f }), GenFuncs::normalise({ 0.0f,  0.0f }));
		pyramidVertices.at(11) = TPVertexNormalised({ 0.5f, -0.5f, -0.5f }, GenFuncs::normalise({ 0.8944f, 0.4472f,  0.0f }), GenFuncs::normalise({ 0.0f,  0.0f }));
		pyramidVertices.at(12) = TPVertexNormalised({ 0.0f,  0.5f,  0.0f }, GenFuncs::normalise({ 0.8944f, 0.4472f,  0.0f }), GenFuncs::normalise({ 0.0f,  0.0f }));

		pyramidVertices.at(13) = TPVertexNormalised({ 0.5f, -0.5f, -0.5f },  GenFuncs::normalise({ 0.0f,	 0.4472f, -0.8944f }), GenFuncs::normalise({ 0.0f,  0.0f }));
		pyramidVertices.at(14) = TPVertexNormalised({ -0.5f, -0.5f, -0.5f }, GenFuncs::normalise({ 0.0f,	 0.4472f, -0.8944f }), GenFuncs::normalise({ 0.0f,  0.0f }));
		pyramidVertices.at(15) = TPVertexNormalised({ 0.0f,  0.5f,  0.0f },  GenFuncs::normalise({ 0.0f,	 0.4472f, -0.8944f }), GenFuncs::normalise({ 0.0f,  0.0f }));


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

		TextureUnitManager textUnitMan(32);

		std::shared_ptr<Textures> letterTexture;
		letterTexture.reset(Textures::create("assets/textures/letterCube.png"));

		std::shared_ptr<Textures> numberTexture;
		numberTexture.reset(Textures::create("assets/textures/numberCube.png"));

		unsigned char whitePixel[4] = { 255, 255, 255, 255 };		//a white pixel.
		std::shared_ptr<Textures> plainWhiteTexture;
		plainWhiteTexture.reset(Textures::create(1, 1, 4, whitePixel));

#pragma endregion

#pragma region MATERIALS
		//initialise materials for 3D objects, uses Material Class; as well as the Texture Type Struct that hold the various types of textures.
		std::shared_ptr<Material> pyramidMaterial;
		TextureTypeStruct textureStructPyramid;

		std::shared_ptr<Material> letterCubeMaterial;
		TextureTypeStruct textureStructLetter;
		textureStructLetter.diffuseTexture = letterTexture;

		std::shared_ptr<Material> numberCubeMaterial;
		TextureTypeStruct textureStructNumber;
		textureStructNumber.diffuseTexture = numberTexture;

		pyramidMaterial.reset(new Material(TPShader, textureStructPyramid, { 0.3f, 0.9f, 0.4f, 1.0f }));
		letterCubeMaterial.reset(new Material(TPShader, textureStructLetter));
		numberCubeMaterial.reset(new Material(TPShader, textureStructNumber));
#pragma endregion
		
#pragma region CAMERAS_LIGHTS_ACTION!
		//creating and setting the cameras.
		FreeOthroCamController cam2D({ 0.0f, 0.0f, 0.0f }, 0.0f, 0.0f, static_cast<float>(m_window->getWidth()), static_cast<float>(m_window->getHeight()), 0.0f);
		FreeEulerCamController cam3D({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f });

		//for the transofrm of the models, array as can have a pyramid and a cube.
		glm::mat4 models[3];
		models[0] = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.5f, -5.0f));
		models[1] = glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, -0.5f, -5.0f));
		models[2] = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, -0.5f, -5.0f));

		//quads created for testing 2d renderer; textures to be place over this quads and then placed in scene using 2d renderer.
		Quad quads[7] =
		{
			Quad::createCentreHalfExtents({ 400.0f, 200.0f }, { 100.0f, 50.0f }),
			Quad::createCentreHalfExtents({ 200.0f, 300.0f }, { 50.0f, 100.0f }),
			Quad::createCentreHalfExtents({ 400.0f, 500.0f }, { 100.0f, 75.0f }),
			Quad::createCentreHalfExtents({ 100.0f, 200.0f }, { 75.0f, 50.0f }),
			Quad::createCentreHalfExtents({ 500.0f, 100.0f }, { 50.0f, 25.0f }),
			Quad::createCentreHalfExtents({ 300.0f, 350.0f }, { 175.0f, 115.0f }),
		};

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
#pragma endregion

#pragma region OPEN_GL_COMMANDS

		std::shared_ptr<RenderCommands> clearCommand;
		clearCommand.reset(RenderCommandsFactory::createCommand(RenderCommands::Commands::clearColourAndDepthBuffer));

		std::shared_ptr<RenderCommands> enableDepthTestCommand;
		enableDepthTestCommand.reset(RenderCommandsFactory::createCommand(RenderCommands::Commands::setglEnableDepthTest));

		std::shared_ptr<RenderCommands> disableDepthTestCommand;
		disableDepthTestCommand.reset(RenderCommandsFactory::createCommand(RenderCommands::Commands::setglDisableDepthTest));

		std::shared_ptr<RenderCommands> enableBlendCommand;
		enableBlendCommand.reset(RenderCommandsFactory::createCommand(RenderCommands::Commands::setglEnableBlend));

		std::shared_ptr<RenderCommands> disableBlendCommand;
		disableBlendCommand.reset(RenderCommandsFactory::createCommand(RenderCommands::Commands::setglDisableBlend));

		std::shared_ptr<RenderCommands> blendFuncCommand;
		blendFuncCommand.reset(RenderCommandsFactory::createCommand(RenderCommands::Commands::setglBlendFunc));

		{
			std::shared_ptr<RenderCommands> setClearCommand;
			setClearCommand.reset(RenderCommandsFactory::createCommand(RenderCommands::Commands::setClearColour, 1.0f, 0.0f, 1.0f, 1.0f));
			RendererCommons::actionCommand(setClearCommand);
		}		//scope around as the memory used to create the the .reset() part will go out of scope and gets cleared before the program is in the running loop.
#pragma endregion

		//create a float for the time step and initialise at 0.
		float timeStep = 0.0f;

		//initiate 2D renderer.
		Renderer2D::init();

		//initiate 3D renderer & attach shaders.
		Renderer3D::init();
		Renderer3D::attachShader(TPShader);

		while (m_running)
		{
			//update the time step with the timer function getElapsedTime()
			timeStep = m_timer->getElapsedTime();
			m_timer->reset();

			/*** DO STUFF IN THE FRAME... ***/

			//clear the commands.
			RendererCommons::actionCommand(clearCommand);
			//enable depth testing.
			RendererCommons::actionCommand(enableDepthTestCommand);

			//get the model to rotate (easier to see whether it is a 3d shape)
			for (auto& model : models) model = glm::rotate(model, timeStep, glm::vec3(0.0f, 1.0f, 0.5f));

			//begin rendering with the scene wide uniforms. 
			Renderer3D::begin(swu3D);
			//submit renderer info with vertex array, material and mat4 model of object that needs to be drawn.
			Renderer3D::submit(pyramidVAO, pyramidMaterial, models[0]);		
			Renderer3D::submit(cubeVAO, numberCubeMaterial, models[1]);
			Renderer3D::submit(cubeVAO, letterCubeMaterial, models[2]);
			//end the rendering.
			Renderer3D::end();

			//disable the depth test.
			RendererCommons::actionCommand(disableDepthTestCommand);
			//enable blending.
			RendererCommons::actionCommand(enableBlendCommand);
			RendererCommons::actionCommand(blendFuncCommand);

			//begin rendering with the scene wide uniforms.		
			Renderer2D::begin(swu2D);
			//submit required 2d rendering information.
			Renderer2D::submit("Welcome", { 230.0f, 75.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
			Renderer2D::submit("To this demo", { 160.0f, 580.0f }, { 0.0f, 1.0f, 0.0f, 1.0f });
			//end the rendering.
			Renderer2D::end();
			
			//disable blending.
			RendererCommons::actionCommand(disableBlendCommand);
			
			//updates on cameras.
			cam2D.onUpdate(timeStep);
			cam3D.onUpdate(timeStep);

			m_window->onUpdate(timeStep);
		}
	}
}