#include <iostream>
#include <string>
#include "instruction.h"
#include "pipeline.h"

#ifndef _PROCESSOR_INCLUDED_
#define _PROCESSOR_INCLUDED_

class Processor{
    private:
        pipelineType pipeType;
        Pipeline *pipeline;
        FetchUnit *fUnit;
        DecodeUnit *dUnit;
    public:
        Processor(Pipeline *pipe, pipelineType type);
        int32_t registers[32];
        int32_t PC = 0;
        int32_t DataMemory[1024];
        std::string instructionMemory[512];
        int32_t instrMemSize = 0;
        void loadInstructionIntoMemory(std::string instruction);
        void attachPipeline(Pipeline *pipe, pipelineType type);
    private:
        void nonPipelinedExecution();
        void makeInstructionOnFetch();
        void decodeCurrentInstruction(Instructions::Instruction instruction);

        void fetch();
        void decode();
        void execute();
        void memref();
        void writeback();
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
        void decodeRTypeInstruction();
        void decodeITypeInstruction();
        void decodeJTypeInstruction();
        Instructions::instructionType getInstrType(Instructions::Instruction *instrPtr);
    public:
        DecodeUnit(pipelineType pipeType);
        Instructions::Instruction* decode(Instructions::Instruction *instrPtr);
};

void printInstructionMemory(Processor processor);
void printInstructionMemoryAtIndex(Processor processor, int index);

#endif