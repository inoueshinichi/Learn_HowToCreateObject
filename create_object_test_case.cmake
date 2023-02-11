cmake_minimum_required(VERSION 3.14.6)

function(make_test_case_create_object TEST_TARGET TERGET_SOURCES)

    # Project
    project(${TEST_TARGET} LANGUAGES CXX VERSION 0.1.0)

    # Header
    include_directories(${CMAKE_CURRENT_SOURCE_DIR})

    # Executable
    add_executable(${TEST_TARGET} ${TERGET_SOURCES})

endfunction()