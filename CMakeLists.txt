cmake_minimum_required(VERSION 3.25)

  project(OrcEngine LANGUAGES CXX)

  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

  option(ORC_BUILD_TEST_PROJECT "Build TestProject demo" ON)

  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)

  find_package(glfw3 CONFIG REQUIRED)
  find_package(glad CONFIG REQUIRED)
  find_package(glm CONFIG REQUIRED)
  find_package(spdlog CONFIG REQUIRED)
  find_package(tinyxml2 CONFIG REQUIRED)
  find_package(Freetype REQUIRED)
  find_package(OpenGL REQUIRED)
  find_package(imgui CONFIG REQUIRED)
  find_package(FMOD REQUIRED)

  find_path(STB_INCLUDE_DIRS stb_image.h REQUIRED)

  file(GLOB_RECURSE ORC_ENGINE_SOURCES CONFIGURE_DEPENDS
      "${CMAKE_CURRENT_SOURCE_DIR}/OrcEngine/source/Orc/*.cpp"
      "${CMAKE_CURRENT_SOURCE_DIR}/OrcEngine/source/Orc/*.inl"
      "${CMAKE_CURRENT_SOURCE_DIR}/OrcEngine/include/Orc/*.hpp"
      "${CMAKE_CURRENT_SOURCE_DIR}/OrcEngine/include/Orc/*.cpp"
      "${CMAKE_CURRENT_SOURCE_DIR}/OrcEngine/include/Orc/*.inl"
      "${CMAKE_CURRENT_SOURCE_DIR}/OrcEngine/include/dependencies/*.h"
      "${CMAKE_CURRENT_SOURCE_DIR}/OrcEngine/include/dependencies/*.c"
      "${CMAKE_CURRENT_SOURCE_DIR}/OrcEngine/include/dependencies/*.hpp"
      "${CMAKE_CURRENT_SOURCE_DIR}/OrcEngine/include/dependencies/*.cpp"
  )

  add_library(OrcEngine STATIC ${ORC_ENGINE_SOURCES})

  target_include_directories(OrcEngine
      PUBLIC
          "${CMAKE_CURRENT_SOURCE_DIR}/OrcEngine/include"
          "${CMAKE_CURRENT_SOURCE_DIR}/OrcEngine/include/Orc"
      PRIVATE
          "${CMAKE_CURRENT_SOURCE_DIR}/OrcEngine/include/dependencies"
          "${STB_INCLUDE_DIRS}"
  )

  target_link_libraries(OrcEngine
      PUBLIC
          glm::glm
          spdlog::spdlog
          FMOD::Core
          FMOD::Studio
      PRIVATE
          glfw
          glad::glad
          tinyxml2::tinyxml2
          Freetype::Freetype
          OpenGL::GL
          imgui::imgui
  )

  target_compile_definitions(OrcEngine
      PRIVATE
          $<$<CONFIG:Debug>:ORC_DEBUG>
          $<$<CONFIG:Release>:ORC_RELEASE>
  )

  if(ORC_BUILD_TEST_PROJECT)
      file(GLOB_RECURSE TEST_PROJECT_SOURCES CONFIGURE_DEPENDS
          "${CMAKE_CURRENT_SOURCE_DIR}/TestProject/source/*.cpp"
          "${CMAKE_CURRENT_SOURCE_DIR}/TestProject/include/*.hpp"
      )

      add_executable(TestProject ${TEST_PROJECT_SOURCES})


      target_include_directories(TestProject PRIVATE
          "${CMAKE_CURRENT_SOURCE_DIR}/TestProject/include"
      )

      target_link_libraries(TestProject PRIVATE OrcEngine)

      set_target_properties(TestProject PROPERTIES
      BUILD_RPATH "$ORIGIN"
      )


    add_custom_command(TARGET TestProject POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
          "$<TARGET_FILE:FMOD::Core>"
          "$<TARGET_FILE_DIR:TestProject>"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
          "$<TARGET_FILE:FMOD::Studio>"
          "$<TARGET_FILE_DIR:TestProject>"
    )

      add_custom_command(TARGET TestProject POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy_directory
              "${CMAKE_CURRENT_SOURCE_DIR}/TestProject/assets"
              "$<TARGET_FILE_DIR:TestProject>/assets"
      )
  endif()
