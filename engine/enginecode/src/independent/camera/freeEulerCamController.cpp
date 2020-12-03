/** \file freeEulerCamController.cpp */
#include "engine_pch.h"
#include "camera/freeEulerCamController.h"
#include "core/inputPoller.h"
#include "events/codes.h"

namespace Engine
{
	/*
	void FreeEulerCamController::updateCameraVectors()
	{
		//apply maths (x = -sin yaw * cos pitch, y = sin pitch, z = -cos yaw * cos pith) to x, y, z of forward
		m_forwards.x = -sin(glm::radians(yawAngle)) * cos(glm::radians(pitchAngle));
		m_forwards.y = sin(glm::radians(yawAngle));
		m_forwards.z = -cos(glm::radians(yawAngle)) * cos(glm::radians(pitchAngle));
		//normalise glm::vec3 forward.
		m_forwards = glm::normalize(m_forwards);

		//do same as above for m_sidewards, don't have to bother with .y axis.
		m_sideways.x = -sin(glm::radians(yawAngle)) * cos(glm::radians(pitchAngle));
		m_sideways.y = 0.0f;
		m_sideways.z = -cos(glm::radians(yawAngle)) * cos(glm::radians(pitchAngle));
		//normalise sideways.
		m_sideways = glm::normalize(m_sideways);
	}
	*/

	void FreeEulerCamController::onUpdate(float time)
	{
		glm::vec3 forward = { -m_transform[2][0], -m_transform[2][1], -m_transform[2][2] };
		glm::vec3 right = { m_transform[0][0], m_transform[0][1], m_transform[0][2] };

		if (GLFWInputPoller::isKeyPressed(NG_KEY_W))	//W
			m_position += (forward * m_CamMovementSpeed * time);
		if (GLFWInputPoller::isKeyPressed(NG_KEY_S))	//S
			m_position -= (forward * m_CamMovementSpeed * time);
		if (GLFWInputPoller::isKeyPressed(NG_KEY_A))	//A
			m_position -= (right * m_CamMovementSpeed * time);
		if (GLFWInputPoller::isKeyPressed(NG_KEY_D))	//D
			m_position += (right * m_CamMovementSpeed * time);
		if (GLFWInputPoller::isKeyPressed(NG_KEY_Q))
			m_rotation.z -= m_CamRotationSpeed * time;
		if (GLFWInputPoller::isKeyPressed(NG_KEY_E))
			m_rotation.z += m_CamRotationSpeed * time;

		glm::vec2 currentMousePosition = InputPoller::getMousePosition();
		glm::vec2 deltaMousePosition = currentMousePosition - m_lastMousePosition;
		deltaMousePosition /= glm::vec2(800.0f, 600.0f);
		m_rotation.y -= m_sensitivity * deltaMousePosition.x * time;
		m_rotation.x -= m_sensitivity * deltaMousePosition.y * time;

		m_lastMousePosition = currentMousePosition;

		glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), m_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), m_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f), m_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

		m_orientation = rotX * rotY * rotZ;

		m_transform = glm::translate(glm::mat4(1.0f), m_position) * m_orientation;
		m_camera.view = inverse(m_transform);


		//update the view matrix.
		//m_camera.view = glm::inverse(glm::translate(m_transform, m_position) * glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation), { 0.0f, 0.0f, 1.0f }));
	}

	void FreeEulerCamController::onEvent(Event & event)
	{

	}
}