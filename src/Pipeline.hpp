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

private:
    std::vector<std::unique_ptr<PipelineStage<T>>> stages_;
};

template<typename T>
void Pipeline<T>::process(Frame<T>& frame)
{
    auto start_time = std::chrono::high_resolution_clock::now();;

    std::for_each(stages_.begin(), stages_.end(), [&frame](const auto& stage) {
        stage->process(frame); // process in-place, no copy
    });
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