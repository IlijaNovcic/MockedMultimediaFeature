#include <iostream>
#include <vector>
#include "Frame.hpp"
#include "filters/BlurFilter.hpp"
#include "filters/BrightnessFilter.hpp"

// Read-only — const reference, no copy
void print_frame_info(const Frame& frame)
{
    std::cout << "Frame created with width: " << frame.width() 
              << ", height: " << frame.height() 
              << ", format: " << (frame.format() == RGB ? "RGB" : "YUV420P") 
              << ", size: " << frame.size() << " bytes" << std::endl;
}

int main() {
    
    Frame f = Frame(1920, 1080, RGB); // create a frame
    std::vector<std::unique_ptr<PipelineStage>> pipeline;
    pipeline.push_back(std::make_unique<BlurFilter>(3)); // add blur filter
    pipeline.push_back(std::make_unique<BrightnessFilter>(1.2f)); // add brightness filter

    print_frame_info(f);   // no copy — const ref

    for (auto& stage : pipeline) {
        stage->process(f); // process in-place, no copy
    }

    return 0;
}