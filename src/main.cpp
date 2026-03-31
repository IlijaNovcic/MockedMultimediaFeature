#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>  // needed for std::unique_ptr and std::make_unique
#include "Frame.hpp"
#include "filters/BlurFilter.hpp"
#include "filters/LambdaFilter.hpp"
#include "Pipeline.hpp"
#include "FrameQueue.hpp"
#include <thread>

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

// Make producer thread function to push frames into the queue
void producer(FrameQueue<uint8_t>& frame_queue)
{
    for (size_t i = 0; i < 5u; ++i)
    {
        Frame8 frame = make_frame(1920 / (i + 1), 1080 / (i + 1), RGB); // create frames of decreasing size
        frame_queue.push(std::move(frame)); // move frame into the queue
    }
    frame_queue.finish(); // signal that no more frames will be added
}

// Consumer thread function to pop frames from the queue and process them using pipeline vector
void consumer(FrameQueue<uint8_t>& frame_queue, Pipeline<uint8_t>& pipeline)
{
    while(true)
    {
        auto frame = frame_queue.pop();
        if(!frame)
            break;
        pipeline.process(*frame);
    }
}

int main() {
    Pipeline<uint8_t> pipeline; // create a processing pipeline
    float brightness_factor = 1.5f; // example factor to brighten the image
    FrameQueue<uint8_t> frame_queue; // create a frame queue for thread-safe communication
    
    // Print frame data pixels
    pipeline.add_stage(std::make_unique<BlurFilter<uint8_t>>(3)); // add blur filter
    pipeline.add_stage(std::make_unique<LambdaFilter<uint8_t>>([brightness_factor](uint8_t pixel) -> uint8_t {
                                                                  return static_cast<uint8_t>(std::clamp(static_cast<float>(pixel) * brightness_factor, 0.0f, 255.0f));
                                                                  }, "Brightness")); // add lambda filter to adjust brightness
    pipeline.add_stage(std::make_unique<LambdaFilter<uint8_t>>([](uint8_t pixel) -> uint8_t {
                                                                  return 255 - pixel;
                                                                  }, "Invert")); // add invert filter

    // Create both producer and consumer threads to demonstrate thread-safe frame processing
    std::thread producer_thread(producer, std::ref(frame_queue));
    std::thread consumer_thread(consumer, std::ref(frame_queue), std::ref(pipeline));

    // Start both threads to process frames from the queue
    producer_thread.join();
    consumer_thread.join();

    return 0;
}