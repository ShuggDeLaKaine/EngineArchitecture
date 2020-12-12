/** \file freeEulerCamController.h */

#pragma once
#include "camera/camera.h"
#include "platform/GLFW/GLFWInputPoller.h"
#include "core/inputPoller.h"
#include "events/codes.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
	/*	\class FreeEulerCamController
	*	\brief Class for controlling a free moving euler camera (3D).
	*/
	class FreeEulerCamController : public CameraController
	{
	public:
		FreeEulerCamController(glm::vec3 pos, glm::vec3 rot, glm::vec3 forward, glm::vec3 up, glm::vec3 side) :
			m_position(pos),
			m_rotation(rot),
			m_forwards(forward),
			m_upwards(up),
			m_sideways(side)
		{
			m_lastMousePosition = InputPoller::getMousePosition();

			m_camera.projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

			glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), m_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), m_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f), m_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
			m_orientation = rotX * rotY * rotZ;

			m_transform = glm::translate(glm::mat4(1.0f), m_position) * m_orientation;
			m_camera.view = inverse(m_transform);
		};												//!< constructor.

		virtual inline Camera& getCamera() override { return m_camera; }	//!< get the camera.
		virtual void onUpdate(float time) override;							//!< on update function.
		virtual void onEvent(Event& event) override;						//!< on event function.

	private:
		glm::mat4 m_transform;												//!< transform to give location of camera. A mat4 to take vec3s position, forwards, sideways and upwards.
		glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };						//!< vec3 to take position of camera.
		glm::vec3 m_forwards = { 0.0f, 0.0f, -1.0f };						//!< vec3 for forward (+/-) movement.
		glm::vec3 m_sideways = { 1.0f, 0.0f, 0.0f };						//!< vec3 for sideways (+/-) movement.
		glm::vec3 m_upwards = { 0.0f, 1.0f, 0.0f };							//!< which way is up.

		const float m_zoom = 1.0f;											//!< rate of the zoom. 
		float m_sensitivity = 0.1f;											//!< mouse sensitivity.
		glm::vec3 m_rotation = { 0.0f, 0.0f, 0.0f };						//!< rotation of camera, vec3 as in 3d.
		float m_CamMovementSpeed = 50.0f;									//!< speed of translation in scene.
		float m_CamRotationSpeed = 10.0f;									//!< speed of rotation in scene.

		glm::vec2 m_lastMousePosition;										//!< last position of the mouse.
		glm::mat4 m_orientation;											//!< mat4 for the orientation of the camera.
		//glm::mat4 m_viewProjection;										//!< mat4 to take the view/projection of the camera.
	};
}
