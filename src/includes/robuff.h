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
    public:
        ROBEntry(std::string tag_name, std::string instr_str);
        void validate();
        void setInstruction(Instructions::Instruction *instrPtr);
        void setValue(int val);
        std::string getInstrStr();
        std::string getTag();
        bool isValid();
};

class ReorderBuffer
{
    private:
        int max_size;
        LinkedList<ROBEntry> *buffer;
        Processor *processor;
        void commit();
    public:
        ReorderBuffer(int size);
        void addEntry(std::string tag_name, std::string instr_str);
        void pop();
        void nextTick();
        void flush(std::string tag);
};

#endif