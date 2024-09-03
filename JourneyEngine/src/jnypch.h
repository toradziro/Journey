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

#include <ranges>

#include <vector>
#include <map>
#include <set>
#include <queue>
#include <unordered_map>
#include <unordered_set>

#ifdef JNY_PLATFORM_WINDOWS
#include <Windows.h>
#endif

#define SPDLOG_HEADER_ONLY
#include <spdlog/spdlog.h>
#include <spdlog/fmt/bundled/format.h>

#include <Journey/Core/Reference.h>