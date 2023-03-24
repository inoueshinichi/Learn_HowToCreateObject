cmake_minimum_required(VERSION 3.14.6)

# Get Target Name
get_filename_component(DIRNAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# Target Name
set(DLL_PLUGIN_TARGET ${DIRNAME}) # ローカルスコープの変数
set(DLL_PLUGIN_TARGET ${DIRNAME} PARENT_SCOPE) # 親スコープの変数

message(STATUS "DLL_PLUGIN_TARGET: ${DLL_PLUGIN_TARGET}")

# Version Management
set(CMAKE_PLUGIN_VERSION_MAJOR 0)
set(CMAKE_PLUGIN_VERSION_MINOR 1)
set(CMAKE_PLUGIN_VERSION_PATCH 0)
set(CMAKE_PLUGIN_AUTHOR "Shinichi Inoue")
set(CMAKE_PLUGIN_EMAIL "inoue.shinichi.1800@gmail.com")

# 64bit or 32bit Architecture
if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
    message(STATUS "64-bit arch in Windows.")
else()
    message(STATUS "32-bit arch in Windows.")
endif()

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

# MSVC Linker Path
message(STATUS "MSVC Linker Path: ${CMAKE_LINKER}") 

# Project
project(${DLL_PLUGIN_TARGET} LANGUAGES C CXX VERSION ${CMAKE_PLUGIN_VERSION_MAJOR}.${CMAKE_PLUGIN_VERSION_MINOR}.${CMAKE_PLUGIN_VERSION_PATCH})

# Library postfix
set(CMAKE_RELEASE_POSTFIX "")
set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_MINSIZEREL_POSTFIX s)
set(CMAKE_RELWITHDEBINFO_POSTFIX rd)

# Dll
# https://cmake.org/cmake/help/latest/command/add_library.html
# This is because CMake expects a SHARED library to always have an associated import library on Windows.
add_library(${DLL_PLUGIN_TARGET} MODULE) # Moduleは, dlopenやLoadLibraryを使った動的リンク用

# Dll Version
configure_file(version.hpp.in version.hpp @ONLY NEWLINE_STYLE CRLF) # Windows
include_directories(${CMAKE_CURRENT_BINARY_DIR}) # Include version.hpp

# Plugin Headers
file(GLOB WIN_PLUGIN_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ./*.hpp) # Win

# Plugin Sources
file(GLOB WIN_PLUGIN_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ./*.cpp) # Win

# Include Directories
target_include_directories(${DLL_PLUGIN_TARGET} 
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

# Target Headers and Sources
target_sources(${DLL_PLUGIN_TARGET}
    PRIVATE ${WIN_PLUGIN_HEADERS}
    PRIVATE ${WIN_PLUGIN_SOURCES}
)

# Win API libararies
# Library Directories
# target_link_directories(${DLL_PLUGIN_TARGET} 
#     PRIVATE
# )

# Libraries
# target_link_libraries(${DLL_PLUGIN_TARGET} 
#     PRIVATE
# )


# MSVC Runtime Libarary Option
 if(MSVC)
    if(OFF)
        set_target_properties(${DLL_PLUGIN_TARGET} PROPERTIES
            MSVC_RUNTIME_LIBRARY MultiThreaded$<CONFIG:Release>>:> # -MT (MultiThreaded)
            MSVC_RUNTIME_LIBRARY MultiThreaded$<CONFIG:Debug>>:Debug> # -MTd (MultiThreadedDebug)
        )
    else()
        set_target_properties(${DLL_PLUGIN_TARGET} PROPERTIES
                MSVC_RUNTIME_LIBRARY MultiThreaded$<CONFIG:Release>>:DLL> # -MD (MultiThreadedDLL)
                MSVC_RUNTIME_LIBRARY MultiThreaded$<CONFIG:Debug>>:DebugDLL> # -MDd (MultiThreadedDebugDLL)
        )
    endif()
endif()

################# 
# Configuration #
#################
# Definitions
target_compile_definitions(${DLL_PLUGIN_TARGET} PRIVATE
    # MSVC
    $<$<CXX_COMPILER_ID:MSVC>:WIN32 _WINDOWS _CRT_NONSTDC_NO_DEPRECATE _CRT_SECURE_NO_WARNINGS _USE_MATH_DEFINES>
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:NDEBUG>
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:_DEBUG>
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:MinSizeRel>>:NDEBUG>
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:RelWithDebgInfo>>:_DEBUG>
)

# Features
target_compile_features(${DLL_PLUGIN_TARGET} PRIVATE 
    cxx_std_17 # C++17
)


 # Options
target_compile_options(${DLL_PLUGIN_TARGET} PRIVATE
    # MSVC
    $<$<CXX_COMPILER_ID:MSVC>: /W4 /wd"4100" /wd"5054" /GR /EHsc /utf-8 /Zc:__cplusplus /Zc:preprocessor /bigobj> # /WXは警告をエラーと見なす
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:/Ob2 /O2>
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:/Ob0 /Od /Zi /RTC1>
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:MinSizeRel>>:/O1>
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:RelWithDebgInfo>>:/Od /Zi /RTC1>
)


 # Output Preprocessor(*.ii) & Assembler(*.s)
if(ON)
    # message(STATUS "Output preprocessed files....")
    target_compile_options(${DLL_PLUGIN_TARGET} PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/FA>
        $<$<CXX_COMPILER_ID:GNU>:-save-temps=obj>
        $<$<CXX_COMPILER_ID:Clang>:-save-temps=obj>
        $<$<CXX_COMPILER_ID:AppleClang>:-save-temps=obj>
    )
endif()

 # OpenMP
if(ON)
    if(NOT APPLE)
        find_package(OpenMP REQUIRED)
        target_compile_options(${DLL_PLUGIN_TARGET} PRIVATE
            $<$<CXX_COMPILER_ID:MSVC>:/openmp>
            $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-fopenmp>
        )
    else()
        # macOSのOpenMPはGeneratorExpressionに対応していないバグがある.
        # macOSのAppleClangの場合CMakeのfildpakageスクリプトの動作にバグがありそうなので, Homebrewでインストールして直接指定.
        # https://zv-louis.hatenablog.com/entry/2018/12/23/141327
        execute_process(COMMAND brew --prefix libomp OUTPUT_VARIABLE OPENMP_HOME OUTPUT_STRIP_TRAILING_WHITESPACE)
        if(${OPENMP_HOME} STREQUAL "")
            message(FATAL_ERROR "Not found OpenMP. Please `brew install libomp`")
        endif()
        # message(STATUS "OpenMP root path: ${OPENMP_HOME}") # 非表示
        set(OpenMP_INCLUDE_DIRS "${OpenMP_HOME}/include/")
        set(OpenMP_LIBRARY "${OpenMP_HOME}/lib/")
        set(OpenMP_CXX_LIB_NAMES "libomp")
        target_compile_options(${DLL_PLUGIN_TARGET} PRIVATE
            $<$<CXX_COMPILER_ID:AppleClang>:-Xpreprocessor -fopenmp> # AppleClang with XCode
        )
    endif()
    target_include_directories(${DLL_PLUGIN_TARGET} PRIVATE ${OpenMP_INCLUDE_DIRS})
    target_link_directories(${DLL_PLUGIN_TARGET} PRIVATE ${OpenMP_LIBRARY})
    target_link_libraries(${DLL_PLUGIN_TARGET} PRIVATE ${OpenMP_LIBRARIES})
endif()