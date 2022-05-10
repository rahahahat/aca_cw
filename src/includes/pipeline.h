#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "instruction.h"

#ifndef _PIPELINE_INCLUDED_
#define _PIPELINE_INCLUDED_

#include "events.h"
#include "pipestage.h"


class Processor;
class PipelineLLNode;
class ProcUnit;
class ODecodeUnit;
class OMemoryUnit;
class OExecuteUnit;

enum ProcUnitTypes
{
    FETCHUNIT, DECODEUNIT, EXECUTEUNIT, MEMORYUNIT
};

enum pipelineType {
    None, Scalar
};

enum StallSource {
    NoSrc, Branch, RS, ROB, Halt, Flush
};

class Pipeline 
{
    protected:
        Processor *processor;
        StallSource stalled_by;
        bool stall;
    public:
        Pipeline();
        virtual void pipeInstructionsToProcessor();
        virtual void stallPipeline(StallSource by);
        virtual void resumePipeline(StallSource);
        virtual void resumePipelineByForce();
        virtual int stalled();
        virtual pipelineType getType();
        virtual bool areAllProcUnitsFree() {return true;};
        virtual void nextTick(int cycle) { return; };
        virtual void flush() { return; };
        StallSource stalledBy() { return stalled_by; };
};

class OoOPipeline: public Pipeline
{
    std::vector<ODecodeUnit*> dn;
    std::vector<OExecuteUnit*> en;
    std::vector<OMemoryUnit*> mn;
    std::map<ProcUnitTypes, std::pair<int, int>*> num_proc_units;
    private:
        void issueTick();
        void execTick();
        void memTick();
        void lsqTick();
        void robuffTick();
        void printSB();
        void post();
    public:
        OoOPipeline();
        virtual void pipeInstructionsToProcessor();
        virtual void nextTick(int cycle);
        virtual bool areAllProcUnitsFree();
        void flush();
};

#endif