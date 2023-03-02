/**
 * @file function_base.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <function.hpp>

// コンストラクタ引数を保持するクラス
template <typename... Args>
class FunctionBase : public Function
{
protected:
    std::tuple<std::remove_reference_t<Args>...> mArgs;

public:
    FunctionBase(Args... args) : Function(), mArgs(args...) {}
    virtual ~FunctionBase() {}

    int NumArgs() const { return sizeof...(Args); }
    const std::tuple<Args...> &GetArgs() const { return mArgs; }

    template <int Index>
    auto GetIndexArg() -> decltype(mArgs.get<Index>(mArgs))
    {
        return mArgs.get<Index>();
    }
};