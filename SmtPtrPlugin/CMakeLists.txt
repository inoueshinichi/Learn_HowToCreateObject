cmake_minimum_required(VERSION 3.14.6)

################################
# Plugin Dll
################################

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

include(../is_test_case.cmake)

# Headers
file(GLOB BUILD_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} /Host/*.hpp)

# Sources
file(GLOB BUILD_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} /Host/*.cpp)

# Include Dir
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Host)

# Exe
make_is_test_case("${BUILD_HEADERS}" "${BUILD_SOURCES}" main_plugin_host.cpp)
