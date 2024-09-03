#pragma once

#include "Journey/Core/Core.h"

namespace jny
{

#define EVENT_CLASS_TYPE(type)	static Event::EventType staticEventType() { return Event::EventType::type; } \
								virtual Event::EventType eventType() const override { return staticEventType(); } \
								virtual const char* name() const override { return #type; }

#define EVENT_CATEGORY(category) virtual int categoryFlags() const override { return static_cast<int>(category); }

class  Event
{
private:
	friend class EventDispatcher;
public:
	enum class EventType : uint16_t
	{
		None = 0,
		
		//-- Window Events
		WindowClose,
		WindowResize,
		WindowFocus,
		WindowLostFocus,
		WindowMoved,
		
		//-- AppEvents
		AppTick,
		AppUpdate,
		AppRender,
		
		//-- Keyboard events
		KeyPressed,
		KeyReleased,
		KeyTyped,

		//-- Mouse events
		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,
		MouseScrolled
	};

	enum EventCategory
	{
		None			= 0,
		Application		= BIT(0),
		Input			= BIT(1),
		Keyboard		= BIT(2),
		Mouse			= BIT(3),
		MouseButton		= BIT(4)
	};

public:
	virtual EventType eventType() const = 0;
	virtual const char* name() const = 0;
	virtual int categoryFlags() const = 0;
	virtual std::string toString() const { return name(); }

	inline bool isInCategory(EventCategory category) const { return static_cast<int>(category) & categoryFlags(); }

	inline bool handeled() const { return m_handeled; }

	virtual ~Event() = default;

protected:
	bool m_handeled = false;
};

class EventDispatcher
{
private:
	template<typename T>
	using EventFn = std::function<bool(T&)>;

public:
	EventDispatcher(Event& event) : m_event(event) {}

	template<typename T>
	bool dispatch(EventFn<T> handler)
	{
		if (m_event.eventType() == T::staticEventType())
		{
			//-- We need to downcast to child for proper handling
			m_event.m_handeled = handler(*static_cast<T*>(&m_event));
			return true;
		}
		return false;
	}

private:
	Event& m_event;
};

} //-- jny