#pragma once
#include <functional>
#include "Frame.hpp"
#include "PipelineStage.hpp"
#include <iostream>

template <typename T>
class LambdaFilter : public PipelineStage<T>
{
public:
    LambdaFilter(std::function<T(T)> transform, const char *name = "LambdaFilter")
        : transform_(transform), name_(name)
    {
    }

    void process(Frame<T> &frame) override;
    const char *name() const override { return name_; }

private:
    std::function<T(T)> transform_; // user-provided lambda function for pixel transformation
    const char *name_;              // optional name for the filter
};

template <typename T>
void LambdaFilter<T>::process(Frame<T> &frame)
{
    T *pixels = frame.data();

    for (size_t i = 0; i < frame.size(); ++i)
    {
        pixels[i] = transform_(pixels[i]);
    }

    std::cout << "Applied [" << name() << "] filter provided by user\n";
}