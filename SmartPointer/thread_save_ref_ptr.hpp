/**
 * @file thread_safe_ref_ptr.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

#include <cstddef>
#include <cstdint>
// #include <mutex>
#include <atomic>

namespace internal
{
    class RefCountedThreadSafeBase
    {
    public:
        RefCountedThreadSafeBase(const RefCountedThreadSafeBase&) = delete;
        RefCountedThreadSafeBase &operator=(const RefCountedThreadSafeBase&) = delete;

        void AddRef() const
        {
#ifndef NDEBUG
    // check !mAdaptionRequired, !mDestructionStarted
#endif
            mRefCount.fetch_add(1u, std::memory_order_relaxed);
        }

        bool HasOneRef() const
        {
            return mRefCount.load(std::memory_order_acquire) == 1u;
        }

        void AssertHasOneRef() const { /*check HasOneRef();*/ }

    protected:
        RefCountedThreadSafeBase();
        /*virtual*/ ~RefCountedThreadSafeBase();


        bool Release() const
        {
#ifndef NDEBUG
            // check !mAdaptionRequired, !mDestructionStarted
#endif
            // check mRefCount.load(std::memory_order_acuire) != 0u

            /**
             * @brief 
             * @todo (vt1): We could add the following:
             *   if (mRefCount.load(std::memory_order_relaxed) == 1u)
             *   {
             * #ifndef NDEBUG
             *    mDestructionStarted = true;
             * #endif
             *      return true;
             *   }
             * this would be correct. On ARM(an Nexus 4), in *single-threaded* tests,
             * this seems to make the destruction case marginally faster (barely
             * measurable), and while the non-destruction case remains about the same
             * (possibly marginally slower, but my measurements are'nt good enough to
             * have any condition in that). I should try multithreaded/multicore tests.
             */
            if (mRefCount.fetch_sub(1u, std::memory_order_release) == 1u)
            {
                std::atomic_thread_fence(std::memory_order_acquire);
#ifndef NDEBUG
                mDestructionRequired = true;
#endif
                return true;
            }

            return false;
        }
#ifndef NDEBUG
        void Adopt()
        {
            // check mAdoptionRequired
            mAdoptionRequired = false;
        }
#endif
    private:
#ifndef NDEBUG
        mutable bool mAdoptionRequired;
        mutable bool mDestructionRequired;
#endif
        mutable std::atomic_uint_fast32_t mRefCount;
    };

    inline RefCountedThreadSafeBase::RefCountedThreadSafeBase()
        : mRefCount(1u),
#ifndef NDEBUG
        mAdoptionRequired(true),
        mDestructionRequired(false)
#endif
    {}

    inline RefCountedThreadSafeBase::~RefCountedThreadSafeBase()
    {
#ifndef NDEBUG
        // check !mAdoptionRequired
        // check !mDestructionRequired
#endif
    }
}

template <typename U> class RefPtr;

template <typename T>
class RefCountedThreadSafe : public internal::RefCountedThreadSafeBase
{
public:
    RefCountedThreadSafe(const RefCountedThreadSafe&) = delete;
    RefCountedThreadSafe& operator=(const RefCountedThreadSafe&) = delete;

    void Release() const
    {
        if (internal::RefCountedThreadSafeBase::Release())
        {
            delete static_cast<const T*>(this);
        }
    }

protected:
    RefCountedThreadSafe() {}
    ~RefCountedThreadSafe() {}

private:
#ifndef NDEBUG
        template <typename U>
        friend RefPtr<U> AdoptRef(U*);

        /**
         * @brief 最初の参照でAdaptTagをつける
         *  Marks the initial reference (assumed on cunstruction) as adopted.
         *  This is only required for Debug builds (when |NDEBUG| is not defined.)
         * @todo (vtl): Should this really be private ? This makes manual ref-counting
         *  and also writing one's own ref pointer class impossible.
         */ 
        void Adopt() { internal::RefCountedThreadSafeBase::Adopt(); }
#endif // !NDEBUG
};


