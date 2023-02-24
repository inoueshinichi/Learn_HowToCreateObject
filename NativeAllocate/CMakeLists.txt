cmake_minimum_required(VERSION 3.14.6)

include(../is_test_case.cmake)

make_is_test_case(CO_C_STYLE_MALLOC_FREE ON OFF "" "./c_style_malloc_free.cpp")
make_is_test_case(CO_C_STYLE_CALLOC_FREE ON OFF "" "./c_style_calloc_free.cpp")
make_is_test_case(CO_C_STYLE_REALLOC_FREE ON OFF "" "./c_style_realloc_free.cpp")
make_is_test_case(CO_CXX_STYLE_NEW_DELETE ON OFF "" "./cxx_style_new_delete.cpp")
make_is_test_case(CO_CXX_STYLE_PLACEMENT_NEW_FOR_CLASS_ARRAY ON OFF "" "./cxx_style_placement_new_for_class_array.cpp")
make_is_test_case(CO_CXX_STYLE_STL_ALLOCATOR_FOR_CLASS_ARRAY ON OFF "" "./cxx_style_stl_allocator_for_class_array.cpp")
make_is_test_case(CO_CXX_STYLE_STL_ALLOCATOR_TRAITS ON OFF "" "./cxx_style_stl_allocator_traits.cpp")