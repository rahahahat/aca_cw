#include "linkedlist.h"
#include "isa.h"
#include "instruction.h"
#include "processor.h"
#include <string>

#ifndef _REORDER_BUFFER_DEFINED_
#define _REORDER_BUFFER_DEFINED_

class ROBEntry
{
    private:
        Instructions::Instruction *instr;
        std::string tag;
        std::string instrStr;
        bool valid;
        int value;
        Register destination;

    public:
        Opcodes opcode;
        bool isBranch;
        int brpred;
        int sw_addr;
        ROBEntry(std::string tag_name);
        void validate();
        void setInstruction(Instructions::Instruction *instrPtr);
        void setValue(int val);
        std::string getInstrStr();
        std::string getTag();
        Register getDestination();
        int getValue();
        bool isValid();
};

class ReorderBuffer
{
    private:
        int max_size;
        int cpc;
        LinkedList<ROBEntry> *buffer;
        Processor *processor;
        void commitHead();
    public:
        ReorderBuffer(int size);
        void addEntry(std::string tag_name, Instructions::Instruction *instrPtr);
        ROBEntry* pop();
        void nextTick();
        void flush(LLNode<ROBEntry> *entry);
        void populateEntry(std::string tag, int value);
        void populateEntry(std::string tag, int value, int mem_addr);
        void print();
        int getSize();
};

#endif