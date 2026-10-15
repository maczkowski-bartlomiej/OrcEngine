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

#include <string>
#include <fstream>
#include <string_view>
#include <source_location>

#include <ranges>
#include <memory>
#include <cstdint>
#include <functional>

#include <any>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Engine/Core.hpp"
#include "Engine/Debug.hpp"
#include "Engine/Logger.hpp"
