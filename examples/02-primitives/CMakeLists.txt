cmake_minimum_required(VERSION 3.10)

# Nome del progetto
project(primitives)

if (APPLE)
  set(CMAKE_MACOSX_RPATH ON)
  set(DEST_DIR "${CMAKE_BINARY_DIR}/macos")
  list(APPEND CMAKE_INSTALL_RPATH "/opt/homebrew/lib")
elseif (UNIX AND NOT APPLE)
  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
  set(DEST_DIR "${CMAKE_BINARY_DIR}/unix")
endif()

find_package(SDL3 REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${DEST_DIR})
add_executable(primitives primitives.c)

target_link_libraries(primitives PRIVATE SDL3::SDL3)
