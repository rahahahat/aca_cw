#include <iostream>
#include <map>
#include <utility>
#include <string>
#include "instruction.h"
#include "json.hpp"
#include "config.h"

#ifndef _PROCESSOR_INCLUDED_
#define _PROCESSOR_INCLUDED_

#include "parser.h"
#include "pipeline.h"
#include "events.h"
#include "procUnit.h"
#include <set>



class Pipeline;
class Parser;
class ProcUnit;
class Scoreboard;
class CommonDataBus;
class LSQueue;
class ReorderBuffer;
class BranchTargetBuffer;
class BranchPredictor;

namespace rs 
{
    class ReservationStation;
};

class Processor
{
    private:
        
        Processor();
        
        Pipeline *pipeline;
        Parser *parser;
        rs::ReservationStation* reservation_station;
        LSQueue *lsq;
        Scoreboard *scoreboard;
        CommonDataBus *cdb;
        ReorderBuffer *robuff;
        BranchPredictor* predictor;

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
        Processor* fabricate();
        static void destroy(Processor *processor);
        
        int clock;
        std::map<std::string, int> labelMap;
        std::map<std::string, int> var_map;
        int32_t registers[32];
        int32_t PC = 0;

        int total_commits = 0;
        
        #ifndef DMSIZE
        int32_t DataMemory[4000];
        int dataMemSize = 4000;
        #endif
        #ifdef DMSIZE
        int32_t DataMemory[DMSIZE];
        int dataMemSize = DMSIZE;
        #endif

        std::string instructionMemory[512];
        int32_t instrMemSize = 0;
        int32_t dataMemoryIndex = 0;

        void loadInstructionIntoMemory(std::string instruction);
        void runProgram();
        void runSScalar();
        void loadProgram(std::string fn);
        void loadDataMemory();
        void dumpDataMemory();
        void regDump();
        void printLabelMap();
        void stepMode();
        bool programEnded();
        void setProgramEnded();
        bool canIssue();

        void attachParser(Parser *parser);
        void attachPipeline(Pipeline *pipe);
        void attachProcHelper(Scoreboard *sb);
        void attachProcHelper(rs::ReservationStation* rs);
        void attachLSQ(LSQueue  *queue);
        void attachCDB(CommonDataBus *bus);
        void attachReorderBuffer(ReorderBuffer *rb);
        void attachBranchPredictor(BranchPredictor* bpr);

        void dumpConvImage();

        Pipeline* getPipeline();
        LSQueue* getLsq();
        rs::ReservationStation* getRS();
        Scoreboard* getSB();
        CommonDataBus* getCDB();
        ReorderBuffer* getRB();
        BranchPredictor* getPredictor();
};


// Extras
void printInstructionMemory(Processor *processor);
void printInstructionMemoryAtIndex(Processor processor, int index);

#endif