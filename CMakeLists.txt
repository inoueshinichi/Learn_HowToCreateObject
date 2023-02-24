cmake_minimum_required(VERSION 3.14.6)

# CMake version
message(STATUS "CMake Ver: ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}.${CMAKE_TWEAK_VERSION}")

###############################################################################
# Policy
###############################################################################

# <= CMake3.*.*でAppleClangをコンパイラIDとして認識させる
# Report AppleClang separately from Clang. Their version numbers are different.
# https://cmake.org/cmake/help/v3.0/policy/CMP0025.html
if(POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW)
endif()

if (POLICY CMP0115)
 cmake_policy(SET CMP0115 NEW) # explicit source symbol
endif()

###############################################################################
# Device Check
###############################################################################
# Platform & Processor
set(HOST_ARCH "")
if (UNIX)
    execute_process(COMMAND uname -m OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE HOST_ARCH) # OUTPUT_STRIP_TRAILING_WHITESPACE : 改行を省く
    if(${HOST_ARCH} STREQUAL "")
        execute_process(COMMAND uname -p OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE HOST_ARCH)
    endif()
    if(${HOST_ARCH} STREQUAL "")
        message(FATAL_ERROR "Not found architecture symbol in Unix.")
    endif()
endif()

if(WIN32)
    message(STATUS "Platform: Windows ${CMAKE_SYSTEM_VERSION}")
    set(HOST_ARCH $ENV{PROCESSOR_ARCHITECTURE})
    if(${HOST_ARCH} STREQUAL "")
        message(FATAL_ERROR "Not found architecture symbol in Windows.")
    endif()
elseif(UNIX AND NOT APPLE)
    message(STATUS "Platform: Linux ${CMAKE_SYSTEM_VERSION}")
elseif(APPLE)
    message(STATUS "Platform: macOS ${CMAKE_SYSTEM_VERSION}")
else()
    message(FATAL_ERROR "Platform: Not match platforms [Windows, macOS, Linux]. Given platform is ${CMAKE_SYSTEM_NAME}")
endif()

if(APPLE)
    set(OSX_ARCHITECTURE "")
    if(${HOST_ARCH} MATCHES "arm64")
        list(APPEND OSX_ARCHITECTURE "AppleSilicon")
    elseif(${HOST_ARCH} MATCHES "x86_64")
        list(APPEND OSX_ARCHITECTURE "Intel(Rosetta2)")
    else()
        message(FATAL_ERROR "Not match architecture. Arch must arm64 or x86_64 in macOS. Given is ${ARCH}")
    endif()
    message(STATUS "Your macOS cpu architecture is ${HOST_ARCH} ${OSX_ARCHITECTURE}!")
endif()
message(STATUS "Host arch: ${HOST_ARCH}")
message(STATUS "CMAKE_SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}") # コンパイル対象のプロセッサ
message(STATUS "CMAKE_HOST_SYSTEM_PROCESSOR: ${CMAKE_HOST_SYSTEM_PROCESSOR}") # 実行しているプロセッサ

# Generator
# https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html
message(STATUS "Generator: ${CMAKE_GENERATOR}")

###############################################################################
# Top-level Project Name
###############################################################################
project(HowToCreateObject LANGUAGES CXX VERSION 0.1.0)

###############################################################################
# Subdirectory
###############################################################################
add_subdirectory(NativeAllocate)
add_subdirectory(SmartPointer)
add_subdirectory(InterfacePattern)
add_subdirectory(RawPtrEnumFactory)