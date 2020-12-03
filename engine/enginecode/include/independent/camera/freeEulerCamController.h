/** \file freeEulerCamController.cpp */

#pragma once
#include "camera/camera.h"
#include "platform/GLFW/GLFWInputPoller.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
	/*	\class FreeEulerCamController
	*	\brief Class for controlling a free moving euler camera (3D).
	*/
	class FreeEulerCamController : public CameraController
	{
	public:
		FreeEulerCamController(glm::vec3 pos, glm::vec3 forward, glm::vec3 up, glm::vec3 side) :
			m_position(pos),
			m_forwards(forward),
			m_upwards(up),
			m_sideways(side)
		{
			//m_transform =  { m_sideways, m_upwards, m_forwards, m_position };

			//updateCameraVectors();
		}	//!< constructor.

		void updateCameraVectors();											//!< update the camera vectors.
		virtual inline Camera& getCamera() override { return m_camera; }	//!< get the camera.
		virtual void onUpdate(float time) override;							//!< on update function.
		virtual void onEvent(Event& event) override;						//!< on event function.
	
		/*
		will likely need function that can handle mouse events for camera movement.
			void handleMouseMove(float xoffset, float yoffset);
			void handleMouseScroll(float yoffset)
		*/

	private:
		glm::mat4 m_transform;												//!< transform to give location of camera. A mat4 to take vec3s position, forwards, sideways and upwards.
		glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };						//!< vec3 to take position of camera.
		glm::vec3 m_forwards;												//!< vec3 for forward (+/-) movement.
		glm::vec3 m_sideways;												//!< vec3 for sideways (+/-) movement.
		glm::vec3 m_upwards = { 0.0f, 1.0f, 0.0f };							//!< which way is up.

		float yawAngle = 0.0f;												//!< 
		float pitchAngle = 0.0f;											//!< 
		const float m_zoom = 45.0f;											//!< 
		const float m_maxPitchAngle = 89.0f;								//!< 

		//float m_rotation;													//!< the rotation (in degrees) of camera.
		glm::vec3 m_rotation;												//!< rotationof camera, vec3 as in 3d.
		float m_CamMovementSpeed = 3.0f;									//!< speed of translation in scene.
		float m_CamRotationSpeed = 200.0f;									//!< speed of rotation in scene.
		glm::mat4 m_viewProjection;											//!< mat4 to take the view/projection of the camera.
	};
}


/*
		//virtual void normaliseAngle() override;							//!< normalise the angle.


*/