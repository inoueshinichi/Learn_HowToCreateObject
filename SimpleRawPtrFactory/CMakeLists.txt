cmake_minimum_required(VERSION 3.14.6)

include(../is_test_case.cmake)

#Headers
file(GLOB ENTITY_HEADERS RELATIVE ./GameEntity ./*.hpp) # Entity
file(GLOB FACTORY_HEADERS RELATIVE ./Factory ./*.hpp) # Factory
set(BUILD_HEADERS "")
list(APPEND BUILD_HEADERS ${ENTITY_HEADERS})
list(APPEND BUILD_HEADERS ${FACTORY_HEADERS})

# Sources
file(GLOB ENTITY_SOURCES RELATIVE ./GameEntity ./*.cpp) # Entity
file(GLOB FACTORY_SOURCES RELATIVE ./Factory ./*.cpp) # Factory
set(BUILD_SOURCES "")
list(APPEND BUILD_SOURCES ${ENTITY_SOURCES})
list(APPEND BUILD_SOURCES ${FACTORY_SOURCES})

include_directories("./GameEntity")

make_is_test_case("${BUILD_HEADERS}" "${BUILD_SOURCES}" ./factory_raw_ptr_with_enum.cpp)
make_is_test_case("${BUILD_HEADERS}" "${BUILD_SOURCES}" ./factory_raw_ptr_with_string.cpp)

