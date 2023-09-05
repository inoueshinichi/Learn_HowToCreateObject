/**
 * @file debug_delegate.hpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-09-05
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <string>
#include <cstdio>
#include <ostream>

/* Interface protocol */
class MemoryDebugProtocol
{
public:
    MemoryDebugProtocol() = default;
    virtual ~MemoryDebugProtocol() = default;
    MemoryDebugProtocol(const MemoryDebugProtocol &) = delete;
    MemoryDebugProtocol &operator=(const MemoryDebugProtocol &) = delete;
    MemoryDebugProtocol(MemoryDebugProtocol &&) = default;
    MemoryDebugProtocol &operator=(MemoryDebugProtocol &&) = default;

    virtual void on_allocation_try_failure() = 0;
    virtual void on_allocation_retry_failure() = 0;
    virtual void on_free_unused_cache_map(const std::string &cache_type) = 0;
    virtual void on_free(size_t bytes, const std::string &cache_type) = 0;
    virtual void on_alloc(size_t bytes, const std::string &cache_type) = 0;
};

/* Base */
class MemoryDelegate final : public MemoryDebugProtocol
{
private:
    std::string _name;

public:
    explicit MemoryDelegate(const std::string &name) : _name(name) {}
    ~MemoryDelegate() {}

    void on_allocation_try_failure() override final
    {
        std::printf("MemoryDelegate::on_allocation_try_failure() in %s.\n", _name.c_str());
    }

    void on_allocation_retry_failure() override final
    {
        std::printf("MemoryDelegate::on_allocation_retry_failure() in %s.\n", _name.c_str());
    }

    void on_free_unused_cache_map(const std::string &cache_type) override final
    {
        std::printf("MemoryDelegate::on_free_unused_cache_map() in %s.\n", _name.c_str());
    }

    void on_free(size_t bytes, const std::string &cache_type) override final
    {
        std::printf("MemoryDelegate::on_free() in %s.\n", _name.c_str());
    }

    void on_alloc(size_t bytes, const std::string &cache_type) override final
    {
        std::printf("MemoryDelegate::on_alloc() in %s.\n", _name.c_str());
    }
};