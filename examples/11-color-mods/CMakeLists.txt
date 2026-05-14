cmake_minimum_required(VERSION 3.10)

# Project Name
project(color-mods)

# OS specific configurations
if (APPLE)
  set(CMAKE_MACOSX_RPATH ON)
  set(DEST_DIR "${CMAKE_BINARY_DIR}/macos")
  list(APPEND CMAKE_INSTALL_RPATH "/opt/homebrew/lib")
elseif (UNIX AND NOT APPLE)
  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
  set(DEST_DIR "${CMAKE_BINARY_DIR}/unix")
endif()

# Find SDL3
find_package(SDL3 REQUIRED)

# Set destination directory and compile name
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${DEST_DIR})
add_executable(color-mods color-mods.c)

target_link_libraries(color-mods PRIVATE SDL3::SDL3)
