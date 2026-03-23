#pragma once
#include "Frame.hpp"

// Base class for all pipeline stages
class PipelineStage
{
    public:
    // Process a frame — to be implemented by derived classes
    virtual void process(Frame& frame) = 0; // Pure virtual function makes this an abstract class

    // Every stage can have a name for logging/debugging
    virtual const char* name() const = 0; // Pure virtual function makes this an abstract class

    // Polymorphic base class should have virtual destructor
    virtual ~PipelineStage() = default; 
};