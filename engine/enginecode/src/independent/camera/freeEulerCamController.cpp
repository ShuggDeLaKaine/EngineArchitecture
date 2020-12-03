/** \file freeEulerCamController.cpp */
#include "engine_pch.h"
#include "camera/freeEulerCamController.h"
#include "core/inputPoller.h"
#include "events/codes.h"

namespace Engine
{

	void FreeEulerCamController::updateCameraVectors()
	{

	}

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
	}

	void FreeEulerCamController::onEvent(Event & event)
	{

	}

	/*
	void FreeOthroCamController::updateCameraVectors()
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
}