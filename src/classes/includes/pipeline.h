#include <iostream>
#include <string>
#include <vector>
#include "instruction.h"

#ifndef _PIPELINE_INCLUDED_
#define _PIPELINE_INCLUDED_
class Processor;

enum pipelineType: int {
    None, Scalar
};

class Pipeline {
    protected:
        std::vector<Instructions::Instruction> instructions;
    public:
        virtual void addInstructionToPipeline(Instructions::Instruction instr);
        virtual std::vector<Instructions::Instruction> getInstructionsInPipeline();
};

// class ScalarPipeline: public Pipeline {
//     public:
//         void addInstructionToPipeline(Instructions::Instruction instr);
// };

#endif