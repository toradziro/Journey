#pragma once

#include "Journey/Core.h"

namespace jny
{

#pragma warning(push)
#pragma warning(disable: 4251)

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
#ifdef JNY_BUILD_DLL
		auto& logger = coreLogger();
#else
		auto& logger = clientLogger();
#endif //-- JNY_BUILD_DLL
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
		log(LogLevel::trace, formatStr, std::forward<Args>(args)...);
	}

	template<typename... Args>
	static void info(const std::string& formatStr, Args&&... args)
	{
		log(LogLevel::info, formatStr, std::forward<Args>(args)...);
	}

	template<typename... Args>
	static void warning(const std::string& formatStr, Args&&... args)
	{
		log(LogLevel::warning, formatStr, std::forward<Args>(args)...);
	}

	template<typename... Args>
	static void error(const std::string& formatStr, Args&&... args)
	{
		log(LogLevel::error, formatStr, std::forward<Args>(args)...);
	}

private:
	inline static std::shared_ptr<spdlog::logger>& coreLogger()
	{
		return s_coreLogger;
	};

	inline static std::shared_ptr<spdlog::logger>& clientLogger()
	{
		return s_clientLogger;
	}

	static std::shared_ptr<spdlog::logger> s_coreLogger;
	static std::shared_ptr<spdlog::logger> s_clientLogger;
};

#pragma warning(pop)

} //-- jny
