#pragma once
#include "Frame.hpp"
#include <string_view>

// Abstract base class for all pipeline stages
// T is the pixel type — uint8_t, float, etc.
template<typename T>
class PipelineStage
{
    public:
    // Process a frame — to be implemented by derived classes
    virtual void process(Frame<T>& frame) = 0; // Pure virtual function makes this an abstract class

    // Every stage can have a name for logging/debugging
    virtual std::string_view name() const = 0; // Pure virtual function makes this an abstract class

    // Polymorphic base class should have virtual destructor
    virtual ~PipelineStage() = default; 
};