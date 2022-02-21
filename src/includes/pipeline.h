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
        static int completedInstr(Instructions::Instruction *instPtr);
        int stall;
    public:
        virtual void addInstructionToPipeline(Instructions::Instruction *instr) {};
        virtual void pipeInstructionsToProcessor() {};
        virtual void attachToProcessor(Processor *proc);
        virtual void stallPipeline() {};
        virtual void resume() {};
        virtual int stalled() {};
        virtual pipelineType getType();
        int isEmpty();
        void removeCompletedInstructions();
        int getInstrSize();
        std::vector<Instructions::Instruction*> getInstructions() { return instructions; };
};

class ScalarPipeline: public Pipeline {
    public:
        ScalarPipeline();
        virtual void addInstructionToPipeline(Instructions::Instruction *instr);
        virtual void pipeInstructionsToProcessor();
        virtual void stallPipeline();
        virtual void resume();
        virtual int stalled();
        virtual pipelineType getType();
};

#endif