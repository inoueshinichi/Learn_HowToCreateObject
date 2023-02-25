cmake_minimum_required(VERSION 3.14.6)

include(../is_test_case.cmake)

make_is_test_case("" "" ./c_style_calloc_free.cpp)
make_is_test_case("" "" ./c_style_malloc_free.cpp)
make_is_test_case("" "" ./c_style_realloc_free.cpp)
make_is_test_case("" "" ./cxx_style_new_delete.cpp)
make_is_test_case("" "" ./cxx_style_placement_new_for_class_array.cpp)
make_is_test_case("" "" ./cxx_style_stl_allocator_for_class_array.cpp)
make_is_test_case("" "" ./cxx_style_stl_allocator_traits.cpp)