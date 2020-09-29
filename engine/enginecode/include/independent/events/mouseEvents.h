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
		MouseButtonPressEvent(const int b) : m_Button(b) {}			//!< using initaliser list to set button straight to m_Button.
		inline int getButtonPressed() const { return m_Button; }	//!< get the mouse button pressed.
	private:
		int m_Button;		//!< int for mouse button pressed.
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
		MouseButtonReleaseEvent(const int b) : m_Button(b) {}		//!< using initaliser list to set button straight to m_Button.
		inline int getButtonReleased() const { return m_Button; }	//!< get the mouse button released.
	private:
		int m_Button;		//!< int for mouse button released.
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
		MouseMovementEvent(const float x, const float y) : m_mouseMoveX(x), m_mouseMoveY(y) {}		//!< using initaliser list to set x/y straight to m_mouseMoveX/Y.
		inline float getXMovement() const { return m_mouseMoveX; }		//!< get the mouse movement coords on the X axis.
		inline float getYMovement() const { return m_mouseMoveY; }		//!< get the mouse movement coords on the Y axis.
	private:
		float m_mouseMoveX;		//!< mouse movement X coords.
		float m_mouseMoveY;		//!< mouse movement Y coords.
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
		MouseScrollEvent(const float x, const float y) : m_scrollX(x), m_scrollY(y) {}	//!< using initaliser list to set x/y straight to m_scrollX/Y.
		inline float getXMouseScroll() const { return m_scrollX; }		//!< get the scroll X float.
		inline float getYMouseScroll() const { return m_scrollY; }		//!< get the scroll Y float.
	private:
		float m_scrollX;	//!< float for X of scroll.
		float m_scrollY;	//!< float for Y of scroll.
	};
}