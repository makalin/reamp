cmake_minimum_required(VERSION 3.20)
project(ReAmp VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build options
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(ENABLE_LUA "Enable Lua scripting support" ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

# Source files
set(SOURCES
    src/main.cpp
    src/audio/audio_engine.cpp
    src/gui/window.cpp
    src/gui/skin_engine.cpp
    src/gui/gui_renderer.cpp
    src/gui/bitmap.cpp
    src/core/player.cpp
    src/core/playlist.cpp
)

# Headers
set(HEADERS
    include/reamp/audio_engine.h
    include/reamp/window.h
    include/reamp/skin_engine.h
    include/reamp/gui_renderer.h
    include/reamp/player.h
    include/reamp/playlist.h
)

# Platform-specific settings
if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15")
    find_library(COCOA_FRAMEWORK Cocoa REQUIRED)
    find_library(IOKIT_FRAMEWORK IOKit REQUIRED)
    find_library(COREAUDIO_FRAMEWORK CoreAudio REQUIRED)
    find_library(AUDIOTOOLBOX_FRAMEWORK AudioToolbox REQUIRED)
endif()

if(WIN32)
    # Windows-specific settings
endif()

if(UNIX AND NOT APPLE)
    # Linux-specific settings
    find_package(X11 REQUIRED)
endif()

# SDL2 for windowing and input
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# SDL2 has built-in BMP support, SDL2_image is optional for PNG/JPG
# For now, we'll use SDL2's built-in BMP loader
option(USE_SDL2_IMAGE "Use SDL2_image for PNG/JPG support" OFF)
if(USE_SDL2_IMAGE)
    find_package(PkgConfig)
    pkg_check_modules(SDL2_IMAGE sdl2_image)
    if(SDL2_IMAGE_FOUND)
        include_directories(${SDL2_IMAGE_INCLUDE_DIRS})
        add_definitions(-DREAMP_USE_SDL2_IMAGE)
    endif()
endif()

# OpenGL for rendering
find_package(OpenGL REQUIRED)

# Lua (optional)
if(ENABLE_LUA)
    find_package(Lua REQUIRED)
    if(LUA_FOUND)
        include_directories(${LUA_INCLUDE_DIR})
        add_definitions(-DREAMP_ENABLE_LUA)
    endif()
endif()

# Miniaudio (header-only, included via include_directories)

# Create executable
add_executable(reamp ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(reamp
    ${SDL2_LIBRARIES}
    OpenGL::GL
)

# Conditionally link SDL2_image
if(USE_SDL2_IMAGE AND SDL2_IMAGE_FOUND)
    target_link_libraries(reamp ${SDL2_IMAGE_LIBRARIES})
    target_link_directories(reamp PRIVATE ${SDL2_IMAGE_LIBRARY_DIRS})
    target_compile_options(reamp PRIVATE ${SDL2_IMAGE_CFLAGS_OTHER})
endif()

if(APPLE)
    target_link_libraries(reamp
        ${COCOA_FRAMEWORK}
        ${IOKIT_FRAMEWORK}
        ${COREAUDIO_FRAMEWORK}
        ${AUDIOTOOLBOX_FRAMEWORK}
    )
endif()

if(ENABLE_LUA AND LUA_FOUND)
    target_link_libraries(reamp ${LUA_LIBRARIES})
endif()

# Compiler-specific options
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(reamp PRIVATE -Wall -Wextra -Wpedantic)
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_options(reamp PRIVATE -O3 -DNDEBUG)
    endif()
endif()

if(MSVC)
    target_compile_options(reamp PRIVATE /W4)
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_options(reamp PRIVATE /O2 /DNDEBUG)
    endif()
endif()

