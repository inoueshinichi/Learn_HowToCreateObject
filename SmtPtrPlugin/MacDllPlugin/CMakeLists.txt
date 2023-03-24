cmake_minimum_required(VERSION 3.14.6)

# Target Name
set(DLL_BUILD_TARGET MacDllPlugin)

# Version Management
set(CMAKE_PLUGIN_VERSION_MAJOR 0)
set(CMAKE_PLUGIN_VERSION_MINOR 1)
set(CMAKE_PLUGIN_VERSION_PATCH 0)
set(CMAKE_PLUGIN_AUTHOR "Shinichi Inoue")
set(CMAKE_PLUGIN_EMAIL "inoue.shinichi.1800@gmail.com")

# Project
project(${DLL_BUILD_TARGET} LANGUAGES C CXX VERSION ${CMAKE_PLUGIN_VERSION_MAJOR}.${CMAKE_PLUGIN_VERSION_MINOR}.${CMAKE_PLUGIN_VERSION_PATCH})

# Dll Version
configure_file(version.hpp.in version.hpp @ONLY NEWLINE_STYLE LF) # Unix

# Plugin Headers
file(GLOB MAC_PLUGIN_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.hpp) # Mac

# Plugin Sources
file(GLOB MAC_PLUGIN_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp) # Mac

# Include Directory
target_include_directories(${BUILD_TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
