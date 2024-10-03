#pragma once

#include "Event.h"

namespace jny
{

class  KeyEvent : public Event
{
public:
	virtual ~KeyEvent() = default;

	EVENT_CATEGORY(Event::EventCategory::Keyboard | Event::EventCategory::Input)

	inline int keyCode() const { return m_keyCode; }

protected:
	KeyEvent(int keycode) : m_keyCode(keycode) {}

	int m_keyCode;
};

class  KeyPressedEvent : public KeyEvent
{
public:
	KeyPressedEvent(int keyCode, int repeatCount) : KeyEvent(keyCode), m_repeatCount(repeatCount) {}
	~KeyPressedEvent() = default;

	EVENT_CLASS_TYPE(KeyPressed)

	inline int repeatCount() const { return m_repeatCount; }
	std::string toString() const override
	{
		return fmt::format("KeyPressedEvent: {} ({} repeats)", m_keyCode, m_repeatCount);
	}

private:
	int m_repeatCount;
};

class  KeyReleasedEvent : public KeyEvent
{
public:
	KeyReleasedEvent(int keyCode) : KeyEvent(keyCode) {}
	~KeyReleasedEvent() = default;

	std::string toString() const override
	{
		return fmt::format("KeyReleasedEvent: {}", m_keyCode);
	}

	EVENT_CLASS_TYPE(KeyReleased)
};

class  KeyTypedEvent : public KeyEvent
{
public:
	KeyTypedEvent(int keyCode) : KeyEvent(keyCode) {}
	~KeyTypedEvent() = default;

	std::string toString() const override
	{
		return fmt::format("KeyTypedEvent: {}", m_keyCode);
	}

	EVENT_CLASS_TYPE(KeyTyped)
};

} //-- jny