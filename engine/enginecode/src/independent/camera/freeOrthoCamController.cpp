/** \file freeOrthoCamController.cpp */

#include "engine_pch.h"
#include "camera/freeOrthoCamController.h"
#include "core/inputPoller.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

namespace Engine
{
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

	void FreeOthroCamController::onUpdate(float time)
	{
		if (GLFWInputPoller::isKeyPressed(87))	//W
		{
			m_position.x += -sin(glm::radians(m_rotation)) * m_CamMovementSpeed * time;
			m_position.y += cos(glm::radians(m_rotation)) * m_CamMovementSpeed * time;
		}
		if (GLFWInputPoller::isKeyPressed(83))	//S
		{
			m_position.x -= -sin(glm::radians(m_rotation)) * m_CamMovementSpeed * time;
			m_position.y -= cos(glm::radians(m_rotation)) * m_CamMovementSpeed * time;
		}
		if (GLFWInputPoller::isKeyPressed(65))	//A
		{
			m_position.x += cos(glm::radians(m_rotation)) * m_CamMovementSpeed * time;
			m_position.y += sin(glm::radians(m_rotation)) * m_CamMovementSpeed * time;
		}
		if (GLFWInputPoller::isKeyPressed(68))	//D
		{
			m_position.x -= cos(glm::radians(m_rotation)) * m_CamMovementSpeed * time;
			m_position.y -= sin(glm::radians(m_rotation)) * m_CamMovementSpeed * time;
		}
		if (GLFWInputPoller::isKeyPressed(81))	//Q
		{
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
		if (GLFWInputPoller::isKeyPressed(69))	//E
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
		//projection = glm::ortho(left, right, bottom, top);
		view = glm::mat4(1.0f);
		m_viewProjection = projection * view;

	}

	void FreeOthroCamController::onEvent(Event & event)
	{


	}

	void FreeOthroCamController::normaliseAngle()
	{
		//more than the max (89.0f), then set to the max.
		if (pitchAngle > m_maxPitchAngle)
			pitchAngle = m_maxPitchAngle;

		//more than MINUS max (-89.0f), then set to the MINUS max.
		if (pitchAngle < -m_maxPitchAngle)
			pitchAngle = -m_maxPitchAngle;

		//set the circle for this, once below 0, then make 360, so keeps spinning.
		if (yawAngle < 0.0f)
			yawAngle += 360.0f;
	}
}