/** \file freeOthroCamController.h*/
#pragma once
#include "camera/camera.h"
#include "platform/GLFW/GLFWInputPoller.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
	/** \class FreeOthroCamController
	*	\brief Class for controlling a free moving orthographic camera.
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

		}	//!< constructor.

		//void updateCameraVectors();											//!< update the camera vectors.
		virtual inline Camera& getCamera() override { return m_camera; }	//!< get the camera.
		virtual void onUpdate(float time) override;							//!< on update function.
		virtual void onEvent(Event& event) override;						//!< on event function.	//TO-DO: change to do just resizing - will need to change projectionmatrix.	

		/* will likely need function that can handle key and mouse events for camera movement.
		void handleKeyPress( direction, timestep );
		void handleMouseMove(float xoffset, float yoffset);
		void handleMouseScroll(float yoffset)
		*/

	private:
		glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };						//!< vec3 to take position of camera.
		const float m_zoom = 1.0f;											//!< 
		float m_rotation = 0.0f;											//!< the rotation (in degrees) of camera.
		float m_CamMovementSpeed = 30.0f;									//!< speed of translation in scene.
		float m_CamRotationSpeed = 60.0f;									//!< speed of rotation in scene.
	};
}


/*

#pragma once
#include "camera/camera.h"
#include "platform/GLFW/GLFWInputPoller.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
	\class FreeOthroCamController
	*	\brief Class for controlling a free moving orthographic camera.

	class FreeOthroCamController : public CameraController
	{
	public:
		FreeOthroCamController(glm::vec3 pos, glm::vec3 up, float yaw, float pitch) :
			m_position(pos),
			m_upwards(up),
			yawAngle(yaw),
			pitchAngle(pitch)
		{
			updateCameraVectors();
		}	//!< constructor.

		void updateCameraVectors();											//!< update the camera vectors.
		virtual inline Camera& getCamera() override { return m_camera; }	//!< get the camera.
		virtual void onUpdate(float time) override;							//!< on update function.
		virtual void onEvent(Event& event) override;						//!< on event function.
		virtual void normaliseAngle() override;								//!< normalise the angle.		

 will likely need function that can handle key and mouse events for camera movement.
		void handleKeyPress( direction, timestep );
		void handleMouseMove(float xoffset, float yoffset);
		void handleMouseScroll(float yoffset)

	private:
		glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };						//!< vec3 to take position of camera.

		glm::vec3 m_forwards;												//!< vec3 for forward (+/-) movement.
		glm::vec3 m_sideways;												//!< vec3 for sideways (+/-) movement.
		glm::vec3 m_upwards = { 0.0f, 1.0f, 0.0f };							//!< which way is up.

		float yawAngle = 0.0f;												//!< 
		float pitchAngle = 0.0f;											//!< 
		const float m_zoom = 45.0f;											//!< 
		const float m_maxPitchAngle = 89.0f;								//!< 

		float m_rotation;													//!< the rotation (in degrees) of camera.
		float m_CamMovementSpeed = 3.0f;									//!< speed of translation in scene.
		float m_CamRotationSpeed = 200.0f;									//!< speed of rotation in scene.
		glm::mat4 m_viewProjection;											//!< mat4 to take the view/projection of the camera.
	};
}


*/