// struct SharedDeleterBase
// {
//     SharedDeleterBase() {}
//     virtual ~SharedDeleterBase() {}
//     virtual void Destroy() = 0;
// };

// template <typename T, typename DELETER>
// struct SharedDeleter : public SharedDeleterBase
// {
//     T* mPtr;
//     DELETER mDeleter;
//     SharedDeleter(T* ptr, DELETER deleter) : mPtr(ptr), mDeleter(deleter) {}
//     ~SharedDeleter() {}

//     virtual void Destroy() override
//     {
//         mDeleter(mPtr);
//     }
// };

template <typename T>
class RefPtr final
{
public:
    RefPtr() : mPtr(nullptr)/*,*mRefCount(nullptr),*/ /*mDeleterBase(nullptr)*/ {}
    RefPtr(std::nullptr_t) : mPtr(nullptr)/*, mRefCount(nullptr),*/ /*mDeleterBase(nullptr)*/ {}

    explicit RefPtr(T *ptr)
        : mPtr(ptr), /*mRefCount(new std::size_t(0)),*/ /*mDeleterBase(nullptr)*/
    {
       //AddRef();
       if (mPtr)
       {
          mPtr->AddRef();
       }
    }

    template <typename DELETER>
    RefPtr(T *ptr, DELETER deleter) : mPtr(ptr) /*, mRefCount(new std::size_t(0))*/
    {
        //AddRef();
        //mDeleterBase = new SharedDeleter<T, DELETER>(ptr, deleter);
        if (mPtr)
        {
            mPtr->AddRef();
        }
    }

    template <typename U>
    RefPtr(const RefPtr<U>& r)
        : mPtr(r.mPtr)
    {
        if (mPtr)
        {
            mPtr->AddRef();
        }
    }

    ~RefPtr()
    {
        //Release();
        if (mPtr)
        {
            mPtr->Release();
        }
    }

    T* operator->() const { return mPtr; }
    T& operator*() { return *mPtr; }
    T* Get() const { return mPtr; }

    RefPtr(const RefPtr<T>& other)
        : mPtr(other.mPtr)
    {
        //Release();
        //mPtr = other.mPtr;
        //mRefCount = other.mRefCount;
        //AddRef();
        if (mPtr)
        {
            mPtr->AddRef();
        }
    }

    RefPtr<T>& operator=(const RefPtr<T>& other)
    {
        // if (&other == this)
        // {
        //     return *this;
        // }

        //Release();
        //mPtr = other.mPtr;
        //mRefCount = other.mRefCount;
        //AddRef();

        if (mPtr == other.mPtr)
        {
            return *this;
        }

        if (other.mPtr)
        {
            other.mPtr->AddRef();
        }
        T* oldPtr = mPtr;
        mPtr = other.mPtr;
        if (oldPtr)
        {
            oldPtr->Release();
        }
        return *this;
    }

    template <typename U>
    RefPtr<T>& operator=(const RefPtr<U>& other)
    {
        if (reinterpret_cast<T*>(other.ptr) == mPtr)
        {
            return *this;
        }

        if (other.mPtr)
        {
            other.mPtr->AddRef();
        }
        T* oldPtr = mPtr;
        mPtr = other.mPtr;
        if (oldPtr)
        {
            oldPtr->Release();
        }

        return *this;
    }

    RefPtr(RefPtr<T> &&other)
        : mPtr(other.mPtr), /*mRefCount(other.mRefCount),*/ /*mDeleterBase(other.mDeleterBase)*/
    {
        //other.mPtr = nullptr;
        //other.mRefCount = nullptr;
        //other.mDeleterBase = nullptr;
        other.mPtr = nullptr;
    }

    template <typename U>
    RefPtr(RefPtr<U> &&other)
        : mPtr(other.mPtr)
    {
        other.mPtr = nullptr;
    }

