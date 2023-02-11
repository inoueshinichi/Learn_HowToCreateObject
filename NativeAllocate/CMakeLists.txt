cmake_minimum_required(VERSION 3.14.6)

include(../create_object_test_case.cmake)

make_test_case_create_object(CO_C_STYLE_MALLOC_FREE ./c_style_malloc_free.cpp)
make_test_case_create_object(CO_C_STYLE_CALLOC_FREE ./c_style_calloc_free.cpp)
make_test_case_create_object(CO_C_STYLE_REALLOC_FREE ./c_style_realloc_free.cpp)