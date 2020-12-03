/** \file freeEulerCamController.cpp */
#include "engine_pch.h"
#include "camera/freeEulerCamController.h"


namespace Engine
{
	void FreeEulerCamController::onUpdate(float time)
	{
		glm::vec3 forward = { -m_transform[2][0], -m_transform[2][1], -m_transform[2][2] };
		glm::vec3 right = { m_transform[0][0], m_transform[0][1], m_transform[0][2] };

		if (GLFWInputPoller::isKeyPressed(NG_KEY_W))	//W - forwards.
			m_position += (forward * m_CamMovementSpeed * time);
		if (GLFWInputPoller::isKeyPressed(NG_KEY_S))	//S - backwards.
			m_position -= (forward * m_CamMovementSpeed * time);
		if (GLFWInputPoller::isKeyPressed(NG_KEY_A))	//A - left.
			m_position -= (right * m_CamMovementSpeed * time);
		if (GLFWInputPoller::isKeyPressed(NG_KEY_D))	//D - right.
			m_position += (right * m_CamMovementSpeed * time);
		if (GLFWInputPoller::isKeyPressed(NG_KEY_Q))	//Q - rotate anti-clockwise.
			m_rotation.z -= m_CamRotationSpeed * time;
		if (GLFWInputPoller::isKeyPressed(NG_KEY_E))	//E - rotate clockwise.
			m_rotation.z += m_CamRotationSpeed * time;
		
		//for mouse camera movement.
		glm::vec2 currentMousePosition = InputPoller::getMousePosition();
		glm::vec2 deltaMousePosition = currentMousePosition - m_lastMousePosition;
		deltaMousePosition /= glm::vec2(800.0f, 600.0f);
		m_rotation.y -= m_sensitivity * deltaMousePosition.x * time;
		m_rotation.x -= m_sensitivity * deltaMousePosition.y * time;
		m_lastMousePosition = currentMousePosition;

		//getting and setting the rotation of the camera.
		glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), m_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), m_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f), m_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		m_orientation = rotX * rotY * rotZ;

		//set the transform and set to the camera view.
		m_transform = glm::translate(glm::mat4(1.0f), m_position) * m_orientation;
		m_camera.view = glm::inverse(m_transform);
	}


	void FreeEulerCamController::onEvent(Event & event)
	{

	}
}