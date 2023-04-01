cmake_minimum_required(VERSION 3.14.6)

# CMake version
message(STATUS "CMake Verion: ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}.${CMAKE_TWEAK_VERSION}")

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
# Host PC
###############################################################################

# Host
message(STATUS "Host OS: ${CMAKE_HOST_SYSTEM_NAME}")
message(STATUS "Host OS Version: ${CMAKE_HOST_SYSTEM_VERSION}")
message(STATUS "Host System Processor: ${CMAKE_HOST_SYSTEM_PROCESSOR}")

# Architecture Name
set(HOST_ARCH "")
if (UNIX)
    execute_process(COMMAND uname -m OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE HOST_ARCH) # OUTPUT_STRIP_TRAILING_WHITESPACE : 改行を省く
    if(${HOST_ARCH} STREQUAL "")
        execute_process(COMMAND uname -p OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE HOST_ARCH)
    endif()
    if(${HOST_ARCH} STREQUAL "")
        message(FATAL_ERROR "Not found architecture symbol in Unix.")
    endif()
else() # Windows
    set(HOST_ARCH $ENV{PROCESSOR_ARCHITECTURE})
    if(${HOST_ARCH} STREQUAL "")
        message(FATAL_ERROR "Not found architecture symbol in Windows.")
    endif()
endif()

message(STATUS "Host Architecture: ${HOST_ARCH}")

# # 64bit or 32bit
# set(HOST_ARCH_BITS "")
# if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
#     message(STATUS "64-bit arch.")
#     set(HOST_ARCH_BITS "64")
# else()
#     message(STATUS "32-bit arch.")
#     set(HOST_ARCH_BITS "32")
# endif()





# Platform
if(WIN32) # Windows
   
    # Windows SDK (OS Version)
    message(STATUS "Windows SDK Version (system-default): ${CMAKE_SYSTEM_VERSION}")

    # Windows SDK Path
    message(STATUS "Windows SDK Path; UniversalCRTSdkDir: $ENV{UniversalCRTSdkDir}")

    # Windows SDK General Include Path
    message(STATUS "Windows SDK General Include Path: $ENV{UniversalCRTSdkDir}\\Include\\ucrt")

    # Windows SDK Specified Path
    message(STATUS "Windows SDK Specified Path above win 10 (maybe empty): ${CMAKE_WINDOWS_KITS_10_DIR}")

    # CMake Target Platform Version (offered by MSVC)
    message(STATUS "CMake Target Platform Version (offered by MSVC): ${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}")

    # Windows SDK Specified Include Path
    message(STATUS "Windows SDK Specified(${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}) Include Path: $ENV{UniversalCRTSdkDir}\\Include\\${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}\\ucrt")

   

elseif(UNIX AND NOT APPLE) # Linux
    message(STATUS "Platform: Linux ${CMAKE_SYSTEM_VERSION}")
elseif(APPLE) # Darwin(macOS)
    message(STATUS "Platform: macOS ${CMAKE_SYSTEM_VERSION}")
else()
    message(FATAL_ERROR "Platform: Not match platforms [Windows, macOS, Linux]. Given platform is ${CMAKE_SYSTEM_NAME}")
endif()

if(MSVC)
    message(STATUS "CMAKE_VS_SDK_INCLUDE_DIRECTORIES: ${CMAKE_VS_SDK_INCLUDE_DIRECTORIES}")
    message(STATUS "CMAKE_VS_SDK_LIBRARY_DIRECTORIES: ${CMAKE_VS_SDK_LIBRARY_DIRECTORIES}")
    message(STATUS "CMAKE_VS_SDK_REFERENCE_DIRECTORIES: ${CMAKE_VS_SDK_REFERENCE_DIRECTORIES}")
    message(STATUS "CMAKE_VS_SDK_SOURCE_DIRECTORIES: ${CMAKE_VS_SDK_SOURCE_DIRECTORIES}")
    message(STATUS "CMAKE_VS_SDK_EXECUTABLE_DIRECTORIES: ${CMAKE_VS_SDK_EXECUTABLE_DIRECTORIES}")
    message(STATUS "CMAKE_VS_SDK_EXCLUDE_DIRECTORIES: ${CMAKE_VS_SDK_EXCLUDE_DIRECTORIES}")
    # MSVC Linker Path
    message(STATUS "MSVC Linker Path: ${CMAKE_LINKER}") 
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
set(HOST_GENERATOR ${CMAKE_GENERATOR})
message(STATUS "Generator: ${CMAKE_GENERATOR}")


###############################################################################
# Target PC
###############################################################################
# Target
message(STATUS "Target OS: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Target OS Version: ${CMAKE_SYSTEM_VERSION}")
message(STATUS "Target System Processor: ${CMAKE_SYSTEM_PROCESSOR}")

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
add_subdirectory(SimpleRawPtrFactory)
add_subdirectory(SmtPtrFactory)
add_subdirectory(RTTIClassMemAddIDFactory)
add_subdirectory(TemplateDistributedFactory)
add_subdirectory(SmtPtrPlugin)