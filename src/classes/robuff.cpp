#include "robuff.h"
#include "cdb.h"

// #################################################################################################
// ROBEntry
// #################################################################################################

ROBEntry::ROBEntry(std::string tag_name)
{
    tag = tag_name;
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
    destination = instrPtr->getDestination();
    instrStr = instr->instrString;
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
int ROBEntry::getValue()
{
    return value;
};
Register ROBEntry::getDestination()
{
    return destination;
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
void ReorderBuffer::addEntry(std::string tag_name, Instructions::Instruction *instrPtr)
{
    ROBEntry *entry = new ROBEntry(tag_name);
    entry->setInstruction(instrPtr);
    buffer->add(entry);
    return;
};
ROBEntry* ReorderBuffer::pop()
{
    if (buffer->head == NULL) return NULL;
    ROBEntry *entry = buffer->head->payload;
    if (entry->isValid()) return buffer->pop();
    return NULL;
};
void ReorderBuffer::nextTick()
{
    commitHead();
};
void ReorderBuffer::commitHead()
{
    ROBEntry* entry = pop();
    while(entry != NULL)
    {
        processor->getCDB()->commit(entry->getDestination(), entry->getTag(), entry->getValue());
        entry = pop();
    }
    return;
};
void ReorderBuffer::flush(std::string tag)
{
    // TODO: FLUSH entry matching with the tag and all all after it.
    return;
};
void ReorderBuffer::populateEntry(std::string tag, int value)
{
    LLNode<ROBEntry> *node = buffer->head;
    while(node != NULL)
    {
        ROBEntry *entry = node->payload;
        if (entry->getTag().compare(tag) == 0)
        {
            entry->setValue(value);
            entry->validate();
            return;
        }
        node = node->next;
    }
    return;
};
void ReorderBuffer::print()
{
    std::cout << "ROBUFF" << std::endl;
    LLNode<ROBEntry> *curr = buffer->head;
    while(curr != NULL)
    {
        ROBEntry* entry = curr->payload;
        std::cout
        << termcolor::red << termcolor:: bold
        << "Tag: "
        << entry->getTag()
        << termcolor::reset
        << termcolor:: green << termcolor:: bold
        << " | "
        << "instr: "
        << entry->getInstrStr()
        << " | "
        << "valid: "
        << entry->isValid()
        << " | "
        << "value: "
        << entry->getValue()
        << " | "
        << termcolor::reset
        << std::endl;
        curr = curr->next;
    }
}