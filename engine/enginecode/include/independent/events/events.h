/* \file events.h */

#pragma once

namespace Engine
{
	/** \enum EventType
	* An enum class for all the event types. 
	* Using the class type as safer than plain enum.
	*/
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowNoFocus, WindowMove,
		KeyPress, KeyRelease, KeyType,
		MouseMouse, MouseScroll, MouseButtonPress, MouseButtonRelease
	};

	/** \enum EventCategory
	* An enum for type category flags.
	* an older plain classical version of enum use bitshifting/operators; 
	* as something can belong to more than one category, can use flags.
	*/
	enum EventCategory
	{
		None = 0,
		EventCategoryWindow = 1 << 0,		//!< 00000001
		EventCategoryInput = 1 << 1,		//!< 00000010
		EventCategoryKey = 1 << 2,			//!< 00000100
		EventCategoryMouse = 1 << 3,		//!< 00001000
		EventCategoryButton = 1 << 4		//!< 00010000
	};

	/** \class Event
	* Base class for Event.
	*/
	class Event
	{
	public:
		virtual EventType getEventType() const = 0;		//!< get the event type.
		virtual int getCategoryFlag() const = 0;		//!< get the event category.
		inline bool isEventHandled() const { return m_eventHandled; }			//!< has the event in question been handled.
		inline void handleEvent(bool isEventHandled) { m_eventHandled = isEventHandled; }				//!< handle the relevant event.
		inline bool isEventInCategory(EventCategory category) { return getCategoryFlag() & category; }	//!< is this event in a category
	private:
		bool m_eventHandled = false;
	};


}
