#include "BlurFilter.hpp"
#include <iostream>

BlurFilter::BlurFilter(int radius)
    : radius_(radius)
{
}

void BlurFilter::process(Frame& frame) {
    // Placeholder — real blur implementation comes later
    // For now just demonstrates the dispatch mechanism
    std::cout << "[" << name() << "] processing " 
              << frame.width() << "x" << frame.height()
              << " frame with radius " << radius_ << "\n";
}