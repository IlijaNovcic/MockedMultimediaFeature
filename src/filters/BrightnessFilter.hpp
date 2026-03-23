#pragma once
#include "PipelineStage.hpp"

class BrightnessFilter : public PipelineStage
{
    public:
        explicit BrightnessFilter(float factor = 1.0f); 
        void process(Frame& frame) override;
        const char* name() const override { return "BrightnessFilter"; }
    private:
        float factor_; // >1.0 to brighten, <1.0 to darken
};