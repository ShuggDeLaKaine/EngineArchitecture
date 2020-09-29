/* \file mouseEvents.h */
#pragma once

#include "events.h"
#include <glm/glm.hpp>

namespace Engine
{
	/**
	\class MouseButtonPressEvent - class for mouse button press event.
	*/
	class MouseButtonPressEvent : public Event
	{
	public:

		static EventType getStaticType() { return EventType::MouseButtonPress; }	//!< get the static event type, can use to compare.
		virtual inline EventType getEventType() const override { return EventType::MouseButtonPress; }	//!< get the event type.
		virtual int32_t getCategoryFlag() const override { return EventCategory::EventCategoryMouse; }	//!< get the category flag for the event.

		//NEED - a MouseButtonPressEvent which takes in params the relevant mouse button pressed.
		//NEED - a Get() for the relevant mouse button.

	private:
		//NEED - reference to the relevant mouse button.

	};

	/**
	\class MouseButtonReleaseEvent - class for mouse button release event.
	*/
	class MouseButtonReleaseEvent : public Event
	{
	public:
		static EventType getStaticType() { return EventType::MouseButtonRelease; }	//!< get the static event type, can use to compare.
		virtual inline EventType getEventType() const override { return EventType::MouseButtonRelease; }	//!< get the event type.
		virtual int32_t getCategoryFlag() const override { return EventCategory::EventCategoryMouse; }		//!< get the category flag for the event.

		//NEED - a MouseButtonReleaseEvent which takes in params the relevant mouse button released.
		//NEED - a Get() for the relevant mouse button.

	private:
		//NEED - reference to the relevant mouse button.

	};

	/**
	\class MouseMouseEvent - class for mouse movement event.
	*/
	class MouseMovementEvent : public Event
	{
	public:
		static EventType getStaticType() { return EventType::MouseMovement; }	//!< get the static event type, can use to compare.
		virtual inline EventType getEventType() const override { return EventType::MouseMovement; }		//!< get the event type.
		virtual int32_t getCategoryFlag() const override { return EventCategory::EventCategoryMouse; }	//!< get the category flag for the event.

		//NEED - MouseMovementEvent take the new x, y coords of movement.


		//NEED - get()s for the x and y of the mouses movement.
		float getXMovement() const { return m_mouseMoveX; }		//!< 
		float getYMovement() const { return m_mouseMoveY; }		//!< 
	private:
		//NEED - x and y coordinates of the mouse's new position.
		float m_mouseMoveX;		//!< 
		float m_mouseMoveY;		//!< 
	};

	/**
	\class MouseScrollEvent - class for mouse scroll event.
	*/
	class MouseScrollEvent : public Event
	{
	public:
		static EventType getStaticType() { return EventType::MouseScroll; }		//!< get the static event type, can use to compare.
		virtual inline EventType getEventType() const override { return EventType::MouseScroll; }		//!< get the event type.
		virtual int32_t getCategoryFlag() const override { return EventCategory::EventCategoryMouse; }	//!< get the category flag for the event.
	private:
		//NEED - x and y ints for the delta movement of track ball.

	};
}