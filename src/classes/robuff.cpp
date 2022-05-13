#include "robuff.h"
#include "cdb.h"
#include "util.h"
#include "termcolor.h"
#include "branch.h"

// #################################################################################################
// ROBEntry
// #################################################################################################

ROBEntry::ROBEntry(std::string tag_name)
{
    tag = tag_name;
    valid = false;
    brpred = false;
    opcode = NOP;
    instr = NULL;
    instrStr = "";
    value =0;
    sw_addr = -1;
    isBranch = false;
    destination = $noreg;
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
    opcode = instrPtr->opcode;
    isBranch = isOpBranch(opcode);
    brpred = instrPtr->pred;
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
    cpc = getConfig()->units->commit;
    max_size = 128;
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

Instructions::Instruction* ROBEntry::getInsruction()
{
    return instr;
}

ROBEntry* ReorderBuffer::pop()
{
    bool print = getConfig()->debug->print;
    if (buffer->head == NULL) return NULL;
    ROBEntry *entry = buffer->head->payload;

    if (entry->isValid()) 
    {
        if (entry->isBranch)
        {
            // processor->getPredictor()->incrementCount();
            processor->getPredictor()->update(entry->getInstrStr(), entry->getInsruction()->fetched_at_pc, entry->brpred == entry->getValue());
            if (entry->brpred != entry->getValue())
            {
                IF_PRINT(
                    std::cout << termcolor::on_bright_red << termcolor::bold
                    << "Branch Misprediction: " << entry->getInstrStr() << " - Flushing Pipeline." << termcolor::reset << std::endl;
                );
                flush(buffer->head);
                return NULL;
            }
        }
        return buffer->pop();
    }
    return NULL;
};
void ReorderBuffer::nextTick()
{
    commitHead();
};
void ReorderBuffer::commitHead()
{
    
    for (int x = 0; x < cpc; x++)
    {
        ROBEntry* entry = pop();
        if (entry == NULL) return;
        if (isOpBranch(entry->opcode))
        {
            processor->getCDB()->commit($noreg, entry->getTag(), entry->getValue(), entry->getInstrStr());
        }
        else if (entry->opcode == SW)
        {
            processor->getCDB()->commitToMemory(entry->sw_addr, entry->getTag(), entry->getValue(), entry->getInstrStr());
        }
        else
        {
            processor->getCDB()->commit(entry->getDestination(), entry->getTag(), entry->getValue(), entry->getInstrStr());
        }
    }
};

int ReorderBuffer::getSize()
{
    return buffer->size;
}

void ReorderBuffer::flush(LLNode<ROBEntry> *flush_from)
{

    // TODO: FLUSH entry matching with the tag and all all after it.
    LLNode<ROBEntry>* curr = flush_from;
    int pc_value = flush_from->payload->getValue();
    while(curr != NULL)
    {
        LLNode<ROBEntry>* next = curr->next;
        buffer->removeAndDestroy(curr);
        curr = next;
    }
    processor->getCDB()->flushAll(pc_value);
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

void ReorderBuffer::populateEntry(std::string tag, int value, int mem_addr)
{
    LLNode<ROBEntry> *node = buffer->head;
    while(node != NULL)
    {
        ROBEntry *entry = node->payload;
        if (entry->getTag().compare(tag) == 0)
        {
            entry->setValue(value);
            entry->sw_addr = mem_addr;
            entry->validate();
            return;
        }
        node = node->next;
    }
    return;
};

void ReorderBuffer::print()
{
    LLNode<ROBEntry> *curr = buffer->head;
    
    std::cout
    << "\n"
    << termcolor::bright_green
    << "|"
    << "Tag\t\t|"
    << "Instr\t\t|"
    << "Valid\t|"
    << "Value\t\t|"
    << termcolor::reset
    << std::endl;

    while(curr != NULL)
    {
        ROBEntry* entry = curr->payload;
        std::cout
        << termcolor::bright_blue
        << "|"
        << entry->getTag()
        << "\t\t|"
        << entry->getInstrStr()
        << "\t| "
        << entry->isValid()
        << "\t|"
        << entry->getValue()
        << "\t\t| "
        << termcolor::reset
        << std::endl;
        curr = curr->next;
    }
    std::cout << std::endl;
}