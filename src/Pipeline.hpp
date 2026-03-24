#pragma once
#include <vector>
#include <memory>
#include <iostream>
#include "PipelineStage.hpp"

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
    std::for_each(stages_.begin(), stages_.end(), [&frame](const auto& stage) {
        stage->process(frame); // process in-place, no copy
    });
}

template<typename T>
void Pipeline<T>::print_stages() const
{
    std::cout << "Pipeline stages: \n";
    std::for_each(stages_.begin(), stages_.end(), [](const auto& stage) {
        std::cout << " - " << stage->name() << "\n";
    });
}