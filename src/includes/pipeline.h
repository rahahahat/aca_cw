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

class PipelineLL {
    public:
        PipelineLL();
        PipelineLLNode* head;
        PipelineLLNode* tail;
        int size;
        Instructions::Instruction* pop();
        void add(Instructions::Instruction *instrPtr);
        Instructions::Instruction* remove(int i);
        Instructions::Instruction* remove(PipelineLLNode* pl_node);
        void removeAndDestroy(int i);
        void removeAndDestroy(PipelineLLNode* pl_node);
        Instructions::Instruction* addInstructionForFetch();
        Instructions::Instruction* addInstructionForFetch(int id);
        void flushCompletedInstructions();
        void flushAfterNode(PipelineLLNode *node);
};

class PipelineLLNode {
    public:
        PipelineLLNode();
        PipelineLLNode(int id);
        PipelineLLNode(Instructions::Instruction *instrPtr);
        Instructions::Instruction* payload;
        PipelineLLNode* next;
        PipelineLLNode* prev;
        int isTail();
        int isHead();
        Instructions::Instruction* get();
};

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
    protected:
        Processor *processor;
        StallSource stalled_by;
        bool stall;
    public:
        Pipeline();
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

class ScalarPipeline: public Pipeline {
    private:
        FetchUnit* fn;
        ScalarDecodeUnit* dn;
        ScalarExecuteUnit* en;
        ScalarMemoryUnit* mn;

        PipelineLL* instructions;
        PipelineLLNode *flushNode;
        void addInstructionToPipeline(Instructions::Instruction *instr);
        void addInstructionToPipeline(int id);
        void removeCompletedInstructions();
    public:
        ScalarPipeline();
        void flushPipelineOnBranchOrJump();
        int pipelineSize();
        void nextTick(int cycle);
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