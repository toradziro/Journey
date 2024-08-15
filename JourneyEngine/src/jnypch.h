#pragma once

#include <memory>

void* operator new[](size_t size, const char* name, int flags, unsigned debugFlags, const char* file, int line);
void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* name, int flags, unsigned debugFlags, const char* file, int line);
void operator delete[](void* p, const char* name, int flags, unsigned debugFlags, const char* file, int line) noexcept;
void operator delete[](void* p, size_t alignment, size_t alignmentOffset, const char* name, int flags, unsigned debugFlags, const char* file, int line) noexcept;

#include <iostream>
#include <string>
#include <sstream>
#include <functional>
#include <chrono>

#ifdef JNY_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace eastl
{

void AssertionFailure(const char* pExpression);

}

#include <EASTL/allocator.h>
#include <EASTL/algorithm.h>
#include <EASTL/vector.h>
#include <EASTL/vector_map.h>
#include <EASTL/vector_set.h>
#include <EASTL/map.h>
#include <EASTL/set.h>
#include <EASTL/queue.h>
#include <EASTL/unordered_map.h>
#include <EASTL/unordered_set.h>

#define SPDLOG_HEADER_ONLY
#include <spdlog/spdlog.h>
#include <spdlog/fmt/bundled/format.h>