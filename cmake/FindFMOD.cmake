# FindFMOD.cmake
#
# Locates the FMOD Engine SDK (Core + Studio API). The SDK must be downloaded
# manually from https://www.fmod.com/download (its licence prevents distribution
# through package managers).
#
# Point CMake at the SDK root (the directory that contains "api/") with either:
#   -DFMOD_ROOT=/path/to/fmodstudioapi<ver>linux      (cache variable, e.g. CMakeUserPresets.json)
#   export FMOD_ROOT=/path/to/fmodstudioapi<ver>linux (environment variable)
#
# Result:
#   FMOD::Core, FMOD::Studio             imported shared-library targets
#   FMOD_VERSION                         e.g. "2.03.07"
#   FMOD_CORE_SONAME, FMOD_STUDIO_SONAME runtime file names (e.g. libfmod.so.14)

set(FMOD_ROOT "${FMOD_ROOT}" CACHE PATH "Root directory of the FMOD Engine SDK (contains api/)")

set(_fmod_hints "${FMOD_ROOT}" "$ENV{FMOD_ROOT}")
set(_fmod_arch "x86_64")

find_path(FMOD_CORE_INCLUDE_DIR
    NAMES fmod.hpp fmod_common.h fmod_errors.h
    HINTS ${_fmod_hints}
    PATH_SUFFIXES api/core/inc
)

find_path(FMOD_STUDIO_INCLUDE_DIR
    NAMES fmod_studio.hpp fmod_studio_common.h
    HINTS ${_fmod_hints}
    PATH_SUFFIXES api/studio/inc
)

find_library(FMOD_CORE_LIBRARY
    NAMES fmod
    HINTS ${_fmod_hints}
    PATH_SUFFIXES api/core/lib/${_fmod_arch}
)

find_library(FMOD_STUDIO_LIBRARY
    NAMES fmodstudio
    HINTS ${_fmod_hints}
    PATH_SUFFIXES api/studio/lib/${_fmod_arch}
)

# FMOD_VERSION is encoded as 0xaaaabbcc -> aaaa.bb.cc
if(FMOD_CORE_INCLUDE_DIR AND EXISTS "${FMOD_CORE_INCLUDE_DIR}/fmod_common.h")
    file(STRINGS "${FMOD_CORE_INCLUDE_DIR}/fmod_common.h" _fmod_version_line
        REGEX "^#define[ \t]+FMOD_VERSION[ \t]+0x[0-9A-Fa-f]+"
    )
    if(_fmod_version_line MATCHES "0x([0-9A-Fa-f][0-9A-Fa-f][0-9A-Fa-f][0-9A-Fa-f])([0-9A-Fa-f][0-9A-Fa-f])([0-9A-Fa-f][0-9A-Fa-f])")
        set(_fmod_minor "${CMAKE_MATCH_2}")
        set(_fmod_patch "${CMAKE_MATCH_3}")
        math(EXPR _fmod_major "0x${CMAKE_MATCH_1}")
        set(FMOD_VERSION "${_fmod_major}.${_fmod_minor}.${_fmod_patch}")
    endif()
endif()

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(FMOD
    REQUIRED_VARS
        FMOD_CORE_LIBRARY
        FMOD_STUDIO_LIBRARY
        FMOD_CORE_INCLUDE_DIR
        FMOD_STUDIO_INCLUDE_DIR
    VERSION_VAR FMOD_VERSION
)

# Resolve "libfmod.so" -> "libfmod.so.14.x" and derive the soname "libfmod.so.14",
# which is the file the dynamic loader actually looks for at runtime.
function(_fmod_soname library out_var)
    file(REAL_PATH "${library}" _real)
    get_filename_component(_name "${_real}" NAME)
    if(_name MATCHES "^(.+\\.so\\.[0-9]+)")
        set(${out_var} "${CMAKE_MATCH_1}" PARENT_SCOPE)
    else()
        set(${out_var} "${_name}" PARENT_SCOPE)
    endif()
endfunction()

if(FMOD_FOUND)
    _fmod_soname("${FMOD_CORE_LIBRARY}" FMOD_CORE_SONAME)
    _fmod_soname("${FMOD_STUDIO_LIBRARY}" FMOD_STUDIO_SONAME)

    if(NOT TARGET FMOD::Core)
        add_library(FMOD::Core SHARED IMPORTED)
        set_target_properties(FMOD::Core PROPERTIES
            IMPORTED_LOCATION "${FMOD_CORE_LIBRARY}"
            IMPORTED_SONAME "${FMOD_CORE_SONAME}"
            INTERFACE_INCLUDE_DIRECTORIES "${FMOD_CORE_INCLUDE_DIR}"
        )
    endif()

    if(NOT TARGET FMOD::Studio)
        add_library(FMOD::Studio SHARED IMPORTED)
        set_target_properties(FMOD::Studio PROPERTIES
            IMPORTED_LOCATION "${FMOD_STUDIO_LIBRARY}"
            IMPORTED_SONAME "${FMOD_STUDIO_SONAME}"
            INTERFACE_INCLUDE_DIRECTORIES "${FMOD_STUDIO_INCLUDE_DIR}"
            INTERFACE_LINK_LIBRARIES FMOD::Core
        )
    endif()
endif()

mark_as_advanced(
    FMOD_CORE_INCLUDE_DIR
    FMOD_STUDIO_INCLUDE_DIR
    FMOD_CORE_LIBRARY
    FMOD_STUDIO_LIBRARY
)
