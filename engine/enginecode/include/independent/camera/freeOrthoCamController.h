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
		virtual inline Camera& getCamera() override { return m_camera; }	//!< get the camera.
		virtual void onUpdate(float time) override;							//!< on update function.
		virtual void onEvent(Event& event) override;						//!< on event function.

	private:
		glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };						//!< vec3 to take position of camera.
		float m_rotation;													//!< the rotation (in degrees) of camera.
		float m_translationSpeed = 5.0f;									//!< speed of translation in scene.
		float m_rotationSpeed = 200.0f;										//!< speed of rotation in scene.
		glm::mat4 m_viewProjection;											//!< mat4 to take the view/projection of the camera.
	};
}
