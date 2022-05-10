#include "linkedlist.h"
#include "processor.h"
#include "instruction.h"
#include "prochelpers.h"

#ifndef _LSQ_DEFINED_
#define _LSQ_DEFINED_

class LSQNode: public ReserveEntry
{
    public:
        int addr;
        LSQNode(std::string str);
        void populateSources(std::string tag, int value);
        int getAddress();
        bool isBusy();
};

class LSQueue
{
    private:
        Processor *processor;
        int capacity;
        LinkedList<LSQNode> *queue;
        int storeEntry(int addr);
        int loadEntry(int addr);
        void populateLoad(LSQNode* node, Instructions::Instruction *instrPtr);
        void populateStore(LSQNode* node, Instructions::Instruction *instrPtr);
    public:
        LSQueue();
        LSQNode* addToQueue(Instructions::Instruction *instrPtr);
        void removeFromQueue(std::string tag);
        bool isOpValid(LSQNode *node);
        bool isValidLoad(LSQNode *node);
        bool isValidStore(LSQNode *node);
        void populateTags(std::string tag, int value);
        void calculateAddrs();
        int getNumEntries();
        bool hasCapacity();
        LSQNode* getHead();
        LSQNode* getValidInstruction();
        void nextTick();
        void print();
        void flush();
        Opcodes getOpcode(std::string tag);
};

#endif