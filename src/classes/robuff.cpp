#include "robuff.h"

// #################################################################################################
// ROBEntry
// #################################################################################################

ROBEntry::ROBEntry(std::string tag_name, std::string instr_str)
{
    tag = tag_name;
    instrStr = instr_str;
    valid = false;
    return;
};
void ROBEntry::validate()
{
    valid = true;
    return;
};
void ROBEntry::setInstruction(Instructions::Instruction *instrPtr)
{
    instr = instrPtr;
    return;
};
void ROBEntry::setValue(int val)
{
    value = val;
    return;
};
std::string ROBEntry::getTag()
{
    return tag;
};
std::string ROBEntry::getInstrStr()
{
    return instrStr;
};
bool ROBEntry::isValid()
{
    return valid;
};

// #################################################################################################
// ReorderBuffer
// #################################################################################################

ReorderBuffer::ReorderBuffer(int size)
{
    max_size = size;
    buffer = new LinkedList<ROBEntry>();
    processor = Processor::getProcessorInstance();
    return;
};
void ReorderBuffer::addEntry(std::string tag_name, std::string instr_str)
{
    ROBEntry *entry = new ROBEntry(tag_name, instr_str);
    buffer->add(entry);
    return;
};
void ReorderBuffer::pop()
{
    ROBEntry *entry = buffer->head->payload;
    if (entry->isValid()) buffer->pop();
    return;
};

void ReorderBuffer::nextTick()
{

};

