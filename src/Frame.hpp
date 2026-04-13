#pragma once
#include <cstdint>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <limits>
#include <cstring>
#include <string>

// Format of the pixel data in the frame.
enum PixelFormat
{
    RGB,
    YUV420P,
};

// FrameMetadata struct can be added here if needed in the future, for now we keep it simple with just the Frame class.
struct FrameMetadata
{
    size_t frame_number;
    double timestamp_ms;
    std::string source_name;
};

template<typename T>
// Represents a video frame with its associated metadata.
class Frame
{
    public: 
        // Constructor
        Frame(int width, int height, PixelFormat format, FrameMetadata metadata = {})
            : width_(width) 
            , height_(height) 
            , format_(format) 
            , size_(compute_size(width, height, format))
            , data_(std::make_unique<T[]>(size_))
            , metadata_(std::move(metadata))
        {
            // member initializer list above is the C++ way to initialize members
            // prefer it over assignment in the constructor body
        }

        // Move constructor 
        Frame(Frame&& other) noexcept
            : width_(other.width_)
            , height_(other.height_)
            , format_(other.format_)
            , size_(other.size_)
            , data_(std::move(other.data_)) // move ownership of the data
            , metadata_(std::move(other.metadata_))
        {
            // other.data_ is now null, no copy of pixel data occurred
        }

        // Move assignment operator
        Frame& operator=(Frame&& other) noexcept
        {
            if (this != &other) // protect against self-assignment
            {
                width_ = other.width_;
                height_ = other.height_;
                format_ = other.format_;
                size_ = other.size_;
                data_ = std::move(other.data_); // move ownership of the data
                metadata_ = std::move(other.metadata_);
            }
            return *this;
        }

        // No need to declare ~Frame() anymore — unique_ptr handles cleanup

        // Getters - Accessors for frame properties
        int width()  const { return width_; }
        int height() const { return height_; }
        PixelFormat format() const { return format_; }
        size_t size() const { return size_; }
        T*       data()       { return data_.get(); }
        const T* data() const { return data_.get(); }
        const FrameMetadata& metadata() const { return metadata_; }

        Frame clone() const
        {
            Frame copy(width_, height_, format_, metadata_); // create a new frame with the same properties
            std::memcpy(copy.data(), data_.get(), size_ * sizeof(T)); // copy pixel data
            return copy; // relies on move semantics, no copy of the frame object itself
        }

        // Fields of the frame
        private:
        int                  width_;
        int                  height_;
        PixelFormat          format_;
        size_t               size_;
        std::unique_ptr<T[]> data_;
        FrameMetadata        metadata_; // Optional metadata field for future use
        // class field — one shared value, same for all Frame objects
        static constexpr size_t RGB_CHANNELS = 3;
        
        static constexpr size_t compute_size(int width, int height, PixelFormat format)
        {
            if(width <= 0 || height <= 0)
                throw std::invalid_argument("Width and height must be positive integers");

            if(format == YUV420P && (width % 2 != 0 || height % 2 != 0))
                throw std::invalid_argument("YUV420P format requires even width and height");
                
            switch (format)
            {
                case RGB: return width * height * RGB_CHANNELS; // 3 bytes per pixel
                case YUV420P: return width * height + (width / 2) * (height / 2) * 2; // Y plane + U and V planes
                default: throw std::invalid_argument("Unsupported pixel format");
            }
        }
};

template<typename T>
T max_pixel_value() {
    if constexpr (std::is_floating_point_v<T>)
        return 1.0f;   // float frames use 0.0-1.0 range
    else
        return std::numeric_limits<T>::max();  // integer frames use full range
}

// Convenient type aliases
using Frame8  = Frame<uint8_t>;   // standard 8-bit video
using Frame16 = Frame<uint16_t>;  // 10/12-bit video
using FrameF  = Frame<float>;     // HDR / intermediate processing