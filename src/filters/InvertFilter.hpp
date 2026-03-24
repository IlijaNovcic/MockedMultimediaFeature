#pragma once
#include <iostream>
#include "PipelineStage.hpp"

template<typename T>
class InvertFilter : public PipelineStage<T>
{
    public:
        void process(Frame<T>& frame) override;
        const char* name() const override { return "InvertFilter"; }
};

template<typename T>
void InvertFilter<T>::process(Frame<T>& frame)
{
    T* pixels = frame.data();

    for (size_t i = 0; i < frame.size(); ++i)
    {
        pixels[i] = static_cast<T>(255 - pixels[i]); // Invert pixel value
    }

    std::cout << "[" << name() << "] applied \n";
}