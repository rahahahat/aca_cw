#include <iostream>
#include <string>
#include <vector>
#include "instruction.h"

#ifndef _PIPELINE_INCLUDED_
#define _PIPELINE_INCLUDED_
#include "processor.h"

class Processor;

enum pipelineType {
    None, Scalar
};

class Pipeline {
    protected:
        Processor *processor;
        std::vector<Instructions::Instruction*> instructions;
    public:
        virtual void addInstructionToPipeline(Instructions::Instruction *instr) {};
        virtual void pipeInstructionsToProcessor() {};
        virtual void attachToProcessor(Processor *proc);
        virtual pipelineType getType();
};

class ScalarPipeline: public Pipeline {
    public:
        ScalarPipeline();
        virtual void addInstructionToPipeline(Instructions::Instruction *instr);
        virtual void pipeInstructionsToProcessor();
        virtual pipelineType getType();
};

#endif