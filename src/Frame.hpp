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

template<typename T>
// Represents a video frame with its associated metadata.
class Frame
{
    public: 
        // Constructor
        Frame(int width, int height, PixelFormat format)
            : width_(width) 
            , height_(height) 
            , format_(format) 
            , size_(compute_size(width, height, format))
            , data_(std::make_unique<T[]>(size_))
        {
            // member initializer list above is the C++ way to initialize members
            // prefer it over assignment in the constructor body
        }

        // No need to declare ~Frame() anymore — unique_ptr handles cleanup

        // Getters - Accessors for frame properties
        int width()  const { return width_; }
        int height() const { return height_; }
        PixelFormat format() const { return format_; }
        size_t size() const { return size_; }
    
        T*       data()       { return data_.get(); }
        const T* data() const { return data_.get(); }

        // Fields of the frame
        private:
            static size_t compute_size(int width, int height, PixelFormat format)
            {
                switch (format)
                {
                    case RGB: return width * height * 3; // 3 bytes per pixel
                    case YUV420P: return width * height + (width / 2) * (height / 2) * 2; // Y plane + U and V planes
                    default: throw std::invalid_argument("Unsupported pixel format");
                }
            }
        int         width_;
        int         height_;
        PixelFormat format_;
        size_t      size_;
        std::unique_ptr<T[]> data_;
};

// Convenient type aliases
using Frame8  = Frame<uint8_t>;   // standard 8-bit video
using Frame16 = Frame<uint16_t>;  // 10/12-bit video
using FrameF  = Frame<float>;     // HDR / intermediate processing