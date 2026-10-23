#include "Script/Scripting.hpp"
#include "Engine/Logger.hpp"
#include "Core/Platform.hpp"

#include <nethost.h>
#include <coreclr_delegates.h>
#include <hostfxr.h>

#include <cstdlib>
#include <filesystem>
#include <string>
#include <string_view>
#include <system_error>
#include <utility>

#if defined(ORC_PLATFORM_WINDOWS)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

#define ORC_SCRIPTING_FATAL_CHECK(condition, message, ...) \
	do { \
		if (!(condition)) \
		{ \
			ORC_LOG_FATAL(message __VA_OPT__(, ) __VA_ARGS__); \
			ORC_DEBUGBREAK(); \
			std::abort(); \
		} \
	} while (false)/t

namespace orc {

namespace {

using PalString = std::basic_string<char_t>;

PalString toPalString(std::string_view str)
{
#if defined(ORC_PLATFORM_WINDOWS)
	return std::filesystem::path(str).wstring();
#else
	return PalString(str);
#endif
}

void* loadLibrary(const FilePath& path)
{
#if defined(ORC_PLATFORM_WINDOWS)
	return static_cast<void*>(::LoadLibraryW(path.c_str()));
#else
	return ::dlopen(path.c_str(), RTLD_LAZY);
#endif
}

void* getExport(void* lib, const char* symbol)
{
#if defined(ORC_PLATFORM_WINDOWS)
	return reinterpret_cast<void*>(::GetProcAddress(static_cast<HMODULE>(lib), symbol));
#else
	return ::dlsym(lib, symbol);
#endif
}

void freeLibrary(void* lib)
{
	if (!lib)
	{
		return;
	}
#if defined(ORC_PLATFORM_WINDOWS)
	::FreeLibrary(static_cast<HMODULE>(lib));
#else
	::dlclose(lib);
#endif
}

template <typename T>
T getRuntimeDelegate(hostfxr_get_runtime_delegate_fn getDelegateFn, hostfxr_handle context, hostfxr_delegate_type type)
{
	void* delegatePtr = nullptr;
	const int rc = getDelegateFn(context, type, &delegatePtr);
	ORC_SCRIPTING_FATAL_CHECK(rc == 0 && delegatePtr != nullptr,
		"Failed to acquire runtime delegate (type: {}). Hostfxr error code: {:#x}",
		static_cast<int>(type), rc);
	return reinterpret_cast<T>(delegatePtr);
}

template <typename T>
T getManagedFunction(
	load_assembly_and_get_function_pointer_fn loader,
	const FilePath& assemblyPath,
	std::string_view typeName,
	std::string_view methodName)
{
	ORC_SCRIPTING_FATAL_CHECK(std::filesystem::exists(assemblyPath),
		"Managed assembly DLL does not exist: {}", assemblyPath.string());

	std::error_code ec;
	const auto canonicalAssemblyPath = std::filesystem::canonical(assemblyPath, ec);
	ORC_SCRIPTING_FATAL_CHECK(!ec,
		"Failed to canonicalize assembly path '{}': {}", assemblyPath.string(), ec.message());

	const auto palTypeName = toPalString(typeName);
	const auto palMethodName = toPalString(methodName);

	void* functionPtr = nullptr;
	const int rc = loader(
		canonicalAssemblyPath.c_str(),
		palTypeName.c_str(),
		palMethodName.c_str(),
		UNMANAGEDCALLERSONLY_METHOD,
		nullptr,
		&functionPtr);

	ORC_SCRIPTING_FATAL_CHECK(rc == 0 && functionPtr != nullptr,
		"Failed to resolve entry point '{}.{}' in assembly '{}'. Error code: {:#x}",
		typeName, methodName, canonicalAssemblyPath.string(), rc);

	return reinterpret_cast<T>(functionPtr);
}

} // namespace

struct Scripting::Impl
{
	FilePath m_configPath;
	void* m_hostfxrLib{nullptr};
	hostfxr_initialize_for_runtime_config_fn m_hostfxrInit{nullptr};
	hostfxr_get_runtime_delegate_fn m_hostfxrGetDelegate{nullptr};
	hostfxr_close_fn m_hostfxrClose{nullptr};
	load_assembly_and_get_function_pointer_fn m_loadAssemblyAndGetFunctionPointer{nullptr};

	explicit Impl(const FilePath& path)
		: m_configPath(path)
	{
	}

