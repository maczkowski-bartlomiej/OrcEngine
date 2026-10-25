#include "Script/Scripting.hpp"

#include <filesystem>

#include <coreclr_delegates.h>
#include <hostfxr.h>
#include <nethost.h>

#if defined(ORC_PLATFORM_WINDOWS)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#define ORC_LOAD_LIB(path) ::LoadLibraryW(path)
#define ORC_GET_EXPORT(h, name) ::GetProcAddress(static_cast<HMODULE>(h), name)
#define ORC_FREE_LIB(h) ::FreeLibrary(static_cast<HMODULE>(h))
#define ORC_DOTNET_STR(s) L##s
#else
#include <dlfcn.h>
#define ORC_LOAD_LIB(path) ::dlopen(path, RTLD_LAZY)
#define ORC_GET_EXPORT(h, name) ::dlsym(h, name)
#define ORC_FREE_LIB(h) ::dlclose(h)
#define ORC_DOTNET_STR(s) s
#endif

namespace orc {

Scripting::Scripting(const FilePath& configPath)
	: m_configPath(configPath)
{
	ORC_LOG_INFO("Initializing Scripting.");
	loadHostFxr();
	initializeRuntime();
}

Scripting::~Scripting()
{
	ORC_LOG_INFO("Deinitializing Scripting.");
	if (m_hostfxrLib)
	{
		ORC_FREE_LIB(m_hostfxrLib);
		m_hostfxrLib = nullptr;
	}
}

void Scripting::loadHostFxr()
{
	char_t hostfxrPath[512];
	size_t bufferSize = sizeof(hostfxrPath) / sizeof(char_t);
	const int rc = get_hostfxr_path(hostfxrPath, &bufferSize, nullptr);
	ORC_FATAL_CHECK(rc == 0, "Failed to locate hostfxr via nethost. Ensure .NET 10 runtime is installed. Error code: {:#x}", rc);

	m_hostfxrLib = ORC_LOAD_LIB(hostfxrPath);
	ORC_FATAL_CHECK(m_hostfxrLib != nullptr, "Failed to load hostfxr shared library from: {}", FilePath(hostfxrPath).string());

	m_hostfxrInitializeForRuntimeConfig = reinterpret_cast<void*>(ORC_GET_EXPORT(m_hostfxrLib, "hostfxr_initialize_for_runtime_config"));
	m_hostfxrGetRuntimeDelegate = reinterpret_cast<void*>(ORC_GET_EXPORT(m_hostfxrLib, "hostfxr_get_runtime_delegate"));
	m_hostfxrClose = reinterpret_cast<void*>(ORC_GET_EXPORT(m_hostfxrLib, "hostfxr_close"));

	ORC_FATAL_CHECK(m_hostfxrInitializeForRuntimeConfig != nullptr && m_hostfxrGetRuntimeDelegate != nullptr && m_hostfxrClose != nullptr, "Failed to resolve required hostfxr exports from loaded library.");
}

void Scripting::initializeRuntime()
{
	ORC_FATAL_CHECK(std::filesystem::exists(m_configPath), "Runtime configuration file not found at: {}", m_configPath.string());

	const auto canonicalConfigPath = std::filesystem::canonical(m_configPath);

	hostfxr_handle context = nullptr;
	auto initFn = reinterpret_cast<hostfxr_initialize_for_runtime_config_fn>(m_hostfxrInitializeForRuntimeConfig);
	const int rcInit = initFn(canonicalConfigPath.c_str(), nullptr, &context);
	ORC_FATAL_CHECK(rcInit == 0, "hostfxr_initialize_for_runtime_config failed for '{}'. Error code: {:#x}", canonicalConfigPath.string(), rcInit);

	void* loadAssemblyDelegate = nullptr;
	auto getDelegateFn = reinterpret_cast<hostfxr_get_runtime_delegate_fn>(m_hostfxrGetRuntimeDelegate);
	const int rcDelegate = getDelegateFn(context, hdt_load_assembly_and_get_function_pointer, &loadAssemblyDelegate);
	ORC_FATAL_CHECK(rcDelegate == 0, "Failed to acquire load_assembly_and_get_function_pointer delegate. Error code: {:#x}", rcDelegate);

	m_loadAssemblyAndGetFunctionPointer = loadAssemblyDelegate;
	auto closeFn = reinterpret_cast<hostfxr_close_fn>(m_hostfxrClose);
	closeFn(context);

	const auto assemblyPath = canonicalConfigPath.parent_path() / "OrcEngine.Scripting.dll";
	ORC_FATAL_CHECK(std::filesystem::exists(assemblyPath), "Managed assembly DLL does not exist: {}", assemblyPath.string());

	const auto canonicalAssemblyPath = std::filesystem::canonical(assemblyPath);
	ORC_LOG_INFO("Loading managed assembly: {}", canonicalAssemblyPath.string());

	void* functionPtr = nullptr;
	auto loadAssemblyFn = reinterpret_cast<load_assembly_and_get_function_pointer_fn>(m_loadAssemblyAndGetFunctionPointer);
	const int rcLoad = loadAssemblyFn(canonicalAssemblyPath.c_str(), ORC_DOTNET_STR("OrcEngine.Scripting.Class1, OrcEngine.Scripting"), ORC_DOTNET_STR("Hello"), UNMANAGEDCALLERSONLY_METHOD, nullptr, &functionPtr);

	ORC_FATAL_CHECK(rcLoad == 0, "Failed to resolve entry point 'OrcEngine.Scripting.Class1.Hello' in assembly '{}'. Error code: {:#x}", canonicalAssemblyPath.string(), rcLoad);

	using HelloFn = void (*)();
	const auto helloFn = reinterpret_cast<HelloFn>(functionPtr);
	ORC_LOG_INFO("Invoking managed entry point Class1.Hello()...");
	helloFn();
}

} // namespace orc
