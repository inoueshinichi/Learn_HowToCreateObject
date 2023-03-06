/**
 * @file garbage_collector.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <functional>
#include <vector>

/**
 * @brief 確保されたCPUメモリの解放
 * @note 参考: https://qiita.com/43x2/items/59b3e39256be0572cc5d
 * @note saberGC -- https://github.com/43x2/saberGC
 * @note Naïveなマーク・アンド・スイープによるガベージコレクション(GC).
 * @note ポインタと非ポインタを区別する, いわゆる正確な(exact/precise)GC.
 * @note アロケータとデアロケータのカスタマイズが可能.
 * @note オブジェクトはshared_ptrやunique_ptrライクなインターフェースを持つ.
 * @note 静的記憶域を使わず, 必要に応じて複数のGCインスタンスを使い分けることが可能.
 */
class GarbageCollector final
{
public:
    template <typename T> class GCObject;
    using allocator_type = std::function<void(const std::size_t, const std::size_t)>;
    using deallocator_type = std::function<void(void*)>;

    using CollectorType = std::function<void()>;

    ~GarbageCollector() = default;
    GarbageCollector(const GarbageCollector&) = delete;
    GarbageCollector& operator=(const GarbageCollector&) = delete;
    GarbageCollector(GarbageCollector&&) = default;
    GarbageCollector& operator=(GarbageCollector&&) = default;

    GarbageCollector(allocator_type allocator, deallocator_type deallocator);

    // GCObjectを構築
    template <typename T, typename... Args>
    GCObject<T> NewObject(Args&&... args);
   
    // 利用されていないオブジェクトを明示的に破棄する
    void Collection();

    void Register(CollectorType func);

private:
    friend class SingletonManager;
    GarbageCollector();

    std::vector<CollectorType> mCollectors;
};