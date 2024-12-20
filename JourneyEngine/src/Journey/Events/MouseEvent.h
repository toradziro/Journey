#pragma once

#include "Event.h"

namespace jny
{

class  MouseButtonPressedEvent : public Event
{
public:
	MouseButtonPressedEvent(int button) : m_buttonCode(button) { }
	~MouseButtonPressedEvent() = default;

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
	~MouseButtonReleasedEvent() = default;

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

	MouseMovedEvent(f32 x, f32 y) : m_mouseX(x), m_mouseY(y) {}
	~MouseMovedEvent() = default;

	std::string toString() const override { return fmt::format("MouseMovedEvent: x: {}, y: {}", m_mouseX, m_mouseY); }
	
	f32 getX() const { return m_mouseX; }
	f32 getY() const { return m_mouseY; }

private:
	f32 m_mouseX;
	f32 m_mouseY;
};

class  MouseScrolledEvent : public Event
{
public:
	EVENT_CATEGORY(Event::EventCategory::Mouse | Event::EventCategory::Input)
	EVENT_CLASS_TYPE(MouseScrolled)

	MouseScrolledEvent(float offsetX, float offsetY) : m_offsetX(offsetX), m_offsetY(offsetY) {}
	~MouseScrolledEvent() = default;

	std::string toString() const override { return fmt::format("MouseScrolledEvent: offsetX: {}, offsetY: {}", m_offsetX, m_offsetY); }

	float offsetX() const { return m_offsetX; }
	float offsetY() const { return m_offsetY; }

private:
	float m_offsetX;
	float m_offsetY;
};

} //-- jny