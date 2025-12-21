cmake_minimum_required(VERSION 3.22.1)
project(MixCore LANGUAGES CXX)

# === C++ Standard ===
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# === Output directory ===
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# === Resource Directory Macro ===
set(RESOURCE_DIR "${CMAKE_SOURCE_DIR}/src/resources")
add_compile_definitions(RESOURCE_DIR="${RESOURCE_DIR}")

# === Source Files ===
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
        src/main/*.cpp
        src/main/*.h
)

# === Executable ===
add_executable(MixCore ${SRC_FILES})

# === Include Directories ===
target_include_directories(MixCore PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main
)

# === Vulkan Setup ===

find_package(Vulkan REQUIRED)
if (Vulkan_FOUND)
    message(STATUS "Found Vulkan SDK: ${Vulkan_INCLUDE_DIR}")
    target_link_libraries(MixCore PRIVATE Vulkan::Vulkan)
endif ()

# === Platform-specific compile definitions ===
if (WIN32)
    target_compile_definitions(MixCore PRIVATE OC_PLATFORM_WINDOWS)
elseif (UNIX)
    target_compile_definitions(MixCore PRIVATE OC_PLATFORM_LINUX)
endif()

# === Warnings (optional but recommended) ===
if (MSVC)
    target_compile_options(MixCore PRIVATE /W4)
else()
    target_compile_options(MixCore PRIVATE -Wall -Wextra -Wpedantic)
endif()