	~Impl()
	{
		if (m_hostfxrLib)
		{
			freeLibrary(m_hostfxrLib);
			m_hostfxrLib = nullptr;
		}
	}

	Impl(const Impl&) = delete;
	Impl& operator=(const Impl&) = delete;
	Impl(Impl&&) noexcept = delete;
	Impl& operator=(Impl&&) noexcept = delete;

	void loadHostFxr();
	void initializeRuntime();
};

void Scripting::Impl::loadHostFxr()
{
	char_t hostfxrPath[512];
	size_t bufferSize = sizeof(hostfxrPath) / sizeof(char_t);
	const int rc = get_hostfxr_path(hostfxrPath, &bufferSize, nullptr);
	ORC_SCRIPTING_FATAL_CHECK(rc == 0,
		"Failed to locate hostfxr via nethost. Ensure .NET 10 runtime is installed. Error code: {:#x}", rc);

	m_hostfxrLib = loadLibrary(FilePath(hostfxrPath));
	ORC_SCRIPTING_FATAL_CHECK(m_hostfxrLib != nullptr,
		"Failed to load hostfxr shared library from: {}", FilePath(hostfxrPath).string());

	m_hostfxrInit = reinterpret_cast<hostfxr_initialize_for_runtime_config_fn>(
		getExport(m_hostfxrLib, "hostfxr_initialize_for_runtime_config"));
	m_hostfxrGetDelegate = reinterpret_cast<hostfxr_get_runtime_delegate_fn>(
		getExport(m_hostfxrLib, "hostfxr_get_runtime_delegate"));
	m_hostfxrClose = reinterpret_cast<hostfxr_close_fn>(
		getExport(m_hostfxrLib, "hostfxr_close"));

	ORC_SCRIPTING_FATAL_CHECK(m_hostfxrInit != nullptr && m_hostfxrGetDelegate != nullptr && m_hostfxrClose != nullptr,
		"Failed to resolve required hostfxr exports from loaded library.");
}

void Scripting::Impl::initializeRuntime()
{
	ORC_SCRIPTING_FATAL_CHECK(std::filesystem::exists(m_configPath),
		"Runtime configuration file not found at: {}", m_configPath.string());

	std::error_code ec;
	const auto canonicalConfigPath = std::filesystem::canonical(m_configPath, ec);
	ORC_SCRIPTING_FATAL_CHECK(!ec,
		"Failed to canonicalize runtime configuration path '{}': {}", m_configPath.string(), ec.message());

	ORC_LOG_INFO("Loading .NET runtime configuration: {}", canonicalConfigPath.string());

	hostfxr_handle context = nullptr;
	const int rcInit = m_hostfxrInit(canonicalConfigPath.c_str(), nullptr, &context);
	ORC_SCRIPTING_FATAL_CHECK(rcInit == 0 && context != nullptr,
		"hostfxr_initialize_for_runtime_config failed for '{}'. Error code: {:#x}",
		canonicalConfigPath.string(), rcInit);

	m_loadAssemblyAndGetFunctionPointer = getRuntimeDelegate<load_assembly_and_get_function_pointer_fn>(
		m_hostfxrGetDelegate, context, hdt_load_assembly_and_get_function_pointer);
	m_hostfxrClose(context);

	const auto assemblyPath = canonicalConfigPath.parent_path() / "OrcEngine.Scripting.dll";
	ORC_LOG_INFO("Loading managed assembly: {}", assemblyPath.string());

	using HelloFn = void (*)();
	HelloFn helloFn = getManagedFunction<HelloFn>(
		m_loadAssemblyAndGetFunctionPointer,
		assemblyPath,
		"OrcEngine.Scripting.Class1, OrcEngine.Scripting",
		"Hello");

	ORC_LOG_INFO("Invoking managed entry point Class1.Hello()...");
	helloFn();
}

Scripting::Scripting(const FilePath& configPath)
	: m_impl(createUniquePtr<Impl>(configPath))
{
	ORC_LOG_INFO("Initializing Scripting (.NET 10 CoreCLR)...");
	m_impl->loadHostFxr();
	m_impl->initializeRuntime();
}

Scripting::~Scripting()
{
	ORC_LOG_INFO("Deinitializing Scripting.");
}

Scripting::Scripting(Scripting&&) noexcept = default;
Scripting& Scripting::operator=(Scripting&&) noexcept = default;

} // namespace orc
