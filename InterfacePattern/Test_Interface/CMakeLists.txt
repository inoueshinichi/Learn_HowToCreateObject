cmake_minimum_required(VERSION 3.14.6)

include(../../is_test_case.cmake)

#Headers
file(GLOB INTERFACE_HEADERS RELATIVE ../Interface ./*.hpp) # Interface
file(GLOB OBJECT_HEADERS RELATIVE ../Object ./*.hpp) # Object

set(BUILD_HEADERS "")
list(APPEND BUILD_HEADERS ${INTERFACE_HEADERS})
list(APPEND BUILD_HEADERS ${OBJECT_HEADERS})

# Sources
file(GLOB INTERFACE_SOURCES RELATIVE ../Interface ./*.cpp) # Interface
file(GLOB OBJECT_SOURCES RELATIVE ../Object ./*.cpp) # Object

set(BUILD_SOURCES "")
list(APPEND BUILD_SOURCES ${INTERFACE_SOURCES})
list(APPEND BUILD_SOURCES ${OBJECT_SOURCES})

list(APPEND BUILD_SOURCES "test_interface_exe.cpp")

include_directories("../Interface")
include_directories("../Object")

make_is_test_case(TEST_INTERFACE ON OFF "${BUILD_HEADERS}" "${BUILD_SOURCES}")

