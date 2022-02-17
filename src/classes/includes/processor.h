#include <iostream>
#include <map>
#include <utility>
#include <string>
#include "instruction.h"
#include "pipeline.h"
#include "util.h"


#ifndef _PROCESSOR_INCLUDED_
#define _PROCESSOR_INCLUDED_
#include "parser.h"
#include "pipelineUnits.h"

class DecodeUnit;
class FetchUnit;
class ExecuteUnit;
class MemRefUnit;
class WriteBackUnit;

class Parser;

class Processor{
    private:
        pipelineType pipeType;
        Pipeline *pipeline;
        FetchUnit *fUnit;
        DecodeUnit *dUnit;
        ExecuteUnit *eUnit;
        MemRefUnit *mrUnit;
        WriteBackUnit *wbUnit;
        Parser *parser;
        void nonPipelinedExecution();
    public:
        std::map<std::string, int> labelMap;
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



void printInstructionMemory(Processor processor);
void printInstructionMemoryAtIndex(Processor processor, int index);
void printClock(int clock);

#endif