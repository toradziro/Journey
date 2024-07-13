#pragma once

#include "Core.h"
#include <spdlog/spdlog.h>
#include <fmt/core.h>

#include <memory>

namespace jny
{

class JNY_API Log
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
#ifdef JNY_BUILD_DLL
		auto& logger = coreLogger();
#else
		auto& logger = clientLogger();
#endif
		switch (level) {
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
	}

	inline static std::shared_ptr<spdlog::logger>& coreLogger()
	{
		return s_coreLogger;
	};

	inline static std::shared_ptr<spdlog::logger>& clientLogger()
	{
		return s_clientLogger;
	}

private:
	static std::shared_ptr<spdlog::logger> s_coreLogger;
	static std::shared_ptr<spdlog::logger> s_clientLogger;
};

} //-- jny
