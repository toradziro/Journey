#pragma once

#include "Journey/Core/SingletonInterface.h"

class Instrumentor;

struct ProfileResult
{
	int64_t				m_startPoint;
	int64_t				m_endPoint;
	std::string_view	m_name;
};

class InstrumentTimerScope
{
public:
	using TimePoint = std::chrono::steady_clock::time_point;

	InstrumentTimerScope(std::string_view name);

	~InstrumentTimerScope();

	void stop();

private:
	const std::string_view	m_name;
	TimePoint				m_startPoint;
	bool					m_stopped = false;
};

class Instrumentor : jny::ISingleton
{
	//-- Can't use macros outside jny namespace
	static std::string type() { return "Instrumentor"; }
	friend jny::SingletonHolder;

public:
	Instrumentor() = default;
	~Instrumentor() = default;
	void beginSession(/*const std::string& name, */const std::string& filename = "profile.json");

	void endSession();

	void writeProfile(const ProfileResult& result);

	void writeHeader();

	void writeFooter();

private:
	std::mutex		m_mutex;
	std::ofstream	m_stream;
	int				m_profileCount = 0;
};

#ifdef JNY_DEBUG 
#define PROFILE_SCOPE(name) InstrumentTimerScope timer##__LINE__(name)
#define PROFILE_FUNC PROFILE_SCOPE(__FUNCSIG__)
#elif JNY_RELEASE
#define PROFILE_SCOPE(name) InstrumentTimerScope timer##__LINE__(name)
#define PROFILE_FUNC PROFILE_SCOPE(__FUNCSIG__)
#else
#define PROFILE_SCOPE(name)
#define PROFILE_FUNC
#endif
