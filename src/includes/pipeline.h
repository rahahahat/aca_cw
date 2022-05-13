#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "instruction.h"

#ifndef _PIPELINE_INCLUDED_
#define _PIPELINE_INCLUDED_

#include "events.h"
#include "pipestage.h"
#include "linkedlist.h"

class Processor;
class PipelineLLNode;
class ProcUnit;
class ODecodeUnit;
class OMemoryUnit;
class ScalarDecodeUnit;
class ScalarExecuteUnit;
class ScalarMemoryUnit;
class OExecuteUnit;
class FetchUnit;

enum ProcUnitTypes
{
    FETCHUNIT, DECODEUNIT, EXECUTEUNIT, MEMORYUNIT
};

enum pipelineType {
    None, Scalar, SuperScalar
};

enum StallSource {
    NoSrc, Branch, Capacity ,Halt, Flush, Sources
};

class Pipeline 
{
    std::map<StallSource, int> stallMap;
    protected:
        Processor *processor;
        StallSource stalled_by;
        bool stall;
    public:
        Pipeline();
        virtual void printStalls();
        virtual void removeCompletedInstructions() { return; };
        virtual void pipeInstructionsToProcessor() { return; };
        virtual void nextTick(int cycle) { return; };
        virtual void flush() { return; };
        StallSource stalledBy() { return stalled_by; };
        virtual bool areAllProcUnitsFree() {return true;};
        virtual void stallPipeline(StallSource by);
        virtual void resumePipeline(StallSource);
        virtual void resumePipelineByForce();
        virtual int stalled();
        virtual int pipelineSize() { return 0; };

        virtual LinkedList<Instructions::Instruction>* getInstrQ() { return NULL; };
};


class OoOPipeline: public Pipeline
{
    std::vector<ODecodeUnit*> dn;
    std::vector<OExecuteUnit*> en;
    std::vector<OMemoryUnit*> mn;
    std::map<ProcUnitTypes, std::pair<int, int>*> num_proc_units;
    LinkedList<Instructions::Instruction>* instrQ;

    private:
        void issueTick();
        void execTick();
        void memTick();
        void lsqTick();
        void fetchTick();
        void decodeTick();
        void robuffTick();
        void printSB();
        void post();
    public:
        OoOPipeline();
        virtual void pipeInstructionsToProcessor();
        virtual void nextTick(int cycle);
        virtual bool areAllProcUnitsFree();
        void flush();
        LinkedList<Instructions::Instruction>* getInstrQ();
};

#endif