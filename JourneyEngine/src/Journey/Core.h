#pragma once

#define BIT(x) (1 << x)

#include "Journey/Log/Log.h"

#ifdef JNY_ENABLE_ASSERTS
	#ifdef JNY_PLATFORM_WINDOWS
		#define JNY_ASSERT(x, ...) { if(!(x)) { jny::Log::log(jny::Log::LogLevel::error, "Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
		#else
		#define JNY_ASSERT(x, ...) { if(!(x)) { jny::Log::log(jny::Log::LogLevel::error, "Assertion Failed: {0}", __VA_ARGS__); assert(false); } }
	#endif
#else
#define JNY_ASSERT(x, ...)
#endif