#pragma once

#include "Journey/Core/Core.h"

namespace jny
{

#ifdef JNY_PLATFORM_WINDOWS
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

class  Log
{
public:
	enum class LogLevel
	{
		trace,
		info,
		warning,
		error,
	};

	static void init();

	template<typename... Args>
	inline static void log(LogLevel level, const std::string& formatStr, Args&&... args)
	{
#ifndef JNY_DISTR
		auto& logger = coreLogger();
		switch (level)
		{
		case LogLevel::trace:
			logger->trace(fmt::runtime(formatStr), std::forward<Args>(args)...);
			break;
		case LogLevel::info:
			logger->info(fmt::runtime(formatStr), std::forward<Args>(args)...);
			break;
		case LogLevel::warning:
			logger->warn(fmt::runtime(formatStr), std::forward<Args>(args)...);
			break;
		case LogLevel::error:
			logger->error(fmt::runtime(formatStr), std::forward<Args>(args)...);
			break;
		default:
			break;
		}
#endif //-- JNY_DISTR
	}

	template<typename... Args>
	static void trace(const std::string& formatStr, Args&&... args)
	{
		log(LogLevel::trace, "[TRACE] " + formatStr, std::forward<Args>(args)...);
	}

	template<typename... Args>
	static void info(const std::string& formatStr, Args&&... args)
	{
		log(LogLevel::info, "[INFO] " + formatStr, std::forward<Args>(args)...);
	}

	template<typename... Args>
	static void warning(const std::string& formatStr, Args&&... args)
	{
		log(LogLevel::warning, "[WARNING] " + formatStr, std::forward<Args>(args)...);
	}

	template<typename... Args>
	static void error(const std::string& formatStr, Args&&... args)
	{
		log(LogLevel::error, "[ERROR] " + formatStr, std::forward<Args>(args)...);
	}

private:
	inline static std::shared_ptr<spdlog::logger>& coreLogger()
	{
		return s_coreLogger;
	}

	static std::shared_ptr<spdlog::logger> s_coreLogger;
};

#ifdef JNY_PLATFORM_WINDOWS
#pragma warning(pop)
#endif

} //-- jny
