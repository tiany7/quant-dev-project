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
#include <any>

// std::chrono::system_clock::time_point ConvertStringToTime(const std::string& timeStr, const std::string& format = "%Y-%m-%d %H:%M:%S") {
//     std::tm tmStruct = {};
//     std::istringstream ss(timeStr);
//     ss >> std::get_time(&tmStruct, format.c_str());

//     if (ss.fail()) {
//         throw std::runtime_error("Failed to parse the time string");
//     }

//     std::time_t time = std::mktime(&tmStruct);

//     if (time == -1) {
//         throw std::runtime_error("Failed to convert the time structure to time_t");
//     }

//     return std::chrono::system_clock::from_time_t(time);
// }

struct ChannelError
{
    std::string msg;
    ChannelError(const std::string &_msg) : msg(_msg) {}
    ChannelError(std::string &&_msg) : msg(_msg) {}

    std::string what() const
    {
        return msg;
    }

    friend std::ostream &operator<<(std::ostream &os, const ChannelError &error)
    {
        os << error.msg;
        return os;
    }
};

template <typename T>
using Result = std::variant<T, ChannelError>;

// overload the << operator for Result<T>
template <typename T>
std::ostream &operator<<(std::ostream &os, const Result<T> &result)
{
    if (std::holds_alternative<T>(result))
    {
        os << std::get<T>(result);
    }
    else
    {
        os << std::get<ChannelError>(result);
    }
    return os;
}

// write a is_err function
template <typename T>
bool is_err(const Result<T> &result)
{
    return std::holds_alternative<ChannelError>(result);
}

// mpsc channel(unbounded)
template <typename T>
class MPSCChannel
{
public:
    MPSCChannel() : condition_(), shutdown_(false) {}

    Result<T> send(const T &data)
    {
        std::lock_guard<std::mutex> lock(mutex_);

        if (shutdown_)
        {
            return Result<T>(ChannelError("Channel has been shutdown"));
        }

        queue_.push(data);
        condition_.notify_one();

        return Result<T>(data);
    }

    Result<T> receive()
    {
        std::unique_lock<std::mutex> lock(mutex_);

        condition_.wait(lock, [this]
                        { return !queue_.empty() || shutdown_; });
        // if (shutdown_ && queue_.empty()) {
        //     return Result<T>(ChannelError("Channel has been shutdown"));
        // }
        if (!queue_.empty())
        {
            T data = queue_.front();
            queue_.pop();
            return Result<T>(data);
        }

        return Result<T>(ChannelError("Unknown error"));
    }

    void shutdown()
    {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            shutdown_ = true;
        }

        condition_.notify_all(); // Notify all waiting threads
    }

    ~MPSCChannel()
    {
        if (!shutdown_)
            shutdown();
    }

private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable condition_;
    bool shutdown_;
};

namespace utils::mpsc
{
    // mpsc channel(bounded)
    template <typename T>
    class MPSCChannel
    {
    public:
        MPSCChannel(int _capacity) : capacity(_capacity), shutdown_(false) {
            if (capacity <= 0) {
                throw std::runtime_error("Capacity must be positive");
            }
            // queue_.resize(capacity);
        }

        // bounded channel
        Result<T> send(const T &data)
        {
            std::unique_lock<std::mutex> lock(mutex_);

            if (shutdown_)
            {
                return Result<T>(ChannelError("Channel has been shutdown"));
            }

            condition_.wait(lock, [this]{ return queue_.size() < capacity || exit_; });

            if (exit_) {
                return Result<T>(ChannelError("Channel has been closed"));
            }
            queue_.push_back(data);
            condition_.notify_one();

            return Result<T>(data);
        }

        Result<T> receive()
        {
            std::unique_lock<std::mutex> lock(mutex_);

            condition_.wait(lock, [this]
                            { return !queue_.empty() || shutdown_ || exit_; });
            // if (shutdown_ && queue_.empty()) {
            //     return Result<T>(ChannelError("Channel has been shutdown"));
            // }
            // std::cerr<<"queue size: "<<queue_.size()<<std::endl;
            if (!queue_.empty())
            {
                T data = queue_.front();
                queue_.pop_front();
                return Result<T>(data);
            }

            return Result<T>(ChannelError("Unknown error"));
        }

        void shutdown()
        {
            {
                std::lock_guard<std::mutex> lock(mutex_);
                shutdown_ = true;
            }

            condition_.notify_all(); // Notify all waiting threads
        }

        void force_exit() {
            exit_ = true;
            condition_.notify_all();
        }

        ~MPSCChannel()
        {
            if (!shutdown_)
                shutdown();
        }

    private:
        std::deque<T> queue_;
        int capacity;
        std::mutex mutex_;
        std::condition_variable condition_;
        std::atomic_bool shutdown_ = false;
        std::atomic_bool exit_ = false;
    };


    template <typename T>
    class Sender {
    private:
        utils::mpsc::MPSCChannel<T> *channel_ptr;
    public:
        Sender(utils::mpsc::MPSCChannel<T> *_channel_ptr) : channel_ptr(_channel_ptr) {}
        Result<T> send(const T &data) {
            return channel_ptr->send(data);
        }

        ~Sender() {
            channel_ptr->shutdown();
        }
    };

    template <typename T>
    class Receiver {
    private:
        utils::mpsc::MPSCChannel<T> *channel_ptr;
    public:
        Receiver(utils::mpsc::MPSCChannel<T> *_channel_ptr) : channel_ptr(_channel_ptr) {}
        Result<T> receive() {
            return channel_ptr->receive();
        }

        void force_exit() {
            channel_ptr->force_exit();
        }

        ~Receiver() {
            channel_ptr->force_exit();
        }
    };

    template <typename T>
    std::pair<std::shared_ptr<Sender<T>>, std::unique_ptr<Receiver<T>>> create_channel(int capacity) {
        auto channel = new utils::mpsc::MPSCChannel<T>(capacity);
        auto sender_ptr = std::make_shared<Sender<T>>(channel);
        auto receiver_ptr = std::make_unique<Receiver<T>>(channel);
        return std::make_pair(sender_ptr, std::move(receiver_ptr));
    }
}


using receiver_t = std::unique_ptr<utils::mpsc::Receiver<std::any>>;
using sender_t = std::shared_ptr<utils::mpsc::Sender<std::any>>;