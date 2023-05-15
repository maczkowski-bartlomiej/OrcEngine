#pragma once

#ifdef ORC_PLATFORM_WINDOWS //clean Windows API shit
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif

	#ifndef WIN32_EXTRA_LEAN
		#define WIN32_EXTRA_LEAN
	#endif

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
#endif

#ifdef ORC_PLATFORM_WINDOWS
	#include <Windows.h>
#endif

#include <string>
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

#include "Engine/GameLayer.hpp"
#include "Engine/GameLayerStack.hpp"
#include "Engine/ResourceHolder.hpp"

#include "Events/Event.hpp"
#include "Events/MouseEvents.hpp"
#include "Events/WindowEvents.hpp"
#include "Events/KeyboardEvents.hpp"

#include "Graphics/Shader.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Window.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/IndexBuffer.hpp"
#include "Graphics/BufferLayout.hpp"
#include "Graphics/RenderCommand.hpp"

#include "Input/Mouse.hpp"
#include "Input/Keyboard.hpp"

#include "Math/Vector2.hpp"
#include "Math/Vector3.hpp"
#include "Math/Vector4.hpp"

#include "System/System.hpp"

#include "Tools/Debug.hpp"
#include "Tools/Logger.hpp"
#include "Tools/Utility.hpp"
