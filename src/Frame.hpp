#pragma once
#include <cstdint>
#include <cstddef>
#include <memory>

// Format of the pixel data in the frame.
enum PixelFormat
{
    RGB,
    YUV420P,
};

// Represents a video frame with its associated metadata.
class Frame
{
    public: 
        // Constructor
        Frame(int width, int height, PixelFormat format);

        // No need to declare ~Frame() anymore — unique_ptr handles cleanup

        // Getters - Accessors for frame properties
        int width()  const { return width_; }
        int height() const { return height_; }
        PixelFormat format() const { return format_; }
        size_t size() const { return size_; }
    
        uint8_t*       data()       { return data_.get(); }
        const uint8_t* data() const { return data_.get(); }

        // Fields of the frame
        private:
        int         width_;
        int         height_;
        PixelFormat format_;
        size_t      size_;
        std::unique_ptr<uint8_t[]> data_;
};