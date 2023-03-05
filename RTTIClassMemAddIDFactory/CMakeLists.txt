cmake_minimum_required(VERSION 3.14.6)

include(../is_test_case.cmake)

#Headers
file(GLOB RTTI_HEADERS RELATIVE ./Rtti ./*.hpp) # Rtti
file(GLOB ENTITY_HEADERS RELATIVE ./Entity ./*.hpp) # Entity
file(GLOB FACTORY_HEADERS RELATIVE ./Factory ./*.hpp) # Factory

set(BUILD_HEADERS "")
list(APPEND BUILD_HEADERS ${RTTI_HEADERS})
list(APPEND BUILD_HEADERS ${ENTITY_HEADERS})
list(APPEND BUILD_HEADERS ${FACTORY_HEADERS})

# Sources
file(GLOB RTTI_SOURCES RELATIVE ./Interface ./*.cpp) # Rtti
file(GLOB ENTITY_SOURCES RELATIVE ./Entity ./*.cpp) # Entity
file(GLOB FACTORY_SOURCES RELATIVE ./Factory ./*.cpp) # Factory

set(BUILD_SOURCES "")
list(APPEND BUILD_SOURCES ${RTTI_SOURCES})
list(APPEND BUILD_SOURCES ${ENTITY_SOURCES})
list(APPEND BUILD_SOURCES ${FACTORY_SOURCES})

include_directories(./Rtti)
include_directories(./Entity)
include_directories(./Factory)

make_is_test_case("${BUILD_HEADERS}" "${BUILD_SOURCES}" ./class_member_address_id_factory.cpp)