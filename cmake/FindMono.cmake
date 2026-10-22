# FindMono.cmake
#
# Locates the Mono runtime SDK for embedding Mono in C/C++ applications.
#
# Point CMake at the Mono installation root with either:
#   -DMONO_ROOT=/path/to/mono       (cache variable, e.g. CMakeUserPresets.json)
#   export MONO_ROOT=/path/to/mono  (environment variable)
#
# Result:
#   Mono::Mono                      imported shared-library target
#   MONO_FOUND                      TRUE if Mono was found
#   MONO_VERSION                    e.g. "6.12.0"
#   MONO_SONAME                     runtime file name (e.g. mono-2.0-sgen.dll or libmonosgen-2.0.so.1)
#   MONO_INCLUDE_DIRS               include directories
#   MONO_LIBRARIES                  libraries to link

set(MONO_ROOT "${MONO_ROOT}" CACHE PATH "Root directory of the Mono SDK")

set(_mono_hints "${MONO_ROOT}" "$ENV{MONO_ROOT}")

if(WIN32)
    list(APPEND _mono_hints
        "$ENV{ProgramFiles}/Mono"
        "$ENV{ProgramFiles\(x86\)}/Mono"
        "C:/Program Files/Mono"
        "C:/Program Files (x86)/Mono"
    )
    set(_mono_lib_names mono-2.0-sgen libmono-static-sgen mono-2.0)
    set(_mono_dll_names mono-2.0-sgen.dll mono-2.0.dll)
else()
    list(APPEND _mono_hints
        "/usr"
        "/usr/local"
        "/opt/mono"
        "$ENV{HOME}/mono"
    )
    set(_mono_lib_names monosgen-2.0 mono-2.0)
endif()

find_path(MONO_INCLUDE_DIR
    NAMES mono/jit/jit.h
    HINTS ${_mono_hints}
    PATH_SUFFIXES include/mono-2.0 mono-2.0
)

find_library(MONO_LIBRARY
    NAMES ${_mono_lib_names}
    HINTS ${_mono_hints}
    PATH_SUFFIXES lib lib64
)

if(WIN32)
    find_file(MONO_DLL
        NAMES ${_mono_dll_names}
        HINTS ${_mono_hints}
        PATH_SUFFIXES bin
    )
endif()

find_program(MONO_EXECUTABLE
    NAMES mono
    HINTS ${_mono_hints}
    PATH_SUFFIXES bin
)

if(MONO_EXECUTABLE)
    execute_process(
        COMMAND "${MONO_EXECUTABLE}" --version
        OUTPUT_VARIABLE _mono_version_raw
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if(_mono_version_raw MATCHES "Mono JIT compiler version ([0-9]+(\\.[0-9]+)+)")
        set(MONO_VERSION "${CMAKE_MATCH_1}")
    endif()
endif()

include(FindPackageHandleStandardArgs)

if(WIN32)
    set(_mono_required_vars
        MONO_LIBRARY
        MONO_DLL
        MONO_INCLUDE_DIR
    )
else()
    set(_mono_required_vars
        MONO_LIBRARY
        MONO_INCLUDE_DIR
    )
endif()

find_package_handle_standard_args(Mono
    REQUIRED_VARS ${_mono_required_vars}
    VERSION_VAR MONO_VERSION
)

function(_mono_soname library out_var)
    file(REAL_PATH "${library}" _real)
    get_filename_component(_name "${_real}" NAME)
    if(_name MATCHES "^(.+\\.so\\.[0-9]+)")
        set(${out_var} "${CMAKE_MATCH_1}" PARENT_SCOPE)
    else()
        set(${out_var} "${_name}" PARENT_SCOPE)
    endif()
endfunction()

if(Mono_FOUND)
    set(MONO_INCLUDE_DIRS "${MONO_INCLUDE_DIR}")
    set(MONO_LIBRARIES "${MONO_LIBRARY}")

    if(WIN32)
        get_filename_component(MONO_SONAME "${MONO_DLL}" NAME)

        if(NOT TARGET Mono::Mono)
            add_library(Mono::Mono SHARED IMPORTED)
            set_target_properties(Mono::Mono PROPERTIES
                IMPORTED_LOCATION "${MONO_DLL}"
                IMPORTED_IMPLIB "${MONO_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${MONO_INCLUDE_DIR}"
                INTERFACE_LINK_LIBRARIES "ws2_32;psapi;winmm;version"
            )
        endif()
    else()
        _mono_soname("${MONO_LIBRARY}" MONO_SONAME)

        if(NOT TARGET Mono::Mono)
            add_library(Mono::Mono SHARED IMPORTED)
            set_target_properties(Mono::Mono PROPERTIES
                IMPORTED_LOCATION "${MONO_LIBRARY}"
                IMPORTED_SONAME "${MONO_SONAME}"
                INTERFACE_INCLUDE_DIRECTORIES "${MONO_INCLUDE_DIR}"
                INTERFACE_LINK_LIBRARIES "${CMAKE_DL_LIBS};m;pthread"
            )
        endif()
    endif()
endif()

mark_as_advanced(
    MONO_INCLUDE_DIR
    MONO_LIBRARY
    MONO_DLL
    MONO_EXECUTABLE
)
