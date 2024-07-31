#pragma once

#define BIT(x) (1 << x)

#include "Journey/Log/Log.h"

#ifdef JNY_ENABLE_ASSERTS
#define JNY_ASSERT(x, ...) { if(!(x)) { jny::Log::log(jny::Log::LogLevel::error, "Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define JNY_ASSERT(x, ...)
#define JNY_CORE_ASSERT(x, ...)
#endif