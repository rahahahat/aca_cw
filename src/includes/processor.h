#include <iostream>
#include <map>
#include <utility>
#include <string>
#include "instruction.h"
#include "util.h"
#include "json.hpp"
#include "config.h"

#ifndef _PROCESSOR_INCLUDED_
#define _PROCESSOR_INCLUDED_

#include "parser.h"
#include "pipeline.h"
#include "events.h"
#include "procUnit.h"



class Pipeline;
class Parser;
class ProcUnit;
class Scoreboard;
class CommonDataBus;
class LSQueue;
// class CDB;

namespace rs 
{
    class ReservationStation;
};

class Processor: public EventDispatcher
{
    private:
        
        
        Pipeline *pipeline;
        Parser *parser;
        rs::ReservationStation* reservation_station;
        LSQueue *lsq;
        Scoreboard *scoreboard;
        CommonDataBus *cdb;
        bool progEnded;
        
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
        Processor* fabricate();
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

        // void attachProcHelper(ResultForwarder *rf);
        
        void runProgram();
        void loadProgram(std::string fn);
        void regDump();
        void stepMode();
        bool programEnded();
        void setProgramEnded();

        void attachParser(Parser *parser);
        void attachPipeline(Pipeline *pipe);
        void attachProcHelper(Scoreboard *sb);
        void attachProcHelper(rs::ReservationStation* rs);
        void attachLSQ(LSQueue  *queue);
        void attachCDB(CommonDataBus *bus);

        Pipeline* getPipeline();
        LSQueue* getLsq();
        rs::ReservationStation* getRS();
        Scoreboard* getSB();
        CommonDataBus* getCDB();
};



void printInstructionMemory(Processor processor);
void printInstructionMemoryAtIndex(Processor processor, int index);

#endif