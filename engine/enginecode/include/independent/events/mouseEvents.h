/* \file mouseEvents.h */
#pragma once

#include "events.h"
#include <glm/glm.hpp>

namespace Engine
{
	/**
	\class MouseEvent - class to take a number of funcs and vars that will be common in all mouse event classes.
	*/
	class MouseEvent : public Event
	{
		virtual int32_t getCategoryFlag() const override { return EventCategoryMouse | EventCategoryInput; }	//!< get the category flag for the event.
	};

	/**
	\class MouseButtonPressEvent - class for mouse button press event.
	*/
	class MouseButtonPressEvent : public MouseEvent
	{
	public:
		MouseButtonPressEvent(const int b) : m_Button(b) {}			//!< using initaliser list to set button straight to m_Button.
		static EventType getStaticType() { return EventType::MouseButtonPress; }	//!< get the static event type, can use to compare.
		virtual inline EventType getEventType() const override { return getStaticType(); }	//!< get the event type.
		inline int getButtonPressed() const { return m_Button; }	//!< get the mouse button pressed.
	private:
		int m_Button;		//!< int for mouse button pressed.
	};

	/**
	\class MouseButtonReleaseEvent - class for mouse button release event.
	*/
	class MouseButtonReleaseEvent : public MouseEvent
	{
	public:
		MouseButtonReleaseEvent(const int b) : m_Button(b) {}		//!< using initaliser list to set button straight to m_Button.
		static EventType getStaticType() { return EventType::MouseButtonRelease; }	//!< get the static event type, can use to compare.
		virtual inline EventType getEventType() const override { return getStaticType(); }	//!< get the event type.
		inline int getButtonReleased() const { return m_Button; }	//!< get the mouse button released.
	private:
		int m_Button;		//!< int for mouse button released.
	};

	/**
	\class MouseMouseEvent - class for mouse movement event.
	*/
	class MouseMovementEvent : public MouseEvent
	{
	public:
		MouseMovementEvent(const float x, const float y) : m_mouseMoveX(x), m_mouseMoveY(y) {}		//!< using initaliser list to set x/y straight to m_mouseMoveX/Y.
		static EventType getStaticType() { return EventType::MouseMovement; }	//!< get the static event type, can use to compare.
		virtual inline EventType getEventType() const override { return getStaticType(); }		//!< get the event type.
		inline float getXMovement() const { return m_mouseMoveX; }		//!< get the mouse movement coords on the X axis.
		inline float getYMovement() const { return m_mouseMoveY; }		//!< get the mouse movement coords on the Y axis.
	private:
		float m_mouseMoveX;		//!< mouse movement X coords.
		float m_mouseMoveY;		//!< mouse movement Y coords.
	};

	/**
	\class MouseScrollEvent - class for mouse scroll event.
	*/
	class MouseScrollEvent : public MouseEvent
	{
	public:
		MouseScrollEvent(const float x, const float y) : m_scrollX(x), m_scrollY(y) {}	//!< using initaliser list to set x/y straight to m_scrollX/Y.
		static EventType getStaticType() { return EventType::MouseScroll; }		//!< get the static event type, can use to compare.
		virtual inline EventType getEventType() const override { return getStaticType(); }		//!< get the event type.
		inline float getXMouseScroll() const { return m_scrollX; }		//!< get the scroll X float.
		inline float getYMouseScroll() const { return m_scrollY; }		//!< get the scroll Y float.
	private:
		float m_scrollX;	//!< float for X of scroll.
		float m_scrollY;	//!< float for Y of scroll.
	};
}