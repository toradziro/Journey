#include "jnypch.h"
#include "TimeInstruments.h"
#include "Journey/Core/Application.h"
#include "Journey/Core/Core.h"

InstrumentTimerScope::InstrumentTimerScope(std::string_view name) :
	m_name(name)
{
	m_startPoint = std::chrono::high_resolution_clock::now();
}

InstrumentTimerScope::~InstrumentTimerScope()
{
	if (!m_stopped)
	{
		stop();
	}
}

void InstrumentTimerScope::stop()
{
	TimePoint endPoint = std::chrono::high_resolution_clock::now();

	int64_t startMs = std::chrono::duration_cast<std::chrono::microseconds>(m_startPoint.time_since_epoch()).count();
	int64_t endMs = std::chrono::duration_cast<std::chrono::microseconds>(endPoint.time_since_epoch()).count();

	m_stopped = true;

	ProfileResult result(startMs, endMs, m_name);
	jny::Application::subsystems().st<Instrumentor>().writeProfile(result);
}

void Instrumentor::beginSession(const std::string& filename /*= "profile.json"*/)
{
	m_stream.open(filename, std::ios::out | std::ios::trunc);
	writeHeader();
}

void Instrumentor::endSession()
{
	writeFooter();
	m_stream.close();
	m_profileCount = 0;
}

void Instrumentor::writeProfile(const ProfileResult& result)
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
	m_stream << std::string(R"("dur":)") << (result.m_endPoint - result.m_startPoint) << ',';
	m_stream << std::string(R"("name":")") << name << "\",";
	m_stream << std::string(R"("ph":"X",)");
	m_stream << std::string(R"("pid":0,)");
	m_stream << std::string(R"("tid":0,)");
	m_stream << std::string(R"("ts":)") << result.m_startPoint;
	m_stream << "}";

	m_stream.flush();
}

void Instrumentor::writeHeader()
{
	const std::lock_guard lock(m_mutex);

	m_stream << std::string(R"({"otherData": {},"traceEvents":[)");
	m_stream.flush();
}

void Instrumentor::writeFooter()
{
	const std::lock_guard lock(m_mutex);

	m_stream << "]}";
	m_stream.flush();
}
