/* \file keyEvents.h */
#pragma once

#include "events.h"
#include <glm/glm.hpp>

namespace Engine
{
	/**
	\class KeyPressedEvent - class for key pressed event.
	*/
	class KeyPressedEvent : public Event
	{
	public:
		KeyPressedEvent(const uint32_t keycode, const uint32_t keycount) {}	//!< 
		
		static EventType getStaticType() { return EventType::KeyPress; }	//!< get the static event type, can use to compare.
		virtual inline EventType getEventType() const override { return EventType::KeyPress; }			//!< get the event type.
		virtual int32_t getCategoryFlag() const override { return EventCategory::EventCategoryKey; }	//!< get the category flag for the event.
		
		inline int32_t GetKeycode() const { return m_keycode; }				//!< get the keycode that has been pressed.
		inline int32_t GetRepeatCount() const { return m_repeatCount; }		//!< get the count if key press repeated.

	private:
		uint32_t m_keycode;			//!< uint32_t to take keycode int.
		uint32_t m_repeatCount;		//!< uint32_t to take the count of repeated input.
	};

	/**
	\class KeyReleasedEvent - class for key released event.
	*/
	class KeyReleasedEvent : public Event
	{
	public:
		static EventType getStaticType() { return EventType::KeyRelease; }	//!< get the static event type, can use to compare.
		virtual inline EventType getEventType() const override { return EventType::KeyRelease; }		//!< get the event type.
		virtual int32_t getCategoryFlag() const override { return EventCategory::EventCategoryKey; }	//!< get the category flag for the event.

		inline int32_t GetKeycode() const { return m_keycode; }				//!< get the keycode that has been pressed.

	private:
		uint32_t m_keycode;			//!< uint32_t to take keycode int.
	};

	/**
	\class KeyTypedEvent - class for key typing event.
	*/
	class KeyTypedEvent : public Event
	{
	public:
		static EventType getStaticType() { return EventType::KeyType; }		//!< get the static event type, can use to compare.
		virtual inline EventType getEventType() const override { return EventType::KeyType; }			//!< get the event type.
		virtual int32_t getCategoryFlag() const override { return EventCategory::EventCategoryKey; }	//!< get the category flag for the event.

		inline int32_t GetKeycode() const { return m_keycode; }				//!< get the keycode that has been pressed.

	private:
		uint32_t m_keycode;			//!< uint32_t to take keycode int.
	};
}