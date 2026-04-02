#pragma once
#include <vector>
#include <memory>
#include <iostream>
#include "PipelineStage.hpp"
#include <chrono>

template<typename T>
class Pipeline {
public:
    void add_stage(std::unique_ptr<PipelineStage<T>> stage) {
        stages_.push_back(std::move(stage));
    }

    void process(Frame<T>& frame);
    void print_stages() const;
    size_t frames_processed() const { return frames_processed_; }
    // Increment the count of frames processed by this frame (for demonstration)
    
    private:
    std::vector<std::unique_ptr<PipelineStage<T>>> stages_;
    size_t frames_processed_ = 0; // for demonstration, track total frames processed across all stages
};

template<typename T>
void Pipeline<T>::process(Frame<T>& frame)
{
    auto start_time = std::chrono::high_resolution_clock::now();

    std::for_each(stages_.begin(), stages_.end(), [&frame](const auto& stage) {
        auto stage_start = std::chrono::high_resolution_clock::now();
        stage->process(frame); // process in-place, no copy
        auto stage_end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> stage_time = stage_end - stage_start;
        std::cout << "Stage [" << stage->name() << "] took " << stage_time.count() << " ms\n";
    });
    
    frames_processed_++; // for demonstration, track how many stages have processed this frame
    auto end_time = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> processing_time = end_time - start_time;
    std::cout << "Total processing time: " << processing_time.count() << " ms\n";
}

template<typename T>
void Pipeline<T>::print_stages() const
{
    std::cout << "Pipeline stages: \n";
    std::for_each(stages_.begin(), stages_.end(), [](const auto& stage) {
        std::cout << " - " << stage->name() << "\n";
    });
}