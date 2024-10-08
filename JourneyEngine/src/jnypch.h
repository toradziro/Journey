#include <memory>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <functional>
#include <chrono>
#include <atomic>
#include <filesystem>
#include <algorithm>
#include <limits>
#include <random>

#include <ranges>

#include <vector>
#include <map>
#include <set>
#include <queue>
#include <unordered_map>
#include <unordered_set>

#ifdef JNY_PLATFORM_WINDOWS
#define NOMINMAX
#include <Windows.h>
#endif

#define SPDLOG_HEADER_ONLY
#include <spdlog/spdlog.h>
#include <spdlog/fmt/bundled/format.h>

#include <Journey/Core/Reference.h>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using f32 = float;