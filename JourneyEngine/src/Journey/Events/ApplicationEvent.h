#pragma once

#include "Event.h"

namespace jny
{

class  WindowCloseEvent : public Event
{
public:
	WindowCloseEvent() = default;
	~WindowCloseEvent() = default;

	EVENT_CATEGORY(Event::EventCategory::Application)
	EVENT_CLASS_TYPE(WindowClose)
};

class  WindowResizeEvent : public Event
{
public:
	EVENT_CATEGORY(Event::EventCategory::Application)
	EVENT_CLASS_TYPE(WindowResize)

	WindowResizeEvent(int width, int height) : m_width(width), m_height(height) { }
	~WindowResizeEvent() = default;

	inline int width() const { return m_width; }
	inline int height() const { return m_height; }

	std::string toString() const override { return spdlog::fmt_lib::format("WindowResizeEvent: w({}), h({})", m_width, m_height); }

private:
	int m_width;
	int m_height;
};

class  ApplicationTickEvent : public Event
{
public:
	ApplicationTickEvent() = default;
	~ApplicationTickEvent() = default;

	EVENT_CATEGORY(Event::EventCategory::Application)
	EVENT_CLASS_TYPE(AppTick)
};

class  ApplicationUpdateEvent : public Event
{
public:
	ApplicationUpdateEvent() = default;
	~ApplicationUpdateEvent() = default;

	EVENT_CATEGORY(Event::EventCategory::Application)
	EVENT_CLASS_TYPE(AppUpdate)
};

class  ApplicationRenderEvent : public Event
{
public:
	ApplicationRenderEvent() = default;
	~ApplicationRenderEvent() = default;

	EVENT_CATEGORY(Event::EventCategory::Application)
	EVENT_CLASS_TYPE(AppRender)
};

} //-- jny