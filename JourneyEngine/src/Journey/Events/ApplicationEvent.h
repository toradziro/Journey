#pragma once

#include "Event.h"

namespace jny
{

class JNY_API WindowCloseEvent : public Event
{
public:
	WindowCloseEvent() {}
	~WindowCloseEvent() {}

	EVENT_CATEGORY(Event::EventCategory::Application)
	EVENT_CLASS_TYPE(WindowClose)
};

class JNY_API WindowResizeEvent : public Event
{
public:
	EVENT_CATEGORY(Event::EventCategory::Application)
	EVENT_CLASS_TYPE(WindowResize)

	WindowResizeEvent(int width, int height) : m_width(width), m_height(height) { }
	~WindowResizeEvent() {}

	inline int width() const { return m_width; }
	inline int height() const { return m_height; }

	std::string toString() const override { return spdlog::fmt_lib::format("WindowResizeEvent: w({}), h({})", m_width, m_height); }

private:
	int m_width;
	int m_height;
};

class JNY_API ApplicationTickEvent : public Event
{
public:
	ApplicationTickEvent() {}
	~ApplicationTickEvent() {}

	EVENT_CATEGORY(Event::EventCategory::Application)
	EVENT_CLASS_TYPE(AppTick)
};

class JNY_API ApplicationUpdateEvent : public Event
{
public:
	ApplicationUpdateEvent() {}
	~ApplicationUpdateEvent() {}

	EVENT_CATEGORY(Event::EventCategory::Application)
	EVENT_CLASS_TYPE(AppUpdate)
};

class JNY_API ApplicationRenderEvent : public Event
{
public:
	ApplicationRenderEvent() {}
	~ApplicationRenderEvent() {}

	EVENT_CATEGORY(Event::EventCategory::Application)
	EVENT_CLASS_TYPE(AppRender)
};

} //-- jny