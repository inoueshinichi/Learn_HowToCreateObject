cmake_minimum_required(VERSION 3.14.6)


macro(is_macro_plugin_configure PLUGIN_IMPL_DIR VER_MAJOR VER_MINOR VER_PATCH)

    # Get Target Name
    get_filename_component(DIRNAME ${PLUGIN_IMPL_DIR} NAME)

    # Target Name
    set(DLL_PLUGIN_TARGET ${DIRNAME}) # ローカルスコープの変数
    # set(DLL_PLUGIN_TARGET ${DIRNAME} PARENT_SCOPE) # 親スコープの変数

    message(STATUS "DLL_PLUGIN_TARGET: ${DLL_PLUGIN_TARGET}")

    # Plugin Version Management
    set(CMAKE_PLUGIN_AUTHOR "Shinichi Inoue")
    set(CMAKE_PLUGIN_EMAIL "inoue.shinichi.1800@gmail.com")
    set(CMAKE_PLUGIN_PLATFORM ${CMAKE_SYSTEM_NAME})
    set(CMAKE_PLUGIN_PLATFORM_VERSION ${CMAKE_SYSTEM_VERSION})
    set(CMAKE_PLUGIN_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
    set(CMAKE_PLUGIN_ARCHITECTURE_ADDRESS_VOID_LENGTH ${CMAKE_SIZEOF_VOID_P})
    set(CMAKE_PLUGIN_GENERATOR ${CMAKE_GENERATOR})
    set(CMAKE_PLUGIN_VERSION_MAJOR ${VER_MAJOR})
    set(CMAKE_PLUGIN_VERSION_MINOR ${VER_MINOR})
    set(CMAKE_PLUGIN_VERSION_PATCH ${VER_PATCH})

    # Project
    project(${DLL_PLUGIN_TARGET} LANGUAGES C CXX VERSION ${CMAKE_PLUGIN_VERSION_MAJOR}.${CMAKE_PLUGIN_VERSION_MINOR}.${CMAKE_PLUGIN_VERSION_PATCH})

    # Library postfix
    set(CMAKE_RELEASE_POSTFIX "")
    set(CMAKE_DEBUG_POSTFIX d)
    set(CMAKE_MINSIZEREL_POSTFIX s)
    set(CMAKE_RELWITHDEBINFO_POSTFIX rd)

    # # Policy
    # if(POLICY CMP0025)
    #     cmake_policy(SET CMP0025 NEW) # AppleClang
    # endif()

    # if(POLICY CMP0115)
    #     cmake_policy(SET CMP0115 NEW) # explicit source symbol
    # endif()

    # Dll
    # https://cmake.org/cmake/help/latest/command/add_library.html
    # This is because CMake expects a SHARED library to always have an associated import library on Windows.
    add_library(${DLL_PLUGIN_TARGET} MODULE) # Moduleは, dlopenやLoadLibraryを使った動的リンク用

    # Dll Version
    if(WIN32)
        configure_file(version.hpp.in version.hpp @ONLY NEWLINE_STYLE CRLF) # Windows
    else()
        configure_file(version.hpp.in version.hpp @ONLY NEWLINE_STYLE LF) # Unix
    endif()
    include_directories(${CMAKE_CURRENT_BINARY_DIR}) # Include version.hpp

    # Plugin Headers
    file(GLOB PLUGIN_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} 
        ./*.hpp
        ./${PLUGIN_IMPL_DIR}/*.hpp
    )

    # Plugin Sources
    file(GLOB PLUGIN_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} 
        ./*.cpp
        ./${PLUGIN_IMPL_DIR}/*.cpp
    )

    # Include Directories
    target_include_directories(${DLL_PLUGIN_TARGET} 
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/${PLUGIN_IMPL_DIR}
    )

    # Include Host Headers
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Host)
    file(GLOB HOST_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} 
        ../Host/*.hpp
    )

    # Target Headers and Sources
    target_sources(${DLL_PLUGIN_TARGET}
        PRIVATE ${PLUGIN_HEADERS}
        PRIVATE ${PLUGIN_SOURCES}
        PRIVATE ${HOST_HEADERS}
    )


    ################# 
    # Configuration #
    #################
    # Definitions
    target_compile_definitions(${DLL_PLUGIN_TARGET} PRIVATE
        # MSVC
        $<$<CXX_COMPILER_ID:MSVC>:WIN32 _WINDOWS _CRT_NONSTDC_NO_DEPRECATE _CRT_SECURE_NO_WARNINGS _USE_MATH_DEFINES>
        $<$<CXX_COMPILER_ID:MSVC>:UNICODE _UNICODE> # UTF-16
        $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:NDEBUG>
        $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:_DEBUG>
        $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:MinSizeRel>>:NDEBUG>
        $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:RelWithDebgInfo>>:_DEBUG>

        # GNU, Clang, AppleClang
        $<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>,$<OR:$<CONFIG:Release>,$<CONFIG:MinSizeRel>>>:NDEBUG>

        # Dll Exporter
        $<$<CONFIG:Release>:PLUGIN_EXPORTS>
        $<$<CONFIG:Debug>:PLUGIN_dbg_EXPORTS>
        $<$<CONFIG:MinSizeRel>:PLUGIN_EXPORTS>
        $<$<CONFIG:RelWithDebgInfo>:PLUGIN_dbg_EXPORTS>
    )

    # Features
    target_compile_features(${DLL_PLUGIN_TARGET} PRIVATE 
        cxx_std_17 # C++17
    )

    # Options
    target_compile_options(${DLL_PLUGIN_TARGET} PRIVATE
        # MSVC
        $<$<CXX_COMPILER_ID:MSVC>: /W4 /GR /EHsc /utf-8 /Zc:__cplusplus /Zc:preprocessor /bigobj> # /WXは警告をエラーと見なす
        $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:/Ob2 /O2>
        $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:/Ob0 /Od /Zi /RTC1>
        $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:MinSizeRel>>:/O1>
        $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:RelWithDebgInfo>>:/Od /Zi /RTC1>

        # GNU, Clang, AppleClang
        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>: -Wall>
        $<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>,$<CONFIG:Release>>: -O3>
        $<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>,$<CONFIG:Debug>>: -O0 -g>
        $<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>,$<CONFIG:MinSizeRel>>: -Os>
        $<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>,$<CONFIG:RelWithDebgInfo>>: -O2 -g>
    )

    # Suppression of Warning
    target_compile_options(${DLL_PLUGIN_TARGET} PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>: /wd"4100" /wd"5054" /wd"4819" /wd"4099">
        $<$<CXX_COMPILER_ID:GNU>: >
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>: >
    )

    # Output Preprocessor(*.ii) & Assembler(*.s)
    if(ON)
        # message(STATUS "Output preprocessed files....")
        target_compile_options(${DLL_PLUGIN_TARGET} PRIVATE
            $<$<CXX_COMPILER_ID:MSVC>: /FA>
            $<$<CXX_COMPILER_ID:GNU>: -save-temps=obj>
            $<$<CXX_COMPILER_ID:Clang>: -save-temps=obj>
            $<$<CXX_COMPILER_ID:AppleClang>: -save-temps=obj>
        )
    endif()

    # OpenMP
    if(ON)
        if(NOT APPLE)
            find_package(OpenMP REQUIRED)
            target_compile_options(${DLL_PLUGIN_TARGET} PRIVATE
                $<$<CXX_COMPILER_ID:MSVC>: /openmp>
                $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>: -fopenmp>
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
                $<$<CXX_COMPILER_ID:AppleClang>: -Xpreprocessor -fopenmp> # AppleClang with XCode
            )
        endif()
        target_include_directories(${DLL_PLUGIN_TARGET} PRIVATE ${OpenMP_INCLUDE_DIRS})
        target_link_directories(${DLL_PLUGIN_TARGET} PRIVATE ${OpenMP_LIBRARY})
        target_link_libraries(${DLL_PLUGIN_TARGET} PRIVATE ${OpenMP_LIBRARIES})
    endif()

    ##############
    # Properties #
    ##############
    if(UNIX OR APPLE)
        # Makefileの詳細情報を出力
        set(CMAKE_VERBOSE_MAKEFILE ON)
    endif()

    # MSVC Runtime Libarary
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

    # # macOS CFBundle, Framework, App
    # if(APPLE)
    #     set_target_properties(${DLL_PLUGIN_TARGET} PROPERTIES
    #         # $<$<STREQUAL:$<TARGET_PROPERTY:${DLL_PLUGIN_TARGET}, TYPE>, STATIC_LIBRARY>:~>
    #         # $<$<STREQUAL:$<TARGET_PROPERTY:${DLL_PLUGIN_TARGET}, TYPE>, SHARED_LIBRARY>:~>
    #         # $<$<STREQUAL:$<TARGET_PROPERTY:${DLL_PLUGIN_TARGET}, TYPE>, MODULE_LIBRARY>:~>
    #         BUNDLE TRUE
    #     )
    # endif()

    #################
    # Debug utility #
    #################
    # message(STATUS "Output preprocessed files....")
    target_compile_options(${DLL_PLUGIN_TARGET} PRIVATE
        # $<$<CXX_COMPILER_ID:MSVC>:>
        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>: -Wshadow> # シャドウイングの警告
        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>: -Wconversion> # 暗黙の型変換の警告
        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>: -Wfloat-equal> # 実数値の等号比較を警告
        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>: -ftrapv> # オーバーフロー検出(検出すると即座にプログラム停止)
        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-fstack-protector-all> # スタック領域の範囲外書き込みの検出
        # $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>: -fsanitize=address> # 不正メモリ操作の検出
        # $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>: -fsanitize=undefined> # 未定義動作の検出
        # $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>: -fno-omit-frame-pointer> # 有効なスタックトレースの抽出
        # $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>: -ggdb> # GDB向けのデバッグ情報を埋め込む
        # $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>: -fverbose-asm> # アセンブリコードにソースコード情報を埋め込む
        # $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:>
        # $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:>
        # $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:>
    )
        
endmacro()


    

