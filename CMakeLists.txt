cmake_minimum_required(VERSION 3.22.1)
project(Coreful LANGUAGES CXX)

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
file(GLOB_RECURSE COMMON_SRC CONFIGURE_DEPENDS
        src/main/main.cpp
        src/main/common/*.cpp
        src/main/common/*.h
)

# === Platform-specific compile definitions ===
if (WIN32)
    file(GLOB_RECURSE PLATFORM_SRC CONFIGURE_DEPENDS
            src/main/platform/windows/*.cpp
            src/main/platform/windows/*.h
    )
elseif (UNIX)
    file(GLOB_RECURSE PLATFORM_SRC CONFIGURE_DEPENDS
            src/main/platform/linux/*.cpp
            src/main/platform/linux/*.h
    )
endif ()

# === Executable ===
add_executable(Coreful
        ${COMMON_SRC}
        ${PLATFORM_SRC}
)

# === Include Directories ===
target_include_directories(Coreful PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/common
)

# ==========================================================
# Vulkan
# ==========================================================

find_package(Vulkan REQUIRED)
if (Vulkan_FOUND)
    message(STATUS "Found Vulkan SDK: ${Vulkan_INCLUDE_DIR}")
    target_link_libraries(Coreful PRIVATE Vulkan::Vulkan)
endif ()



# ==========================================================
# Linux (X11)
# ==========================================================
if (UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)

    target_link_libraries(Coreful PRIVATE X11)

    target_compile_definitions(Coreful PRIVATE
            VK_USE_PLATFORM_XLIB_KHR
    )
endif ()

# === Warnings ===
if (MSVC)
    target_compile_options(Coreful PRIVATE /W4)
else ()
    target_compile_options(Coreful PRIVATE -Wall -Wextra -Wpedantic)
endif ()



