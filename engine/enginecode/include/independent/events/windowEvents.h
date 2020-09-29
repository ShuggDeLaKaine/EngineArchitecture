/* \file windowEvents.h */
#pragma once

#include "events.h"
#include <glm/glm.hpp>


namespace Engine
{
	/**
	\class WindowCloseEvent - class for window closing event.
	*/
	class WindowCloseEvent : public Event
	{
	public:
		static EventType getStaticType() { return EventType::WindowClose; }			//!< returns static type; reason for static type is that is gives me something to compare with when implementing a handler.
		virtual inline EventType getEventType() const override { return EventType::WindowClose; }	//!< get the event type.
		virtual int32_t getCategoryFlag() const override { return EventCategoryWindow; }			//!< get the category flag for the event.
	};

	/**
	\class WindowResizeEvent - class for resizing window event.
	*/
	class WindowResizeEvent : public Event
	{
	public:
		static EventType getStaticType() { return EventType::WindowResize; }					//!< returns static type; reason for static type is that is gives me something to compare with when implementing a handler.
		virtual inline EventType getEventType() const override { return EventType::WindowResize; }	//!< get the event type.
		virtual int32_t getCategoryFlag() const override { return EventCategoryWindow; }			//!< get the category flag for the event.
		WindowResizeEvent(int32_t width, int32_t height) : m_width(width), m_height(height) { }	//!< using initaliser list as will set width/height straight to m_width/m_height rather than to 0 first.
		inline int32_t getWidth() const { return m_width; }		//!< get the width of window.
		inline int32_t getHeight() const { return m_height; }	//!< get the height of window.
	private:
		int32_t m_width;		//!< window width after resize.
		int32_t m_height;		//!< window height after resize.
	};

	/**
	\class WindowFocusEvent - class for bringing window into focus event.
	*/
	class WindowFocusEvent : public Event
	{
	public:
		static EventType getStaticType() { return EventType::WindowFocus; }		//!< returns static type; reason for static type is that is gives me something to compare with when implementing a handler.
		virtual inline EventType getEventType() const override { return EventType::WindowFocus; }	//!< get the event type.
		virtual int32_t getCategoryFlag() const override { return EventCategoryWindow; }			//!< get the category flag for the event.
		//NEED - initialised function for the Window Focus.

	};

	/**
	\class WindowLostFocusEvent - class for minimising window out of focus event.
	*/
	class WindowLostFocusEvent : public Event
	{
	public:
		static EventType getStaticType() { return EventType::WindowLostFocus; }		//!< returns static type; reason for static type is that is gives me something to compare with when implementing a handler.
		virtual inline EventType getEventType() const override { return EventType::WindowLostFocus; }	//!< get the event type.
		virtual int32_t getCategoryFlag() const override { return EventCategoryWindow; }				//!< get the category flag for the event.
		//NEED - initialised function for the Window Lost Focus.

	};

	/**
	\class WindowMoveEvent - class for window movement event.
	*/
	class WindowMoveEvent : public Event
	{
	public:
		static EventType getStaticType() { return EventType::WindowMove; }		//!< returns static type; reason for static type is that is gives me something to compare with when implementing a handler.
		virtual inline EventType getEventType() const override { return EventType::WindowMove; }	//!< get the event type.
		virtual int32_t getCategoryFlag() const override { return EventCategoryWindow; }			//!< get the category flag for the event.

		//NEED - initialised function for the Window Move.
		WindowMoveEvent(const float x, const float y) : windowMoveX(x), windowMoveY(y) {}		//!< initaliser, listed the x/y straight to windowMoveX/Y.

		//NEED - get functions for float x & y for window movement.
		inline float getWindowXMove() { return windowMoveX; }		//!< get the float for window movement on the X axis.
		inline float getWindowYMove() { return windowMoveY; }		//!< get the float for window movement on the Y axis.

	private:
		//NEED - floats for new window position on the x and the y
		float windowMoveX;		//!< float for X position of window movement.
		float windowMoveY;		//!< float for Y position of window movement.
	};
}

