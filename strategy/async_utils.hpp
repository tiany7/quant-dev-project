#pragma once

#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>
#include <list>
#include <functional>



// these are some async utilities that are implemented in lock-free manner
class async_condition_variable {
private:
    std::atomic_bool flag;
public:
    async_condition_variable() : flag(false) {}

    void notify_one() {
        flag.store(true);
    }

    void wait(std::function<bool(void)> predicate) {
        for (bool expected = false; !flag.compare_exchange_strong(expected, false); expected = false) {
            if (predicate()) {
                return;
            }
        }
    }

    // assume we only have one thread waiting
    void notify() {
        flag.store(true);
    }
};


class async_mutex {
private:
    std::atomic_bool flag;
public:
    async_mutex() : flag(false) {}

    void lock() {
        for (bool expected = false; !flag.compare_exchange_weak(expected, true); expected = false) {
            // self spin
        }
    }

    void unlock() {
        flag.store(false);
    }
};
