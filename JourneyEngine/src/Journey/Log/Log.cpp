#include "jnypch.h"
#include "Journey/Log/Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace jny
{

std::shared_ptr<spdlog::logger> Log::s_coreLogger;

void Log::init()
{
	//-- This pattern will set out logger to print output like:
	//-- "[hh:mm:ss] logger_name: message" with proper color
	//-- %^ start of color segment and %$ is an and of that segment
	spdlog::set_pattern("%^[%T] %n: %v%$");

	s_coreLogger = spdlog::stdout_color_mt("[LOG]");
	s_coreLogger->set_level(spdlog::level::trace);
}

} //-- jny
