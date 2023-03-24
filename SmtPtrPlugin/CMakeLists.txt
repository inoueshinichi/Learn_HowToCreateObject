cmake_minimum_required(VERSION 3.14.6)

################################
# Plugin Dll
################################

# Policy
if(POLICY CMP0025)
    cmake_policy(SET CMP0025 NEW) # AppleClang
endif()

if(POLICY CMP0115)
    cmake_policy(SET CMP0115 NEW) # explicit source symbol
endif()

# Plugin Directory
set(DLL_PLUGIN_TARGET "")
if(WIN32)
    # Windows
    add_subdirectory(WinDllPlugin)
elseif(UNIX AND NOT APPLE)
    # Linux
    add_subdirectory(LinuxDllPlugin)
elseif(APPLE)
    # MacOS
    add_subdirectory(MacDllPlugin)
else()
    # Error
    message(FATAL_ERROR "Platform: Not match platforms [Windows, macOS, Linux]. Given platform is ${CMAKE_SYSTEM_NAME}")
endif()


################################
# Host Exe
################################
# Host Source
set(HOST_EXE_SOURCE main_plugin_host.cpp)
get_filename_component(HOST_EXE_NAME ${HOST_EXE_SOURCE} NAME_WE)

# Target Name
set(BUILD_TARGET ${HOST_EXE_NAME})

# Project
project(${BUILD_TARGET} LANGUAGES C CXX VERSION 0.1.0)

# Exe
add_executable(${BUILD_TARGET} 
    ${HOST_EXE_SOURCE}
    plugin_manager_host.hpp
    plugin_manager_host.cpp
)