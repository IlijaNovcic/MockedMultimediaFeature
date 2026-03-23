#include <iostream>
#include <vector>
#include "Frame.hpp"

// Read-only — const reference, no copy
void print_frame_info(const Frame& frame)
{
    std::cout << "Frame created with width: " << frame.width() 
              << ", height: " << frame.height() 
              << ", format: " << (frame.format() == RGB ? "RGB" : "YUV420P") 
              << ", size: " << frame.size() << " bytes" << std::endl;
}

// Modifies the frame — non-const reference
void clear_frame(Frame& frame)
{
    for(size_t i = 0; i < frame.size(); ++i)
    {
        frame.data()[i] = 0; // Clear the frame data
    }
}

// Returns a frame — by value (we'll optimize with move semantics in Topic 7)
Frame make_test_frame(int width, int height) {
    return Frame(width, height, PixelFormat::RGB);
}

int main() {
    
    Frame f = make_test_frame(1920, 1080);
    
    print_frame_info(f);   // no copy — const ref
    clear_frame(f);        // modifies f directly — ref
    print_frame_info(f);   // verify it still works

    return 0;
}