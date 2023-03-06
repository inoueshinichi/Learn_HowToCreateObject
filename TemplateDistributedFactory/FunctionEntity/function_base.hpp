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
#include <tuple>
#include <type_traits>

/**
 * @brief Type Eraser
 * 非テンプレート基底クラスとテンプレート派生クラスによる
 * 型推論と継承を利用して型情報をカプセル化しつつ基底クラスから
 * 自由にアクセスできる. 結果, ポリモーフィズムを実現できる.
 */

// コンストラクタ引数を保持するクラス
template <typename... Args>
class FunctionBase : public Function
{
protected:
    std::tuple<std::remove_reference_t<Args>...> mArgs;

public:
    FunctionBase(const std::string& type, Args... args) : Function(type), mArgs(args...) {}
    virtual ~FunctionBase() {}

    int NumArgs() const { return sizeof...(Args); }
    const std::tuple<Args...> &GetArgs() const { return mArgs; }

    template <int Index>
    auto GetIndexArg() -> decltype(std::get<Index>(mArgs))
    {
        return std::get<Index>(mArgs);
    }

    const std::string& GetTypeStr() const { return mType; }
};