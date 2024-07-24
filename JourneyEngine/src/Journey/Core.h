#pragma once

#ifdef JNY_PLATFORM_WINDOWS
	#ifdef JNY_BUILD_DLL
		#define JNY_API __declspec(dllexport)
	#else
		#define JNY_API __declspec(dllimport)
	#endif
#else
	#error Only Win supported
#endif

#define BIT(x) (1 << x)

#include "Journey/Log/Log.h"

#ifdef JNY_ENABLE_ASSERTS
#define JNY_ASSERT(x, ...) { if(!(x)) { jny::Log::log(jny::Log::LogLevel::error, "Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define JNY_ASSERT(x, ...)
#define JNY_CORE_ASSERT(x, ...)
#endif