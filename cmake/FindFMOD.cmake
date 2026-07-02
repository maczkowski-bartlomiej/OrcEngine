  find_path(FMOD_CORE_INCLUDE_DIR
      NAMES fmod.hpp fmod_common.h fmod_errors.h
      HINTS
          "$ENV{FMOD_ROOT}/api/core/inc"
  )

  find_path(FMOD_STUDIO_INCLUDE_DIR
      NAMES fmod_studio.hpp fmod_studio_common.h
      HINTS
          "$ENV{FMOD_ROOT}/api/studio/inc"
  )

  find_library(FMOD_CORE_LIBRARY
      NAMES fmod
      HINTS
          "$ENV{FMOD_ROOT}/api/core/lib/x86_64"
  )

  find_library(FMOD_STUDIO_LIBRARY
      NAMES fmodstudio
      HINTS
          "$ENV{FMOD_ROOT}/api/studio/lib/x86_64"
  )

  include(FindPackageHandleStandardArgs)

  find_package_handle_standard_args(FMOD
      REQUIRED_VARS
          FMOD_CORE_INCLUDE_DIR
          FMOD_STUDIO_INCLUDE_DIR
          FMOD_CORE_LIBRARY
          FMOD_STUDIO_LIBRARY
  )

  if(FMOD_FOUND AND NOT TARGET FMOD::Core)
      add_library(FMOD::Core SHARED IMPORTED)
      set_target_properties(FMOD::Core PROPERTIES
          IMPORTED_LOCATION "${FMOD_CORE_LIBRARY}"
          INTERFACE_INCLUDE_DIRECTORIES "${FMOD_CORE_INCLUDE_DIR}"
      )

      add_library(FMOD::Studio SHARED IMPORTED)
      set_target_properties(FMOD::Studio PROPERTIES
          IMPORTED_LOCATION "${FMOD_STUDIO_LIBRARY}"
          INTERFACE_INCLUDE_DIRECTORIES "${FMOD_STUDIO_INCLUDE_DIR};${FMOD_CORE_INCLUDE_DIR}"
      )
  endif()
