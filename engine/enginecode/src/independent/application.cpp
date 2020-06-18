/** \file application.cpp
*/

#include "engine_pch.h"

#pragma region TempIncludes
// temp includes
#include <glad/glad.h>
#include <gl/GL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb_image.h>

#include <limits>
#pragma endregion TempIncludes

#include "core/application.h"
#include "events/windowsEvents.h"
#ifdef NG_PLATFORM_WINDOWS
#include "../platform/GLFW/GLFWWindowsSystem.h"
#endif // NG_PLATFORM_WINDOWS

#include <glm/gtx/matrix_decompose.hpp> 
#include <glm/gtc/quaternion.hpp>
namespace Engine {
	// Set static vars
	Application* Application::s_instance = nullptr;
	float Application::s_timestep = 0.f;
	glm::ivec2 Application::s_screenResolution = glm::ivec2(0, 0);
	std::shared_ptr<ResourceManager> Application::m_resources = nullptr;

#pragma region TempGlobalVars
	
#pragma endregion TempGlobalVars



	Application::Application()
	{
		if (s_instance == nullptr)
		{
			s_instance = this;
		}

		// Start systems 
		m_log.reset(new Log());
		m_log->start();
		ENGINE_INFO("Logging system initialised");
		m_resources.reset(new ResourceManager());
		m_resources->start();
		ENGINE_INFO("Resource management system initialised");
		m_timer.reset(new Timer());
		m_timer->start();
		ENGINE_INFO("Timing system initialised");
		m_IMGuiSystem.reset(new IMGUISystem());
		m_IMGuiSystem->start();
		ENGINE_INFO("IMGui system started");
#ifdef NG_PLATFORM_WINDOWS
		m_windows = std::shared_ptr<WindowsSystem>(new GLFWWindowsSystem());
#endif // NG_PLATFORM_WINDOWS
		m_windows->start();
		ENGINE_INFO("Windows system initialised");
		m_physics.reset(new Physics());
		m_physics->start();
		ENGINE_INFO("Physics engine started");
		m_layerStack.reset(new LayerStack());
		m_layerStack->start();
		ENGINE_INFO("Layerstack initialised");

		// Create window
		m_window = std::shared_ptr<Window>(Window::create());
		m_window->setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1));
		// Set screen res
		Application::s_screenResolution = glm::ivec2(m_window->getWidth(), m_window->getHeight());

		// Reset timer
		m_timer->getFrameTimeSeconds();
	}

	Application::~Application()
	{
		// Stop systems
		m_layerStack->stop();
		ENGINE_INFO("Layer stack stopped");
		m_physics->stop();
		ENGINE_INFO("Physics engine stopped");
		m_windows->stop();
		ENGINE_INFO("Windows system stopped");
		m_IMGuiSystem->stop();
		ENGINE_INFO("IMGui system stopped");
		m_timer->stop();
		ENGINE_INFO("Timer system stopped");
		m_resources->stop();
		ENGINE_INFO("Resource management system stopped");
		ENGINE_INFO("About to stop logging system");
		m_log->stop();
	}

	void Application::onEvent(Event & e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(std::bind(&Application::onClose, this, std::placeholders::_1));
		dispatcher.dispatch<WindowResizeEvent>(std::bind(&Application::onResize, this, std::placeholders::_1));
		dispatcher.dispatch<KeyPressedEvent>(std::bind(&Application::onKeyPressed, this, std::placeholders::_1));

		for (auto it = m_layerStack->end(); it != m_layerStack->begin(); )
		{
			(*--it)->onEvent(e);
			if (e.handled()) break;
		}
	}

	bool Application::onClose(WindowCloseEvent & e)
	{
		m_running = false;
		return true;
	}

	bool Application::onResize(WindowResizeEvent & e)
	{
		m_window->onResize(e.getWidth(), e.getHeight());
		s_screenResolution.x = e.getWidth();
		s_screenResolution.y = e.getHeight();
		return true;
	}

	bool Application::onKeyPressed(KeyPressedEvent & e)
	{
		if (e.GetKeyCode() == NG_KEY_ESCAPE) //Escape
		{
			m_running = false;
			m_window->close();
			return true;
		}
		/*
		if (e.GetKeyCode() == NG_KEY_SPACE) // Space
		{
			float desiredVel = 2.0;
			rp3d::Vector3 vel = m_bodies[0]->getLinearVelocity();
			float velChange = desiredVel - vel.length();
			// f = m v/t
			float forceScalar = m_bodies[0]->getMass() * velChange / (1.0 / 60.f);
			rp3d::Vector3 force(m_direction.x * forceScalar, m_direction.y * forceScalar, m_direction.z * forceScalar);

			// Apply the force to the body 
			m_bodies[0]->applyForceToCenterOfMass(force);
		}
		if (e.GetKeyCode() == NG_KEY_LEFT) // Left
		{
			float desiredVel = 2.5f; // Radians per second
			rp3d::Vector3 vel = m_bodies[0]->getAngularVelocity();
			float velChange = desiredVel - vel.length();
			// T = I v/t
			float I = m_bodies[0]->getMass() / 6.0f;
			float forceScalar = I * velChange / (1.0 / 60.f);
			rp3d::Vector3 force(0.0, forceScalar, 0.0);
			// Apply the force to the body 
			m_bodies[0]->applyTorque(force);
		}
		if (e.GetKeyCode() == NG_KEY_RIGHT) // Right
		{
			float desiredVel = 2.5f; // Radians per second
			rp3d::Vector3 vel = m_bodies[0]->getAngularVelocity();
			float velChange = desiredVel - vel.length();
			// T = I v/t
			float I = m_bodies[0]->getMass() / 6.0f;
			float forceScalar = I * velChange / (1.0 / 60.f);
			rp3d::Vector3 force(0.0, -forceScalar, 0.0);
			// Apply the force to the body 
			m_bodies[0]->applyTorque(force);
		}*/
		return false;
	}

	void Application::run()
	{
		



		/*float positions[5][3] = { {-2.0, 4.0, 16.0}, {0.0, 2.0, 16.0}, {2.0, 4.0, 16.5}, {0.0, -2.0, 16.0}, {3.0, 0.0, 15.0} };
		rp3d::Quaternion q = rp3d::Quaternion::identity();
		rp3d::Quaternion orientations[5] = { q,q,q,q,q };
		float scales[5][3] = { { 1.0, 1.0, 1.0 }, { 6.0, 0.5, 6.0 }, { 1.0, 1.0, 1.0 }, { 20.0, 0.5, 20.0 }, { 1.0, 1.0, 1.0 } };
		
 
		int bodyCount = 4;
		
		for (int bodyIdx = 0; bodyIdx < bodyCount; bodyIdx++)
		{

			rp3d::Transform transform(rp3d::Vector3(positions[bodyIdx][0], positions[bodyIdx][1], positions[bodyIdx][2]), orientations[bodyIdx]);
			glm::mat4 model;
			transform.getOpenGLMatrix(&model[0][0]);
			m_models.push_back(model);

			m_bodies.push_back(nullptr);
			m_bodies.at(bodyIdx) = m_world->createRigidBody(transform);
			m_bodies.at(bodyIdx)->enableGravity(true);
			//m_bodies.at(bodyIdx)->setType(rp3d::BodyType::DYNAMIC);
			if(bodyIdx % 2) m_bodies.at(bodyIdx)->setType(rp3d::BodyType::STATIC);
			else m_bodies.at(bodyIdx)->setType(rp3d::BodyType::DYNAMIC);
			m_bodies.at(bodyIdx)->setLinearDamping(rp3d::decimal(0.2));
			m_bodies.at(bodyIdx)->setAngularDamping(rp3d::decimal(0.2));
			m_bodies.at(bodyIdx)->setUserData((void*) bodyIdx);

			rp3d::Material& material = m_bodies.at(bodyIdx)->getMaterial();
			material.setBounciness(rp3d::decimal(0.7));
			material.setFrictionCoefficient(rp3d::decimal(0.3));
		
			// Add collision shapes
			float size[3];
			size[0] = scales[bodyIdx][0] * 0.5f;
			size[1] = scales[bodyIdx][1] * 0.5f;
			size[2] = scales[bodyIdx][2] * 0.5f;
			rp3d::Vector3 halfExtents(size[0], size[1], size[2]);
			rp3d::CollisionShape* boxShape = new rp3d::BoxShape(halfExtents);

			m_scales.push_back(glm::scale(glm::mat4(1.0f), glm::vec3(scales[bodyIdx][0], scales[bodyIdx][1], scales[bodyIdx][2])));
			m_models[bodyIdx] = m_models[bodyIdx] * m_scales[bodyIdx];

			float density = 1.0f;
			rp3d::decimal mass = rp3d::decimal(size[0] * size[1] * size[2]); // Should think about density here
			auto proxy = m_bodies[bodyIdx]->addCollisionShape(boxShape, rp3d::Transform::identity(), mass);
		}

		auto transform = m_bodies[0]->getTransform();
		q = transform.getOrientation();

		m_direction.x = -2.f * (q.x * q.z - q.w * q.y);
		m_direction.y = 2.f * (q.y * q.z + q.w * q.x);
		m_direction.z = 1.f - 2.f * (q.x * q.x + q.y * q.y);
		*/
		glm::mat4 projection;
		glm::mat4 view;

		m_window->setVSync(true);
		while (m_running)
		{
			s_timestep = m_timer->getFrameTimeSeconds();

			for (auto it = m_layerStack->begin(); it != m_layerStack->end(); it++)
				(*it)->onUpdate(s_timestep);

			m_window->onUpdate(s_timestep);
		}
	}

}
