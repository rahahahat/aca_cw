#include "linkedlist.h"
#include "processor.h"
#include "instruction.h"
#include "events.h"

#ifndef _LSQ_DEFINED_
#define _LSQ_DEFINED_

class LSQNode
{
    private:
        int entry_number;
        Instructions::Instruction *instr;
    public:
        LSQNode(Instructions::Instruction *instrPtr, int entry_num);
        Instructions::Instruction* getInstruction();
        int getEntryNumber();
        int getAddress();
};

class LSQueue
{
    private:
        Processor *processor;
        int total_entries;
        LinkedList<LSQNode> *queue;
        int storeEntry(int addr);
        int loadEntry(int addr);
        void addInstructionOnEvent(const EventBase& base);

    public:
        LSQueue();
        void addToQueue(Instructions::Instruction *instrPtr);
        void removeFromQueue(Instructions::Instruction *instrPtr);
        bool isOpValid(Instructions::Instruction *instrPtr);
};

#endif