#pragma once

#include "Journey/Core/SingletonInterface.h"
#include "Journey/Core/Core.h"
#include "Journey/Core/Application.h"

namespace jny
{

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

	InstrumentTimerScope(std::string_view name) :
		m_name(name)
	{
		m_startPoint = std::chrono::high_resolution_clock::now();
	}

	~InstrumentTimerScope()
	{
		if (!m_stopped)
		{
			stop();
		}
	}

	void stop()
	{
		TimePoint endPoint = std::chrono::high_resolution_clock::now();

		int64_t startMs = std::chrono::duration_cast<std::chrono::microseconds>(m_startPoint.time_since_epoch()).count();
		int64_t endMs = std::chrono::duration_cast<std::chrono::microseconds>(endPoint.time_since_epoch()).count();

		//int64_t timeTakenMcs = std::chrono::duration_cast<std::chrono::microseconds>(endPoint - m_startPoint).count();

		m_stopped = true;

		ProfileResult result(startMs, endMs, m_name);
		Application::subsystems().st<Instrumentor>().writeProfile(result);
	}

private:
	const std::string_view	m_name;
	TimePoint				m_startPoint;
	bool					m_stopped = false;
};

class Instrumentor : ISingleton
{
	JNY_SINGLETON_TYPE(Instrumentor)
public:
	Instrumentor() = default;
	void beginSession(const std::string& name, const std::string& filename = "profile.json")
	{
		m_stream.open(filename);
		writeHeader();
	}

	void endSession()
	{
		writeFooter();
		m_stream.close();
		m_profileCount = 0;
	}

	void writeProfile(const ProfileResult& result)
	{
		const std::lock_guard lock(m_mutex);
			
		if (m_profileCount > 0)
		{
			m_stream << ",";
		}
		++m_profileCount;

		std::string name = std::string(result.m_name);
		std::ranges::replace(name, '"', '\'');

		m_stream << "{";
		m_stream << std::string(R"("cat":"function:",)");
		m_stream << std::string(R"("dur:")") << (result.m_endPoint - result.m_startPoint) << ',';
		m_stream << std::string(R"("name:"")") << name << "\",";
		m_stream << std::string(R"("ph":"X",)");
		m_stream << std::string(R"("pid":0,)");
		m_stream << std::string(R"("tid":0)");
		m_stream << std::string(R"("ts":)") << result.m_startPoint;
		m_stream << "}";

		m_stream.flush();
	}

	void writeHeader()
	{
		const std::lock_guard lock(m_mutex);

		m_stream << std::string(R"({"otherData": {},"traceEvents"):[")");
		m_stream.flush();
	}

	void writeFooter()
	{
		const std::lock_guard lock(m_mutex);

		m_stream << "]}";
		m_stream.flush();
	}

private:
	std::mutex		m_mutex;
	std::ofstream	m_stream;
	int				m_profileCount = 0;
};

#ifdef JNY_DEBUG
#define PROFILE_SCOPE(name) InstrumentTimerScope timer##__LINE__(name)
#define PROFILE_FUNC PROFILE_SCOPE(__FUNCSIG__)
#else
#define PROFILE_SCOPE(name)
#define PROFILE_FUNC
#endif

} //--jny