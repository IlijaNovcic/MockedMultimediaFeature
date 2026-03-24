#pragma once
#include "PipelineStage.hpp"
#include <algorithm>
#include <iostream>

template<typename T>
class BrightnessFilter : public PipelineStage<T>
{
    public:
        explicit BrightnessFilter(float factor = 1.0f); 
        void process(Frame<T>& frame) override;
        const char* name() const override { return "BrightnessFilter"; }
    private:
        float factor_; // >1.0 to brighten, <1.0 to darken
};

// Constructor
template<typename T>
BrightnessFilter<T>::BrightnessFilter(float factor)
    : factor_(factor)
{
}

// Overridden process method
template<typename T>
void BrightnessFilter<T>::process(Frame<T>& frame)
{
    uint8_t* pixels = frame.data();

    // Clamp each filter 0-255 and apply brightness factor
    for (size_t i = 0; i < frame.size(); ++i)
    {
        // clamp to 0-255 after scaling
        pixels[i] = static_cast<uint8_t>(
            std::clamp(pixels[i] * factor_, 0.0f, 255.0f)
        );
    }
    std::cout << "[" << name() << "] applied factor " << factor_ << "\n";
}