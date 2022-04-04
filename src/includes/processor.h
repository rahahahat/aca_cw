#include <iostream>
#include <map>
#include <utility>
#include <string>
#include "instruction.h"
#include "util.h"
#include "json.hpp"
#include "prochelpers.h"
#include "config.h"

#ifndef _PROCESSOR_INCLUDED_
#define _PROCESSOR_INCLUDED_

#include "parser.h"
#include "pipeline.h"
#include "events.h"
#include "procUnit.h"
#include "lsq.h"

enum ProcUnitTypes
{
    FETCHUNIT, DECODEUNIT, EXECUTEUNIT, MEMORYUNIT
};


class Pipeline;
class Parser;
class ProcUnit;
class Scoreboard;
namespace rs 
{
    class ReservationStation;
};

class Processor: public EventDispatcher
{
    private:
        // {ProcUnit, std::pair<init,int>(total_units, available_units)} (Execute, MemoryAccess and WriteBack units)
        std::map<ProcUnitTypes, std::pair<int, int>*> num_proc_units;
        Pipeline *pipeline;
        Parser *parser;
        std::map<ProcUnitTypes, ProcUnit*> proc_units;
        rs::ReservationStation* reservation_station;
        LSQueue *lsq;
        Scoreboard *scoreboard;

        void fetch(Instructions::Instruction *instrPtr);
        void decode(Instructions::Instruction *instrPtr);
        void execute(Instructions::Instruction *instrPtr);
        void memref(Instructions::Instruction *instrPtr);
        void writeback(Instructions::Instruction *instrPtr);
        void resetProcResources();
        
        
    public:
        static Processor* getProcessorInstance()
        {
            static Processor* instance;
            if (instance == NULL)
            {
                instance = new Processor();
            }
            return instance;
        }
        int clock;
        static Processor* fabricate();
        static void destroy(Processor *processor);
        // ResultForwarder *resultForwarder;
        std::map<std::string, int> labelMap;
        int32_t registers[32];
        int32_t PC = 0;
        int32_t DataMemory[1024];
        std::string instructionMemory[512];
        int32_t instrMemSize = 0;
        Processor();
        void loadInstructionIntoMemory(std::string instruction);
        void attachPipeline(Pipeline *pipe);
        // void attachProcHelper(ResultForwarder *rf);
        void attachProcHelper(Scoreboard *sb);
        void runProgram();
        void loadProgram(std::string fn);
        void runInstr(Instructions::Instruction *instr);
        void regDump();
        Pipeline* getPipeline();
        ProcUnit* getProcUnit(ProcUnitTypes unit);
        LSQueue* getLsq();
        rs::ReservationStation* getRS();
        Scoreboard* getSB();
};



void printInstructionMemory(Processor processor);
void printInstructionMemoryAtIndex(Processor processor, int index);
void printClock(int clock);

#endif