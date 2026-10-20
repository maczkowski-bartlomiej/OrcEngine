#pragma once

#ifdef _WIN32
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOWINOFFSETS
#define NOCOMM
#define NOKANJI
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define NOIME
#define NOPROXYSTUB
#define NOIMAGE
#define NO
#define NOTAPE
#define NOMINMAX
#include <Windows.h>
#endif

#include <any>
#include <cstdint>
#include <fstream>
#include <functional>
#include <memory>
#include <ranges>
#include <source_location>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Core/Core.hpp"
#include "Engine/Debug.hpp"
#include "Engine/Logger.hpp"
