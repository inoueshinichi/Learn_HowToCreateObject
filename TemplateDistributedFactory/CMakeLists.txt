cmake_minimum_required(VERSION 3.14.6)

include(../is_test_case.cmake)

#Headers
file(GLOB SINGLETON_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} SingletonManager/*.hpp) # Singleton
file(GLOB OPERAND_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FunctionOperand/*.hpp) # Operand
file(GLOB ENTITY_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FunctionEntity/*.hpp) # Entity
file(GLOB GENERIC_ENTITY_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FunctionEntity/Generic/*.hpp) # Generic
file(GLOB FACTORY_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} DistributedFactory/*.hpp) # Factory

set(BUILD_HEADERS "")
list(APPEND BUILD_HEADERS ${SINGLETON_HEADERS})
list(APPEND BUILD_HEADERS ${OPERAND_HEADERS})
list(APPEND BUILD_HEADERS ${ENTITY_HEADERS})
list(APPEND BUILD_HEADERS ${GENERIC_ENTITY_HEADERS})
list(APPEND BUILD_HEADERS ${FACTORY_HEADERS})


# message(STATUS "BUILD_HEADERS: ${BUILD_HEADERS}")

# Sources
file(GLOB SINGLETON_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} SingletonManager/*.cpp) # Singleton
file(GLOB OPERAND_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FunctionOperand/*.cpp) # Operand
file(GLOB ENTITY_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FunctionEntity/*.cpp) # Entity
file(GLOB GENERIC_ENTITY_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FunctionEntity/Generic/*.cpp) # Generic
file(GLOB FACTORY_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} DistributedFactory/*.cpp) # Factory

set(BUILD_SOURCES "")
list(APPEND BUILD_SOURCES ${SINGLETON_SOURCES})
list(APPEND BUILD_SOURCES ${OPERAND_SOURCES})
list(APPEND BUILD_SOURCES ${ENTITY_SOURCES})
list(APPEND BUILD_SOURCES ${GENERIC_ENTITY_SOURCES})
list(APPEND BUILD_SOURCES ${FACTORY_SOURCES})

# message(STATUS "BUILD_SOURCES: ${BUILD_SOURCES}")


include_directories(./SingletonManager)
include_directories(./FunctionOperand)
include_directories(./FunctionEntity)
include_directories(./FunctionEntity/Generic)
include_directories(./DistributedFactory)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}) # system_init_for_register.hpp

list(APPEND BUILD_HEADERS ./system_init_for_register.hpp)

make_is_test_case("${BUILD_HEADERS}" "${BUILD_SOURCES}" ./template_distributed_factory.cpp)