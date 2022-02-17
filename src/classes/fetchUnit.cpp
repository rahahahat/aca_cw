#include "processor.h"

FetchUnit::FetchUnit(Processor *proc, Pipeline *pl ,pipelineType tp)
{
    processor = proc;
    pipeType = tp;
    pipeline = pl;
    return;
};

Instructions::Instruction* FetchUnit::fetch()
{
    switch(pipeType) {
        case Scalar:
            return NULL;
        default:
           Instructions::Instruction *ptr = NULL;
           nonPipelinedFetch(ptr);
           return ptr;
    }
    return NULL;
};

void FetchUnit::nonPipelinedFetch(Instructions::Instruction *instrPtr)
{
    std::string instr = processor->instructionMemory[processor->PC];
    Instructions::Instruction instruction = Instructions::Instruction(instr);
    instrPtr = &instruction;
    processor->PC++;
    return;
};
