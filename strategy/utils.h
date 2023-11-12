#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>
#include <variant>
#include <string>
#include <string_view>


std::chrono::system_clock::time_point ConvertStringToTime(const std::string& timeStr, const std::string& format = "%Y-%m-%d %H:%M:%S") {
    std::tm tmStruct = {};
    std::istringstream ss(timeStr);
    ss >> std::get_time(&tmStruct, format.c_str());

    if (ss.fail()) {
        throw std::runtime_error("Failed to parse the time string");
    }

    std::time_t time = std::mktime(&tmStruct);

    if (time == -1) {
        throw std::runtime_error("Failed to convert the time structure to time_t");
    }

    return std::chrono::system_clock::from_time_t(time);
}

struct ChannelError{
    std::string msg;
    ChannelError(const std::string& _msg) : msg(_msg) {}
    ChannelError(std::string&& _msg) : msg(_msg) {}

    std::string what() const {
        return msg;
    }

    friend std::ostream& operator<<(std::ostream& os, const ChannelError& error) {
        os << error.msg;
        return os;
    }

    
};

template <typename T>
using Result = std::variant<T, ChannelError>;

// overload the << operator for Result<T>
template <typename T>
std::ostream& operator<<(std::ostream& os, const Result<T>& result) {
    if (std::holds_alternative<T>(result)) {
        os << std::get<T>(result);
    } else {
        os << std::get<ChannelError>(result);
    }
    return os;
}



// write a is_err function
template <typename T>
bool is_err(const Result<T>& result) {
    return std::holds_alternative<ChannelError>(result);
}



// mpsc channel(unbounded)
template <typename T>
class MPSCChannel {
public:
    MPSCChannel() : condition_(), shutdown_(false) {}

    Result<T> send(const T& data) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        if (shutdown_) {
            return Result<T>(ChannelError("Channel has been shutdown"));
        }

        queue_.push(data);
        condition_.notify_one();

        return Result<T>(data);
    }

    Result<T> receive() {
        std::unique_lock<std::mutex> lock(mutex_);

        condition_.wait(lock, [this] { return !queue_.empty() || shutdown_; });

        if (!queue_.empty()) {
            T data = queue_.front();
            queue_.pop();
            return Result<T>(data);
        } else if (shutdown_) {
            return Result<T>(ChannelError("Channel has been shutdown"));
        }

        return Result<T>(ChannelError("Unknown error"));
    }

    void shutdown() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            shutdown_ = true;
        }

        condition_.notify_all(); // Notify all waiting threads
    }

    ~MPSCChannel() {
        shutdown();
    }

private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable condition_;
    bool shutdown_;
};