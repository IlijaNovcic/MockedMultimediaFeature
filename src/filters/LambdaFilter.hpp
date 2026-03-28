#pragma once 
#include <functional>
#include "Frame.hpp"
#include "PipelineStage.hpp"
#include <iostream>

template<typename T>
class LambdaFilter : public PipelineStage<T> {
    public: 
        explicit LambdaFilter(std::function<T(T)> transform)
        : transform_(transform) 
        {

        }

        void process(Frame<T>& frame) override;
        const char* name() const override { return "LambdaFilter"; }
    private:
        std::function<T(T)> transform_; // user-provided lambda function for pixel transformation
};

template<typename T>
void LambdaFilter<T>::process(Frame<T>& frame)
{
    T* pixels = frame.data();
    
    for (size_t i = 0; i < frame.size(); ++i)
    {
        pixels[i] = transform_(pixels[i]);
    }

    std::cout << "[" << name() << "] applied transform function provided by user\n";
}