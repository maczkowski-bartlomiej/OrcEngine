cmake_minimum_required(VERSION 3.25)

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

# Manually installed SDK (cmake/FindFMOD.cmake)
find_package(FMOD REQUIRED)

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
)

target_include_directories(OrcEngine SYSTEM PRIVATE "${STB_INCLUDE_DIRS}")

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
)

# PUBLIC: Engine/Core.hpp (a public header) branches on these macros, so the engine
# and every consumer must agree on them. Every non-Debug config counts as release.
target_compile_definitions(OrcEngine
    PUBLIC
        $<$<CONFIG:Debug>:ORC_DEBUG>
        $<$<NOT:$<CONFIG:Debug>>:ORC_RELEASE>
)

target_precompile_headers(OrcEngine PRIVATE "${ORC_ENGINE_DIR}/include/Orc/OrcPch.hpp")
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

    # Look for the deployed FMOD libraries next to the executable.
    set_target_properties(TestProject PROPERTIES BUILD_RPATH "$ORIGIN")

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
        VERBATIM
    )
endif()
