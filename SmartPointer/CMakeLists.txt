cmake_minimum_required(VERSION 3.14.6)

include(../is_test_case.cmake)

make_is_test_case(CO_SP_UNIQUE_PTR ON OFF "" "./sp_unique_ptr.cpp")
make_is_test_case(CO_SP_SHARED_PTR ON OFF "" "./sp_shared_ptr.cpp")
make_is_test_case(CO_SP_VOID_SHARED_PTR ON OFF "" "./sp_void_shared_ptr.cpp")
make_is_test_case(CO_SP_ENABLE_SHARED_FROM_THIS ON OFF "" "./sp_enable_shared_from_this.cpp")
make_is_test_case(CO_SP_WEAK_PTR ON OFF "" "./sp_weak_ptr.cpp")