#include <iostream>
#include <vector>
#include "Frame.hpp"
#include <memory>  // needed for std::unique_ptr and std::make_unique
#include "filters/BlurFilter.hpp"
#include "filters/BrightnessFilter.hpp"
#include "filters/InvertFilter.hpp"
#include "filters/LambdaFilter.hpp"
#include "Pipeline.hpp"

// Read-only — const reference, no copy
void print_frame_info(const Frame8& frame)
{
    std::cout << "Frame created with width: " << frame.width() 
              << ", height: " << frame.height() 
              << ", format: " << (frame.format() == RGB ? "RGB" : "YUV420P") 
              << ", size: " << frame.size() << " bytes" << std::endl;
}

Frame8 make_frame(int width, int height, PixelFormat format)
{
    return Frame8(width, height, format); // Return by value — relies on move semantics, no copy
}

int main() {
    
    Frame8 f8 = make_frame(1920, 1080, RGB); // create a frame
    Frame8 f8_new = std::move(f8); // move the frame, f8 is now empty (moved-from state)
    FrameF hdr(1920, 1080, RGB); // create another frame
    Pipeline<uint8_t> pipeline; // create a processing pipeline

    // Print frame data pixels
    pipeline.add_stage(std::make_unique<BlurFilter<uint8_t>>(3)); // add blur filter
    pipeline.add_stage(std::make_unique<LambdaFilter<uint8_t>>([](uint8_t pixel) -> uint8_t {
                                                                  return pixel > 128 ? 128 : pixel;
                                                                  })); // add lambda filter to cap pixel values at 128
    pipeline.add_stage(std::make_unique<LambdaFilter<uint8_t>>([](uint8_t pixel) -> uint8_t {
                                                                  return 255 - pixel;
                                                                  })); // add invert filter

    print_frame_info(f8_new);   // no copy — const ref

    // See that the HDR frame is much larger due to float data type
    std::cout << "HDR frame size: " << hdr.size() * sizeof(float) << " bytes" << std::endl;
    pipeline.process(f8_new); // process the frame through the pipeline
    pipeline.print_stages(); // print all stage names

    return 0;
}