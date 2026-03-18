#pragma once
#include <cstdint>
#include <cstddef>

// Represents a video frame with its associated metadata.
enum PixelFormat
{
    RGB,
    YUV420P,
};

class Frame
{
    public: 
        // Constructor
        Frame(int width, int height, PixelFormat format);

        // Destructor
        ~Frame();

        // Getters - Accessors for frame properties
        int width()  const { return width_; }
        int height() const { return height_; }
        PixelFormat format() const { return format_; }
        size_t size() const { return size_; }
    
        uint8_t*       data()       { return data_; }
        const uint8_t* data() const { return data_; }

        // Fields of the frame
        private:
        uint8_t*    data_;
        int         width_;
        int         height_;
        PixelFormat format_;
        size_t      size_;
};