#pragma once
#include "PipelineStage.hpp"

template<typename T>
class BlurFilter : public PipelineStage<T> {
public:
    explicit BlurFilter(int radius = 1);

    void process(Frame<T>& frame) override;
    const char* name() const override { return "BlurFilter"; }

private:
    int radius_;
};

// Implementation in header — template requirement
template<typename T>
BlurFilter<T>::BlurFilter(int radius)
    : radius_(radius)
{
}

template<typename T>
void BlurFilter<T>::process(Frame<T>& frame) {
    std::cout << "[" << name() << "] processing "
              << frame.width() << "x" << frame.height()
              << " frame with radius " << radius_ << "\n";
}