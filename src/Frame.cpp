#include "Frame.hpp"
#include <stdexcept>

static size_t compute_size(int width, int height, PixelFormat format)
{
    switch (format)
    {
        case RGB: return width * height * 3; // 3 bytes per pixel
        case YUV420P: return width * height + (width / 2) * (height / 2) * 2; // Y plane + U and V planes
        default: throw std::invalid_argument("Unsupported pixel format");
    }
}

Frame::Frame(int width, int height, PixelFormat format)
    : width_(width), 
      height_(height), 
      format_(format), 
      size_(compute_size(width, height, format)),
      data_(new uint8_t[size_])
{
    // member initializer list above is the C++ way to initialize members
    // prefer it over assignment in the constructor body
}

Frame::~Frame()
{
    delete[] data_;
}