    RefPtr<T> &operator=(RefPtr<T> &&other)
    {
        //Release();
        //mPtr = other.mPtr;
        //mRefCount = other.mRefCount;
        //mDeleterBase = other.mDeleterBase;
        //other.mPtr = nullptr;
        //other.mRefCount = nullptr;
        //other.mDeleterBase = nullptr;

        RefPtr<T>(std::move(other)).Swap(*this);
        return *this;
    }

    template <typename U>
    RefPtr<T> &operator=(RefPtr<U>&& other)
    {
        RefPtr<T>(std::move(other)).Swap(*this);
    }

    void Swap(RefPtr<T>& other)
    {
        T* p = mPtr;
        mPtr = other.mPtr;
        other.mPtr = p;
    }

    // 中身を共有したスマートポインタの生成
    RefPtr<T> Clone() const { return *this; }

    // if文用
    explicit operator bool() const { return !!mPtr; }

    // 他の型Uとの比較(主に継承関係のある親子スマートポインタ同士の比較)
    template <typename U>
    bool operator==(const RefPtr<U>& rhs) const
    {
        return mPtr == rhs.mPtr;
    }

    template <typename U>
    bool operator!=(const RefPtr<U>& rhs) const
    {
        return !operator==(rhs);
    }

    template <typename U>
    bool operator<(const RefPtr<U>& rhs) const
    {
        return mPtr < rhs.mPtr;
    }

private:
    // void AddRef()
    // {
    //     std::lock_guard<std::mutex> locker(mMtx);
    //     if (mRefCount)
    //     {
    //         ++*mRefCount;
    //     }
    // }

    // void Release()
    // {
    //     std::lock_guard<std::mutex> locker(mMtx);
    //     if (mRefCount == nullptr)
    //     {
    //         return;
    //     }

    //     --*mRefCount;
    //     if (*mRefCount == 0)
    //     {
    //         // カスタムデリーターが定義されていた場合
    //         if (mDeleterBase)
    //         {
    //             mDeleterBase->Destroy(); // 解法はカスタムデリータに任せる
    //             delete mRefCount;
    //             mRefCount = nullptr;
    //             delete mDeleterBase;
    //             mDeleterBase = nullptr;
    //         }
    //         // カスタムデリータが定義されていなかった場合
    //         else
    //         {
    //             delete mPtr;
    //             mPtr = nullptr;
    //             delete mRefCount;
    //             mRefCount = nullptr;
    //         }
    //     }
    // }

private:
    template <typename U> friend class RefPtr;

    //SharedDeleterBase *mDeleterBase;
    T *mPtr;
    //std::sitz_t* mRefCount;
    //std::mutex mMtx;

    friend RefPtr<T> AdoptRef<T>(T*); // フレンド関数

    enum AdoptTag { ADOPT };
};


// Adoptタグをつける専用のCreator関数
template <typename T>
inline RefPtr<T> AdoptRef(T* ptr)
{
#ifndef NDEBU
    // Debug Mode
    ptr->Adopt();
#endif
    return RefPtr<T>(ptr, RefPtr<T>::ADOPT);
}

// 既に中身がある生ポインタのためのラップ関数
template <typename T>
inline RefPtr<T> Ref(T *plain_ptr)
{
    return RefPtr<T>(plain_ptr);
}

namespace internal
{
    template <typename T>
    class MakeRefCountedHelder final
    {
    public:
        template <typename... Args>
        static RefPtr<T> MakeRefCounted(Args&&... args)
        {
            return AdoptRef<T>(new T(std::forward<Args>(args)...));
        }
    };
}

template <typename T, typename... Args>
RefPtr<T> MakeRefCounted(Args&&... args)
{
    return internal::MakeRefCountedHelder<T>::MakeRefCounted(
        std::forward<Args>(args)...);
}


// std custom hash
namespace std
{
    template <typename T>
    struct hash<RefPtr<T>>
    {
        using argument_type = RefPtr<T>;
        using result_type = std::size_t;

        result_type operator()(const argument_type& ptr) const
        {
            return std::hash<T*>()(ptr.Get());
        }
    };
}