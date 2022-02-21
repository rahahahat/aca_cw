#include <iostream>
#include <string>
#include <vector>
#include "instruction.h"
#include <map>

#ifndef _PIPELINE_INCLUDED_
#define _PIPELINE_INCLUDED_
#include "processor.h"

class Processor;
class PipelineLLNode;

class PipelineLL {
    public:
        PipelineLL();
        PipelineLLNode* head;
        PipelineLLNode* tail;
        int size;
        Instructions::Instruction* pop();
        void push(Instructions::Instruction *instrPtr);
        Instructions::Instruction* remove(int i);
        void removeAndDestroy(int i);
        Instructions::Instruction* addInstructionForFetch();
        void flushCompletedInstructions();
};

class PipelineLLNode {
    public:
        PipelineLLNode();
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

class Pipeline {
    protected:
        Processor *processor;
        std::vector<Instructions::Instruction*> instructions;
        static int completedInstr(Instructions::Instruction *instPtr);
        int stall;
    public:
        virtual void addInstructionToPipeline(Instructions::Instruction *instr, int id) {};
        virtual void pipeInstructionsToProcessor() {};
        virtual void attachToProcessor(Processor *proc);
        virtual void stallPipeline() {};
        virtual void resume() {};
        virtual int stalled() {};
        virtual pipelineType getType();
        int isEmpty();
        void removeCompletedInstructions();
        int getInstrSize();
        std::vector<Instructions::Instruction*> getInstructions() { return instructions; };
};

class ScalarPipeline: public Pipeline {
    public:
        ScalarPipeline();
        virtual void addInstructionToPipeline(Instructions::Instruction *instr, int id);
        virtual void pipeInstructionsToProcessor();
        virtual void stallPipeline();
        virtual void resume();
        virtual int stalled();
        virtual pipelineType getType();
};

#endif