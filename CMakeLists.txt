cmake_minimum_required(VERSION 3.25)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
	if(DEFINED ENV{VCPKG_ROOT} AND EXISTS "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
		set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "vcpkg toolchain")
	elseif(EXISTS "$ENV{HOME}/vcpkg/scripts/buildsystems/vcpkg.cmake")
		set(CMAKE_TOOLCHAIN_FILE "$ENV{HOME}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "vcpkg toolchain")
	endif()
endif()

project(OrcEngine LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(OrcWarnings)

option(ORC_BUILD_TEST_PROJECT "Build TestProject demo" ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Dependencies -----------------------------------------------------------
# vcpkg (vcpkg.json)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(tinyxml2 CONFIG REQUIRED)
find_package(Freetype REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_path(STB_INCLUDE_DIRS stb_image.h REQUIRED)

# System
find_package(OpenGL REQUIRED)

# Manually installed SDKs (cmake/FindFMOD.cmake)
find_package(FMOD REQUIRED)

# .NET Hosting (nethost & CoreCLR delegates)
set(_DOTNET_ROOT "")
if(DEFINED ENV{DOTNET_ROOT} AND EXISTS "$ENV{DOTNET_ROOT}")
    set(_DOTNET_ROOT "$ENV{DOTNET_ROOT}")
elseif(WIN32)
    set(_DOTNET_ROOT "$ENV{ProgramFiles}/dotnet")
elseif(EXISTS "/usr/share/dotnet")
    set(_DOTNET_ROOT "/usr/share/dotnet")
elseif(EXISTS "$ENV{HOME}/.local/share/JetBrains/Toolbox/apps/rider/lib/ReSharperHost/linux-x64/dotnet")
    set(_DOTNET_ROOT "$ENV{HOME}/.local/share/JetBrains/Toolbox/apps/rider/lib/ReSharperHost/linux-x64/dotnet")
elseif(EXISTS "$ENV{HOME}/.local/share/JetBrains/Toolbox/apps/clion/plugins/clion-radler/DotFiles/linux-x64/dotnet")
    set(_DOTNET_ROOT "$ENV{HOME}/.local/share/JetBrains/Toolbox/apps/clion/plugins/clion-radler/DotFiles/linux-x64/dotnet")
endif()

if(WIN32)
    set(_DOTNET_RID "win-x64")
else()
    set(_DOTNET_RID "linux-x64")
endif()

file(GLOB _DOTNET_PACK_NATIVE_DIRS
    "${_DOTNET_ROOT}/packs/Microsoft.NETCore.App.Host.${_DOTNET_RID}/[0-9]*/runtimes/${_DOTNET_RID}/native"
    "${_DOTNET_ROOT}/packs/Microsoft.NETCore.App.Host.${_DOTNET_RID}/[0-9]*/native"
)
list(REVERSE _DOTNET_PACK_NATIVE_DIRS)

find_path(DOTNET_HOST_INC
    NAMES coreclr_delegates.h hostfxr.h nethost.h
    HINTS ${_DOTNET_PACK_NATIVE_DIRS}
    DOC "Path to .NET Core hosting headers (coreclr_delegates.h, hostfxr.h, nethost.h)"
)
find_library(NETHOST_LIB
    NAMES nethost
    HINTS ${_DOTNET_PACK_NATIVE_DIRS}
    DOC "Path to nethost library"
)

if(WIN32)
    find_file(NETHOST_DLL
        NAMES nethost.dll
        HINTS ${_DOTNET_PACK_NATIVE_DIRS}
    )
endif()

find_program(DOTNET_EXECUTABLE
    NAMES dotnet
    HINTS
        "${_DOTNET_ROOT}"
        "$ENV{DOTNET_ROOT}"
        "$ENV{ProgramFiles}/dotnet"
        "/usr/share/dotnet"
        "$ENV{HOME}/.local/share/JetBrains/Toolbox/apps/rider/lib/ReSharperHost/linux-x64/dotnet"
        "$ENV{HOME}/.local/share/JetBrains/Toolbox/apps/clion/plugins/clion-radler/DotFiles/linux-x64/dotnet"
        "/usr/bin"
        "/usr/local/bin"
)

# --- OrcEngine --------------------------------------------------------------
set(ORC_ENGINE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/OrcEngine")

file(GLOB_RECURSE ORC_ENGINE_SOURCES CONFIGURE_DEPENDS
    "${ORC_ENGINE_DIR}/source/Orc/*.cpp"
    "${ORC_ENGINE_DIR}/source/Orc/*.inl"
    "${ORC_ENGINE_DIR}/include/Orc/*.hpp"
    "${ORC_ENGINE_DIR}/include/Orc/*.inl"
)

add_library(OrcEngine STATIC ${ORC_ENGINE_SOURCES})

target_include_directories(OrcEngine
    PUBLIC
        "${ORC_ENGINE_DIR}/include"
        "${ORC_ENGINE_DIR}/include/Orc"
    PRIVATE
        "${ORC_ENGINE_DIR}/source/Orc"
)

target_include_directories(OrcEngine SYSTEM PRIVATE "${STB_INCLUDE_DIRS}" "${DOTNET_HOST_INC}")

target_link_libraries(OrcEngine
    PUBLIC
        glm::glm
        fmt::fmt
        spdlog::spdlog
        FMOD::Studio
        FMOD::Core
    PRIVATE
        glfw
        glad::glad
        tinyxml2::tinyxml2
        Freetype::Freetype
        OpenGL::GL
        imgui::imgui
        ${NETHOST_LIB}
)

# PUBLIC: Core/Platform.hpp (a public header) branches on these macros, so the engine
# and every consumer must agree on them. Every non-Debug config counts as release.
target_compile_definitions(OrcEngine
    PUBLIC
        $<$<CONFIG:Debug>:ORC_DEBUG>
        $<$<NOT:$<CONFIG:Debug>>:ORC_RELEASE>
        $<$<PLATFORM_ID:Windows>:ORC_PLATFORM_WINDOWS>
        $<$<PLATFORM_ID:Linux>:ORC_PLATFORM_LINUX>
        FMT_USE_OPTIMIZE_PRAGMA=0
)

target_precompile_headers(OrcEngine PRIVATE "${ORC_ENGINE_DIR}/include/Orc/OrcPch.hpp")
target_compile_options(OrcEngine
    PUBLIC
        $<$<CXX_COMPILER_ID:MSVC>:/Zc:preprocessor>
)
orc_enable_warnings(OrcEngine)

# --- TestProject ------------------------------------------------------------
if(ORC_BUILD_TEST_PROJECT)
    set(TEST_PROJECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/TestProject")

    file(GLOB_RECURSE TEST_PROJECT_SOURCES CONFIGURE_DEPENDS
        "${TEST_PROJECT_DIR}/source/*.cpp"
        "${TEST_PROJECT_DIR}/include/*.hpp"
    )

    add_executable(TestProject ${TEST_PROJECT_SOURCES})

    target_include_directories(TestProject PRIVATE "${TEST_PROJECT_DIR}/include")
    target_link_libraries(TestProject PRIVATE OrcEngine)
    orc_enable_warnings(TestProject)

    # Automatically compile managed C# scripts into the executable directory
    if(DOTNET_EXECUTABLE)
        add_custom_target(OrcEngineScripting ALL
            COMMAND "${DOTNET_EXECUTABLE}" build "${CMAKE_CURRENT_SOURCE_DIR}/OrcEngine.Scripting/OrcEngine.Scripting.csproj"
                -c $<CONFIG>
                -o "$<TARGET_FILE_DIR:TestProject>"
            COMMENT "Building OrcEngine.Scripting (.NET CoreCLR)..."
            VERBATIM
        )
        add_dependencies(TestProject OrcEngineScripting)
    endif()

    # Look for the deployed FMOD libraries next to the executable (Linux ELF).
    if(UNIX AND NOT APPLE)
        set_target_properties(TestProject PROPERTIES BUILD_RPATH "$ORIGIN")
    endif()

    # Make the build directory runnable: FMOD runtime libs (under their soname),
    # assets and shaders (the renderer loads "shaders/*.glsl" relative to the CWD).
    add_custom_command(TARGET TestProject POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:FMOD::Core>"
            "$<TARGET_FILE_DIR:TestProject>/${FMOD_CORE_SONAME}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:FMOD::Studio>"
            "$<TARGET_FILE_DIR:TestProject>/${FMOD_STUDIO_SONAME}"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${TEST_PROJECT_DIR}/assets"
            "$<TARGET_FILE_DIR:TestProject>/assets"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${TEST_PROJECT_DIR}/shaders"
            "$<TARGET_FILE_DIR:TestProject>/shaders"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${NETHOST_DLL}"
            "$<TARGET_FILE_DIR:TestProject>/"
        VERBATIM
    )
endif()
