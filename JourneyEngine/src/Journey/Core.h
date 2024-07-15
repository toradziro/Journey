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

#include <memory>
#include <string>
#include <functional>
#include <cstdint>
#include <fmt/core.h>