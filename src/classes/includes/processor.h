#include <iostream>
#include <string>
#include "instruction.h"
#include "pipeline.h"
#include "util.h"


#ifndef _PROCESSOR_INCLUDED_
#define _PROCESSOR_INCLUDED_
#include "parser.h"

class DecodeUnit;
class FetchUnit;
class ExecuteUnit;
class Parser;

class Processor{
    private:
        pipelineType pipeType;
        Pipeline *pipeline;
        FetchUnit *fUnit;
        DecodeUnit *dUnit;
        ExecuteUnit *eUnit;
        Parser *parser;
        void nonPipelinedExecution();
    public:
        int32_t registers[32];
        int32_t PC = 0;
        int32_t DataMemory[1024];
        std::string instructionMemory[512];
        int32_t instrMemSize = 0;
        Processor(Pipeline *pipe, pipelineType type);
        void loadInstructionIntoMemory(std::string instruction);
        void attachPipeline(Pipeline *pipe, pipelineType type);
        // should be private or refactored in run()
        void loadProgram(std::string fn);
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
        DecodeUnit(pipelineType type);
        void decode(Instructions::Instruction *instrPtr);
};

class ExecuteUnit {
    private:
        pipelineType pipeType;
        Processor *processor;
        void executeRTypeInstruction(Instructions::Instruction *instrPtr);
        void executeITypeInstruction(Instructions::Instruction *instrPtr);
        void executeJTypeInstruction(Instructions::Instruction *instrPtr);
    public:
        ExecuteUnit(Processor *procPtr, pipelineType type);
        void execute(Instructions::Instruction *instrPtr);
};


void printInstructionMemory(Processor processor);
void printInstructionMemoryAtIndex(Processor processor, int index);
void printClock(int clock);

#endif