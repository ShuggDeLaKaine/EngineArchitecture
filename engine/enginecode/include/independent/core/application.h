/** \file application.h
*/
#pragma once

#include "systems/log.h"
#include "systems/timer.h"
#include "systems/windows.h"
#include "systems/layerStack.h"
#include "systems/resourceManager.h"
#include "systems/IMGuiSystem.h"
#include "systems/physics.h"

#include "windows/window.h"

#include "events/event.h"
#include "events/windowsEvents.h"
#include "events/keyEvents.h"
#include "events/codes.h"
#include "events/inputPolling.h"

#include "rendering/cameras/FPSCameraControllerEuler.h"



namespace Engine {

	/**
	\class Application
	Fundemental class of the engine. A singleton which runs the game loop infinitely.
	Provides ...
	*/

	class Application
	{
	protected:
		Application(); //!< Constructor
		// Systems
		std::shared_ptr<Log> m_log; //!< Logging system
		std::shared_ptr<Timer> m_timer; //!< Timing system
		std::shared_ptr<WindowsSystem> m_windows; //!< Windows system
		static std::shared_ptr<ResourceManager> m_resources; //!< Resource management system
		std::shared_ptr<LayerStack> m_layerStack;
		std::shared_ptr<IMGUISystem> m_IMGuiSystem;
		std::shared_ptr<Physics> m_physics;
		// shared data
		static glm::ivec2 s_screenResolution; //!< Screen res
		std::shared_ptr<Window> m_window; //!< Window (abstract)
		static float s_timestep; //!< last frame timestep

#pragma region TempVars
		/*
		std::shared_ptr<rp3d::DynamicsWorld> m_world;

		std::vector<rp3d::RigidBody*> m_bodies;
		std::vector<glm::mat4> m_models;
		std::vector<glm::mat4> m_scales;
		rp3d::RigidBody* body;
		glm::vec3 m_direction;

		std::shared_ptr<FPSCameraControllerEuler> m_camera3D;*/

	private:
		static Application* s_instance; //!< Singleton instance of the application
		bool m_running = true; //!< Is the application running?
	public:
		virtual ~Application(); //!< Deconstructor
		inline static Application& getInstance() { return *s_instance; } //!< Instance getter from singleton pattern
		inline static float getTimestep() { return s_timestep; }
		inline static glm::vec2 getScreenRes() { return s_screenResolution;  }
		inline static std::shared_ptr<ResourceManager> getResources() { return m_resources; }
		inline std::shared_ptr<Window> getWindow() { return m_window; }
		void onEvent(Event& e); //!< Called when an event happens
		bool onClose(WindowCloseEvent& e); //!< On close event
		bool onResize(WindowResizeEvent& e); //!< On resize event
		bool onKeyPressed(KeyPressedEvent& e); //!< On keypress event
		void run(); //!< Main loop
	};

	// To be defined in users code
	Application* startApplication(); //!< Function definition which provides an entry hook

}