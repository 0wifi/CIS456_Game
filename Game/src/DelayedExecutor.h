#pragma once

#include <mutex>
#include <thread>
#include <chrono>
#include <functional>
#include <condition_variable>

class DelayedExecutor
{
public:
    explicit DelayedExecutor(std::function<void()> task)
        : _task(std::move(task))
        , _worker(&DelayedExecutor::thread_loop, this)
    {
    }

    DelayedExecutor(DelayedExecutor&&) = delete;
    DelayedExecutor(DelayedExecutor const&) = delete;
    DelayedExecutor& operator=(DelayedExecutor&&) = delete;
    DelayedExecutor& operator=(DelayedExecutor const&) = delete;

    ~DelayedExecutor()
    {
        {
            std::lock_guard lock(_mutex);  // NOLINT(modernize-use-scoped-lock)
            _shutdown = true;
        }
        _cv.notify_one();

        if (_worker.joinable())
        {
            _worker.join();
        }
    }

    void trigger(int delay_ms)
    {
        {
            std::lock_guard lock(_mutex);  // NOLINT(modernize-use-scoped-lock)
            _delay_ms = delay_ms;
            _pending = true;
        }
        _cv.notify_one();
    }

private:
    std::function<void()>   _task;
    std::thread             _worker;
    std::mutex              _mutex;
    std::condition_variable _cv;
    int                     _delay_ms = 0;
    bool                    _pending = false;
    bool                    _shutdown = false;

    void thread_loop()
    {
        while (true)
        {
            int current_delay = 0;

            {
                std::unique_lock lock(_mutex);
                _cv.wait(lock, [this]
                    {
                        return _pending || _shutdown;
                    });

                if (_shutdown)
                    break;

                current_delay = _delay_ms;
                _pending = false;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(current_delay));

            if (_task)
                _task();
        }
    }

};