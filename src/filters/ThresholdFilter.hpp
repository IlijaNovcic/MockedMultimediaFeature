#pragma once
#include "PipelineStage.hpp"
#include "Frame.hpp"
#include <iostream>
#include <string_view>

template<typename T>
class ThresholdFilter : public PipelineStage<T>
{
    public:
        explicit ThresholdFilter(T threshold) : threshold_(threshold) {}
        void process(Frame<T>& frame) override;
        std::string_view name() const override { return "ThresholdFilter"; }
    private:
        T threshold_;
};

template<typename T>
void ThresholdFilter<T>::process(Frame<T>& frame)
{
    for(size_t i = 0; i < frame.size(); ++i)
    {
        if (frame.data()[i] < threshold_)
            frame.data()[i] = 0; // set to black if below threshold
        else
            frame.data()[i] = max_pixel_value<T>(); // set to white if above threshold 
    }
    std::cout << "[" << name() << "] applied threshold " << +threshold_ << "\n";
}