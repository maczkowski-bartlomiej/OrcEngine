#pragma once

#ifdef ORC_PLATFORM_WINDOWS //clean Windows API shit

	#define WIN32_LEAN_AND_MEAN
	#define WIN32_EXTRA_LEAN

	#define NOIME
	#define NOWINRES
	#define NOGDICAPMASKS
	#define NOVIRTUALKEYCODES
	//#define NOWINMESSAGES
	//#define NOWINSTYLES
	#define NOSYSMETRICS
	//#define NOMENUS
	//#define NOICONS
	#define NOKEYSTATES
	//#define NOSYSCOMMANDS
	//#define NORASTEROPS
	////#define NOSHOWWINDOW
	//#define OEMRESOURCE
	//#define NOATOM
	//#define NOCLIPBOARD
	//#define NOCOLOR
	//#define NOCTLMGR
	////#define NODRAWTEXT
	//#define NOGDI
	////#define NOUSER
	//#define NOMB
	//#define NOMEMMGR
	//#define NOMETAFILE
	//#define NOMINMAX
	//#define NOMSG
	#define NOOPENFILE
	#define NOSCROLL
	#define NOSERVICE
	#define NOSOUND
	#define NOTEXTMETRIC
	#define NOWH
	#define NOWINOFFSETS
	#define NOCOMM
	#define NOKANJI
	//#define NOHELP
	#define NOPROFILER
	#define NODEFERWINDOWPOS
	#define NOMCX
	#define NOIME
	#define NOPROXYSTUB
	#define NOIMAGE
	#define NO
	#define NOTAPE
	//#define ANSI_ONLY
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
