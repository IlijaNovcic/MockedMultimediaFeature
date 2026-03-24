#include <iostream>
#include <vector>
#include "Frame.hpp"
#include <memory>  // needed for std::unique_ptr and std::make_unique
#include "filters/BlurFilter.hpp"
#include "filters/BrightnessFilter.hpp"
#include "filters/InvertFilter.hpp"

// Read-only — const reference, no copy
void print_frame_info(const Frame8& frame)
{
    std::cout << "Frame created with width: " << frame.width() 
              << ", height: " << frame.height() 
              << ", format: " << (frame.format() == RGB ? "RGB" : "YUV420P") 
              << ", size: " << frame.size() << " bytes" << std::endl;
}

int main() {
    
    Frame8 f8(1920, 1080, RGB); // create a frame
    FrameF hdr(1920, 1080, RGB); // create another frame

    // Print frame data pixels

    std::vector<std::unique_ptr<PipelineStage<uint8_t>>> pipeline;
    pipeline.push_back(std::make_unique<BlurFilter<uint8_t>>(3)); // add blur filter
    pipeline.push_back(std::make_unique<BrightnessFilter<uint8_t>>(1.2f)); // add brightness filter
    pipeline.push_back(std::make_unique<InvertFilter<uint8_t>>()); // add invert filter

    print_frame_info(f8);   // no copy — const ref

    for (auto& stage : pipeline) {
        stage->process(f8); // process in-place, no copy
    }

    // See that the HDR frame is much larger due to float data type
    std::cout << "HDR frame size: " << hdr.size() * sizeof(float) << " bytes" << std::endl;

    return 0;
}