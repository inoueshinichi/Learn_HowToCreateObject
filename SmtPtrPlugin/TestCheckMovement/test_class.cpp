#include <test_class.hpp>


inline int &TestClass::GetStaticInt()
{
    std::mutex &sMtx = TestClass::GetStaticMutex();
    std::lock_guard<std::mutex> locker(sMtx);

    static int sIntVal = 0;
    return sIntVal;
}

inline std::mutex &TestClass::GetStaticMutex()
{
    static std::mutex sMtx;
    return sMtx;
}

double TestClass::GetDoubleVariable()
{
    int& sVal = TestClass::GetStaticInt();
    return (double)sVal;
}

float TestClass::GetFloatVariable()
{
    return 101.0f;
}
