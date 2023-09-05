cmake_minimum_required(VERSION 3.14.6)

include(../is_test_case.cmake)

#Headers
file(GLOB MEMORY_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ./Memroy/*.hpp) # Memory
file(GLOB POOL_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ./PoolAllocator/*.hpp) # PoolAllocator
file(GLOB DELEGATE_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ./Delegate/*.hpp) # Delegate

set(BUILD_HEADERS "")
list(APPEND BUILD_HEADERS ${MEMORY_HEADERS})
list(APPEND BUILD_HEADERS ${POOL_HEADERS})
list(APPEND BUILD_HEADERS ${DELEGATE_HEADERS})

include_directories("./Memory")
include_directories("./PoolAllocator")
include_directories("./Delegate")

make_is_test_case("${BUILD_HEADERS}" "${BUILD_SOURCES}" ./memory_pool.cpp)