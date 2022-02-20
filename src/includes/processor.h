#include <iostream>
#include <map>
#include <utility>
#include <string>
#include "instruction.h"
#include "util.h"
#include "prochelpers.h"


#ifndef _PROCESSOR_INCLUDED_
#define _PROCESSOR_INCLUDED_
#include "parser.h"
#include "procUnits.h"
#include "pipeline.h"

class DecodeUnit;
class FetchUnit;
class ExecuteUnit;
class MemRefUnit;
class WriteBackUnit;
class Pipeline;
class Parser;

class Processor
{
    private:
        Pipeline *pipeline;
        DecodeUnit *dUnit;
        ExecuteUnit *eUnit;
        MemRefUnit *mrUnit;
        WriteBackUnit *wbUnit;
        Parser *parser;

        void fetch(Instructions::Instruction *instrPtr);
        void decode(Instructions::Instruction *instrPtr);
        void execute(Instructions::Instruction *instrPtr);
        void memref(Instructions::Instruction *instrPtr);
        void writeback(Instructions::Instruction *instrPtr);

    public:
        FetchUnit *fUnit;
        ResultForwarder *resultForwarder;
        Scoreboard *scoreboard;
        std::map<std::string, int> labelMap;
        int32_t registers[32];
        int32_t PC = 0;
        int32_t DataMemory[1024];
        std::string instructionMemory[512];
        int32_t instrMemSize = 0;
        Processor(Pipeline *pipe, Scoreboard *sb, ResultForwarder *rf);
        void loadInstructionIntoMemory(std::string instruction);
        void attachPipeline(Pipeline *pipe);

        void attachProcUnit(FetchUnit *pu);
        void attachProcUnit(DecodeUnit *pu);
        void attachProcUnit(ExecuteUnit *pu);
        void attachProcUnit(MemRefUnit *pu);
        void attachProcUnit(WriteBackUnit *pu);

        void loadProgram(std::string fn);
        void runInstr(Instructions::Instruction *instr);
};



void printInstructionMemory(Processor processor);
void printInstructionMemoryAtIndex(Processor processor, int index);
void printClock(int clock);

#endif