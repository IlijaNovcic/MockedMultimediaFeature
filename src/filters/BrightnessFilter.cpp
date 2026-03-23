#include "BrightnessFilter.hpp"
#include <iostream>
#include <algorithm>

// Constructor
BrightnessFilter::BrightnessFilter(float factor)
    : factor_(factor)
{
}

void BrightnessFilter::process(Frame& frame)
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
