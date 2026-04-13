#pragma once
#include "Frame.hpp"
#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <optional>

template <typename T>
class FrameQueue
{
    public:
        // Constructor
        FrameQueue(size_t max_size = 10) : max_size_(max_size) {}

        // Add a frame to the queue
        void push(Frame<T> frame)
        {
            std::unique_lock<std::mutex> lock(mutex_);  // locks, unlocks automatically on scope exit
            // only one thread can be here at a time
            if(queue_.size() >= max_size_)
            {
                std::cout << "Queue is full. Producer is waiting...\n";
                cv_producer_.wait(lock, [this]{ return queue_.size() < max_size_; }); // block until space is available
            }
            queue_.push(std::move(frame));
            std::cout << "Pushed a frame to the queue. Queue size: " << queue_.size() << std::endl;
            cv_.notify_one(); // wake up one waiting thread, if any

        }

        // Remove frame from the queue, blocks if empty
        std::optional<Frame<T>> pop() {
            std::unique_lock<std::mutex> lock(mutex_);

            cv_.wait(lock, [this]{ return !queue_.empty() || done_; });  // sleeps until data arrives
            if (queue_.empty()) return std::nullopt;

            Frame<T> frame = std::move(queue_.front());
            queue_.pop();
            
            cv_producer_.notify_one(); // wake up one waiting producer thread, if any
            std::cout << "Popped a frame from the queue. Queue size: " << queue_.size() << std::endl;
            return frame;
        }

        void finish()
        {
            std::unique_lock<std::mutex> lock(mutex_);  // locks, unlocks automatically on scope exit
            // only one thread can be here at a time

            done_ = true;
            std::cout << "Finished adding frames. No more frames will be added.\n";
            cv_.notify_all(); // wake up all waiting threads
        }

    private:
        std::queue<Frame<T>> queue_; // Queue to hold frames of type T
        std::condition_variable cv_; // Condition variable for thread synchronization
        std::condition_variable cv_producer_; // Condition variable for thread synchronization
        bool done_ = false; // Flag to indicate no more frames will be added
        std::mutex mutex_; // Mutex to protect access to the queue and done flag
        size_t max_size_; // Optional max size for the queue, can be used to block producers if needed
};