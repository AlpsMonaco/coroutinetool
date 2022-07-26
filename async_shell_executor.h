#ifndef __ASYNC_SHELL_EXECUTOR_H
#define __ASYNC_SHELL_EXECUTOR_H

#include "prefix.h"
#include <stdio.h>

#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#endif

COROUTINE_TOOL_NAMESPACE_START
class AsyncExecutor
{
public:
    struct promise_type
    {
        AsyncExecutor get_return_object() { return AsyncExecutor(handle_type::from_promise(*this)); }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(const char* p_msg)
        {
            p_msg_ = p_msg;
            return {};
        }
        void unhandled_exception() { exception_ = std::current_exception(); }
        void return_value(int code) { return_code_ = code; }

        const char* p_msg_;
        std::exception_ptr exception_;
        int return_code_;
    };
    using handle_type = std::coroutine_handle<promise_type>;
    AsyncExecutor(handle_type handle) : handle_(handle) {}
    AsyncExecutor(const AsyncExecutor& rhs) : handle_(rhs.handle_) {}
    AsyncExecutor(AsyncExecutor&& rhs) : handle_(rhs.handle_) { rhs.handle_ = nullptr; }
    AsyncExecutor& operator=(const AsyncExecutor& rhs)
    {
        if (handle_ != nullptr)
        {
            handle_.destroy();
        }
        handle_ = rhs.handle_;
        return *this;
    }
    AsyncExecutor& operator=(AsyncExecutor&& rhs)
    {
        if (handle_ != nullptr)
        {
            handle_.destroy();
        }
        handle_     = rhs.handle_;
        rhs.handle_ = nullptr;
        return *this;
    }
    ~AsyncExecutor()
    {
        if (handle_ != nullptr)
            handle_.destroy();
    }
    explicit operator bool()
    {
        handle_.resume();
        return !handle_.done();
    }

    int Code() { return handle_.promise().return_code_; }
    static AsyncExecutor Run(const char* command)
    {
        FILE* fp = NULL;
        fp       = popen(command, "r");
        if (fp == NULL)
            co_return -1;
        static constexpr size_t buffer_size = 128;
        char buf[buffer_size];
        while (fgets(buf, buffer_size, fp) != NULL)
            co_yield buf;
        co_return pclose(fp);
    }

    const char* operator()()
    {
        return handle_.promise().p_msg_;
    }

protected:
    handle_type handle_;
};

COROUTINE_TOOL_NAMESPACE_END

#endif