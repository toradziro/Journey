#pragma once

#include "Event.h"

namespace jny
{

class  MouseButtonPressedEvent : public Event
{
public:
	MouseButtonPressedEvent(int button) : m_buttonCode(button) { }
	~MouseButtonPressedEvent() {}

	EVENT_CATEGORY(Event::EventCategory::Mouse | Event::EventCategory::MouseButton | Event::EventCategory::Input)
	EVENT_CLASS_TYPE(MouseButtonPressed)

	inline int buttonCode() const { return m_buttonCode; }
	std::string toString() const override { return fmt::format("MouseButtonPressed: {}", m_buttonCode); }

private:
	int m_buttonCode;
};

class  MouseButtonReleasedEvent : public Event
{
public:
	MouseButtonReleasedEvent(int button) : m_buttonCode(button) { }
	~MouseButtonReleasedEvent() {}

	EVENT_CATEGORY(Event::EventCategory::Mouse | Event::EventCategory::MouseButton | Event::EventCategory::Input)
	EVENT_CLASS_TYPE(MouseButtonReleased)

	inline int buttonCode() const { return m_buttonCode; }
	std::string toString() const override { return fmt::format("MouseButtonReleased: {}", m_buttonCode); }

private:
	int m_buttonCode;
};

class  MouseMovedEvent : public Event
{
public:
	EVENT_CATEGORY(Event::EventCategory::Mouse | Event::EventCategory::Input)
	EVENT_CLASS_TYPE(MouseMoved)

	MouseMovedEvent(float x, float y) : m_mouseX(x), m_mouseY(y) {}
	~MouseMovedEvent() {}

	std::string toString() const override { return fmt::format("MouseMovedEvent: x: {}, y: {}", m_mouseX, m_mouseY); }
	
	float getX() const { return m_mouseX; }
	float getY() const { return m_mouseY; }

private:
	float m_mouseX;
	float m_mouseY;
};

class  MouseScrolledEvent : public Event
{
public:
	EVENT_CATEGORY(Event::EventCategory::Mouse | Event::EventCategory::Input)
	EVENT_CLASS_TYPE(MouseScrolled)

	MouseScrolledEvent(float offsetX, float offsetY) : m_offsetX(offsetX), m_offsetY(offsetY) {}
	~MouseScrolledEvent() {}

	std::string toString() const override { return fmt::format("MouseScrolledEvent: offsetX: {}, offsetY: {}", m_offsetX, m_offsetY); }

	float offsetX() const { return m_offsetX; }
	float offsetY() const { return m_offsetY; }

private:
	float m_offsetX;
	float m_offsetY;
};

} //-- jny