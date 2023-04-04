#include <test_class.hpp>


// inline int &TestClass::GetStaticInt()
// {
//     std::mutex &sMtx = TestClass::GetStaticMutex();
//     std::lock_guard<std::mutex> locker(sMtx);

//     static int sIntVal = 0;
//     return sIntVal;
// }

// inline std::mutex &TestClass::GetStaticMutex()
// {
//     static std::mutex sMtx;
//     return sMtx;
// }