cmake_minimum_required(VERSION 3.14.6)

include(../is_test_case.cmake)

#Headers
file(GLOB INTERFACE_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} Interface/*.hpp) # Interface
file(GLOB ENTITY_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} SmtPtrGameEntity/*.hpp) # Game Entity
file(GLOB FACTORY_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} SmtPtrFactory/*.hpp) # Factory

set(BUILD_HEADERS "")
list(APPEND BUILD_HEADERS ${INTERFACE_HEADERS})
list(APPEND BUILD_HEADERS ${ENTITY_HEADERS})
list(APPEND BUILD_HEADERS ${FACTORY_HEADERS})

# Sources
file(GLOB INTERFACE_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} Interface/*.cpp) # Interface
file(GLOB ENTITY_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} SmtPtrGameEntity/*.cpp) # Game Entity
file(GLOB FACTORY_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} SmtPtrFactory/*.cpp) # Factory

set(BUILD_SOURCES "")
list(APPEND BUILD_SOURCES ${INTERFACE_SOURCES})
list(APPEND BUILD_SOURCES ${ENTITY_SOURCES})
list(APPEND BUILD_SOURCES ${FACTORY_SOURCES})

include_directories("./Interface")
include_directories("./SmtPtrGameEntity")
include_directories("./SmtPtrFactory")

make_is_test_case("${BUILD_HEADERS}" "${BUILD_SOURCES}" ./factory_key_string_value_fanctor_and_smt_ptr_with_interface.cpp)