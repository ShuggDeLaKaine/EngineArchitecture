/** \file freeOrthoCamController.cpp */

#include "engine_pch.h"
#include "camera/freeOrthoCamController.h"
#include "core/inputPoller.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

namespace Engine
{
	void FreeOthroCamController::onUpdate(float time)
	{
		if (GLFWInputPoller::isKeyPressed(87))	//W
		{
			m_position.x += -sin(glm::radians(m_rotation)) * m_translationSpeed * time;
			m_position.y += cos(glm::radians(m_rotation)) * m_translationSpeed * time;
		}
		if (GLFWInputPoller::isKeyPressed(83))	//S
		{
			m_position.x -= -sin(glm::radians(m_rotation)) * m_translationSpeed * time;
			m_position.y -= cos(glm::radians(m_rotation)) * m_translationSpeed * time;
		}
		if (GLFWInputPoller::isKeyPressed(65))	//A
		{
			m_position.x += cos(glm::radians(m_rotation)) * m_translationSpeed * time;
			m_position.y += sin(glm::radians(m_rotation)) * m_translationSpeed * time;
		}
		if (GLFWInputPoller::isKeyPressed(68))	//D
		{
			m_position.x -= cos(glm::radians(m_rotation)) * m_translationSpeed * time;
			m_position.y -= sin(glm::radians(m_rotation)) * m_translationSpeed * time;
		}
		if (GLFWInputPoller::isKeyPressed(81))	//Q
		{
			m_rotation -= time * m_rotationSpeed;
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
			m_rotation += time * m_rotationSpeed;
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
		projection = glm::ortho(left, right, bottom, top);
		view = glm::mat4(1.0f);
		m_viewProjection = projection * view;

	}

	void FreeOthroCamController::onEvent(Event & event)
	{

	}
}