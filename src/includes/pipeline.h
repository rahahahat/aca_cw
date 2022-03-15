#include <iostream>
#include <string>
#include <vector>
#include "instruction.h"
#include <map>

#ifndef _PIPELINE_INCLUDED_
#define _PIPELINE_INCLUDED_

#include "events.h"
#include "processor.h"


namespace PipelineEvents
{
    const std::string StallPipelineEvent = "StallPipeline";
    const std::string FlushPipelineEvent = "FlushPipeline";
};

class Processor;
class PipelineLLNode;

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

enum pipelineType {
    None, Scalar
};

class Pipeline: public EventDispatcher {
    protected:
        Processor *processor;
        PipelineLL* instructions;
        PipelineLLNode *flushNode;
        static int completedInstr(Instructions::Instruction *instPtr);
        int stall;
    public:
        int flush;
        virtual void addInstructionToPipeline(Instructions::Instruction *instr) {};
        virtual void addInstructionToPipeline(int id) {};
        virtual void pipeInstructionsToProcessor() {};
        virtual void attachToProcessor(Processor *proc);
        virtual void stallPipeline() {};
        // virtual void stallPipelineOnEvent(EventBase& base) {};
        virtual void flushPipeline() {}
        virtual void flushPipelineOnEvent(const EventBase& base) {};
        virtual void flushPipelineOnBranchOrJump() {};
        virtual void resume() {};
        virtual int stalled() {};
        virtual pipelineType getType();
        int isEmpty();
        void removeCompletedInstructions();
        int getInstrSize();

};

class ScalarPipeline: public Pipeline {
    public:
        ScalarPipeline();
        virtual void addInstructionToPipeline(Instructions::Instruction *instr);
        virtual void addInstructionToPipeline(int id);
        virtual void pipeInstructionsToProcessor();
        virtual void stallPipeline();
        void stallPipelineOnEvent(const EventBase& base);
        virtual void flushPipeline();
        virtual void flushPipelineOnEvent(const EventBase& base);
        virtual void flushPipelineOnBranchOrJump();
        virtual void resume();
        virtual int stalled();
        virtual pipelineType getType();
};

#endif