cmake_minimum_required(VERSION 3.14.6)

include(../is_test_case.cmake)

#Headers
file(GLOB INTERFACE_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} Interface/*.hpp) # Interface
file(GLOB OBJECT_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} Object/*.hpp) # Object

set(BUILD_HEADERS "")
list(APPEND BUILD_HEADERS ${INTERFACE_HEADERS})
list(APPEND BUILD_HEADERS ${OBJECT_HEADERS})

# Sources
file(GLOB INTERFACE_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} Interface/*.cpp) # Interface
file(GLOB OBJECT_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} Object/*.cpp) # Object

set(BUILD_SOURCES "")
list(APPEND BUILD_SOURCES ${INTERFACE_SOURCES})
list(APPEND BUILD_SOURCES ${OBJECT_SOURCES})

include_directories("./Interface")
include_directories("./Object")

make_is_test_case("${BUILD_HEADERS}" "${BUILD_SOURCES}" ./interface_pattern.cpp)

