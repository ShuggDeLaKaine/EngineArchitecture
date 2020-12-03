/** \file freeOrthoCamController.cpp */

#include "engine_pch.h"
#include "camera/freeOrthoCamController.h"
#include "core/inputPoller.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "events/codes.h"

namespace Engine
{

	void FreeOthroCamController::onUpdate(float time)
	{
		if (GLFWInputPoller::isKeyPressed(NG_KEY_W))	//W
		{
			m_position.x += -sin(glm::radians(m_rotation)) * m_CamMovementSpeed * time;
			m_position.y += cos(glm::radians(m_rotation)) * m_CamMovementSpeed * time;
		}
		if (GLFWInputPoller::isKeyPressed(NG_KEY_S))	//S
		{
			m_position.x -= -sin(glm::radians(m_rotation)) * m_CamMovementSpeed * time;
			m_position.y -= cos(glm::radians(m_rotation)) * m_CamMovementSpeed * time;
		}
		if (GLFWInputPoller::isKeyPressed(NG_KEY_A))	//A
		{
			m_position.x += cos(glm::radians(m_rotation)) * m_CamMovementSpeed * time;
			m_position.y += sin(glm::radians(m_rotation)) * m_CamMovementSpeed * time;
		}
		if (GLFWInputPoller::isKeyPressed(NG_KEY_D))	//D
		{
			m_position.x -= cos(glm::radians(m_rotation)) * m_CamMovementSpeed * time;
			m_position.y -= sin(glm::radians(m_rotation)) * m_CamMovementSpeed * time;
		}
		if (GLFWInputPoller::isKeyPressed(NG_KEY_Q))	//Q
		{
			//to-do - shift to radians.
			m_rotation -= time * m_CamRotationSpeed;
			if (m_rotation > 180.0f)
			{
				m_rotation -= 360.0f;
			}
			else if (m_rotation <= -180.0f)
			{
				m_rotation += 360.0f;
			}
		}
		if (GLFWInputPoller::isKeyPressed(NG_KEY_E))	//E
		{
			m_rotation += time * m_CamRotationSpeed;
			if (m_rotation > 180.0f)
			{
				m_rotation -= 360.0f;
			}
			else if (m_rotation <= -180.0f)
			{
				m_rotation += 360.0f;
			}
		}

		//update the view matrix.
		m_camera.view = glm::inverse(glm::translate(glm::mat4(1.0f), m_position) * glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation), { 0.0f, 0.0f, 1.0f }));
	}

	void FreeOthroCamController::onEvent(Event & event)
	{


	}

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
