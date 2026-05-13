cmake_minimum_required(VERSION 3.10)

# Nome del progetto
project(lines)

if (APPLE)
  set(CMAKE_MACOSX_RPATH ON)
  list(APPEND CMAKE_INSTALL_RPATH "/opt/homebrew/lib")
elseif (WIN32)
  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif()

# 2. Cerca il pacchetto SDL3 nel sistema
find_package(SDL3 REQUIRED)

# 3. Definisci l'eseguibile (cambia 'clear' col nome che preferisci)
add_executable(lines lines.c)

# 4. Collega le librerie di SDL3 e include automaticamente gli header
target_link_libraries(lines PRIVATE SDL3::SDL3)
