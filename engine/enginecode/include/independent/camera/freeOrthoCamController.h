/** \file freeOthroCamController.h*/
#pragma once
#include "camera/camera.h"
#include "platform/GLFW/GLFWInputPoller.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
	/** \class FreeOthroCamController
	*	\brief Class for controlling a free moving orthographic camera (2D).
	*/
	class FreeOthroCamController : public CameraController
	{	
	public:
		FreeOthroCamController(glm::vec3 pos, float rot, float left, float right, float bottom, float top) :
			m_position(pos),
			m_rotation(rot)
		{
			m_camera.projection = glm::ortho(left, right, bottom, top);
			m_camera.view = glm::inverse(glm::translate(glm::mat4(1.0f), m_position) * glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation), { 0.0f, 0.0f, 1.0f }));

		}	//!< constructor with its position and rotation; then left, right, bottom and top to set with the window in application.cpp

		virtual inline Camera& getCamera() override { return m_camera; }	//!< get the camera.
		virtual void onUpdate(float time) override;							//!< on update function.
		virtual void onEvent(Event& event) override;						//!< on event function.	//TO-DO: change to do just resizing - will need to change projectionmatrix.	
	
	private:
		glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };						//!< vec3 to take position of camera.
		const float m_zoom = 1.0f;											//!< rate of the zoom.
		float m_rotation = 0.0f;											//!< the rotation (in degrees) of camera.
		float m_CamMovementSpeed = 100.0f;									//!< speed of translation in scene.
		float m_CamRotationSpeed = 30.0f;									//!< speed of rotation in scene.
	};
}