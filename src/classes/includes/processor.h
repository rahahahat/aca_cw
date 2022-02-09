#include <iostream>
#include <string>
#include "instruction.h"
#include "pipeline.h"
#include "util.h"

#ifndef _PROCESSOR_INCLUDED_
#define _PROCESSOR_INCLUDED_

class DecodeUnit;
class FetchUnit;

class Processor{
    private:
        pipelineType pipeType;
        Pipeline *pipeline;
        FetchUnit *fUnit;
        DecodeUnit *dUnit;
    public:
        int32_t registers[32];
        int32_t PC = 0;
        int32_t DataMemory[1024];
        std::string instructionMemory[512];
        int32_t instrMemSize = 0;
        Processor(Pipeline *pipe, pipelineType type);
        void loadInstructionIntoMemory(std::string instruction);
        void attachPipeline(Pipeline *pipe, pipelineType type);
        void runProgram();
    private:
        void nonPipelinedExecution();
};

class FetchUnit {
    private:
        Processor *processor;
        void nonPipelinedFetch(Instructions::Instruction *instrPtr);
        pipelineType pipeType;
    public:
        FetchUnit(Processor *processor, pipelineType type);
        Instructions::Instruction* fetch();
};

class DecodeUnit {
    private:
        pipelineType pipeType;
        void decodeRTypeInstruction(Instructions::Instruction *instrPtr, std::vector<std::string> splitInstr);
        void decodeITypeInstruction(Instructions::Instruction *instrPtr, std::vector<std::string> splitInstr);
        void decodeJTypeInstruction(Instructions::Instruction *instrPtr, std::vector<std::string> splitInstr);
        InstructionType getInstrType(Instructions::Instruction *instrPtr);
    public:
        DecodeUnit(pipelineType pipeType);
        void decode(Instructions::Instruction *instrPtr);
};

void printInstructionMemory(Processor processor);
void printInstructionMemoryAtIndex(Processor processor, int index);
void printClock(int clock);

#endif