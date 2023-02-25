cmake_minimum_required(VERSION 3.14.6)

include(../is_test_case.cmake)

make_is_test_case("" "" ./sp_unique_ptr.cpp)
make_is_test_case("" "" ./sp_shared_ptr.cpp)
make_is_test_case("" "" ./sp_void_shared_ptr.cpp)
make_is_test_case("" "" ./sp_enable_shared_from_this.cpp)
make_is_test_case("" "" ./sp_weak_ptr.cpp)