/** \file freeOrthoCamController.cpp */

#include "engine_pch.h"
#include "camera/freeOrthoCamController.h"
#include "core/inputPoller.h"
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
			m_rotation += time * m_CamRotationSpeed;
			if (m_rotation >  180.0f)
				m_rotation -= 360.0f;
			else if (m_rotation <= -180.0f)
					 m_rotation +=  360.0f;
		}
		if (GLFWInputPoller::isKeyPressed(NG_KEY_E))	//E
		{
			m_rotation -= time * m_CamRotationSpeed;
			if (m_rotation >  180.0f)
				m_rotation -= 360.0f;
			else if (m_rotation <= -180.0f)
					 m_rotation +=  360.0f;
		}

		//update the view matrix.
		m_camera.view = glm::inverse(glm::translate(glm::mat4(1.0f), m_position) * glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation), { 0.0f, 0.0f, 1.0f }));
	}

	void FreeOthroCamController::onEvent(Event & event)
	{
		//for resize of window.
	}
}
