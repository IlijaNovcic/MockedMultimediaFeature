#pragma once
#include "PipelineStage.hpp"

class BlurFilter : public PipelineStage
{
    public:
        explicit BlurFilter(int radius = 1);
        void process(Frame& frame) override;
        const char* name() const override{return "BlurFilter";}
    private:
        int radius_;
};