#include "engine_pch.h"
#include "camera/freeEulerCamController.h"

namespace Engine
{

	void FreeEulerCamController::updateCameraVectors()
	{

	}

	void FreeEulerCamController::onUpdate(float time)
	{

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