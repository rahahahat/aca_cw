#include "procUnit.h"
#include "memoryUnit.h"
#include "processor.h"
#include "pipeline.h"
#include "termcolor.h"
#include "cdb.h"
#include "util.h"

// #################################################################################################
// OMemoryUnit
// #################################################################################################

OMemoryUnit::OMemoryUnit()
{
    busy = false;
    address = -1;
    destination = $noreg;
    store_val = 0;
    opcode = NOP;
    lsqTag = "~";
    cycle = 0;
    result = 0;
    instrStr = "";
}

void OMemoryUnit::pre()
{
    if (busy) return;
    busy = seekInstruction();
    return;
}

bool OMemoryUnit::seekInstruction()
{
    LSQNode *node = processor->getLsq()->getValidInstruction();
    if (node == NULL) return false;
    node->busy = true;
    instrStr = node->instrStr;
    if (node->opcode == LW)
    {
        opcode = LW;
        address = node->address;
        destination = node->destination;
        lsqTag = node->getTag();
    }
    if (node->opcode == SW)
    {
        opcode = SW;
        address = node->address;
        destination = $noreg;
        // TODO: Check store format
        store_val = node->value_pair.second;
        lsqTag = node->getTag();
    }
    return true;
}

void OMemoryUnit::run()
{

    pre();
    if (opcode == LW && busy) load();
    if (opcode == SW && busy) store();
    post();
    return;
}

void OMemoryUnit::load()
{
    bool print = config->debug->print;
    IF_PRINT(
        std::cout << termcolor::bold << termcolor::green << "Executing Load: " << termcolor::white << instrStr << std::endl;
    );
    if (!busy) return;
    result = processor->DataMemory[address];
    cycle--;
    return;
};

void OMemoryUnit::store()
{
    bool print = config->debug->print;
    IF_PRINT(
        std::cout << termcolor::bold << termcolor::green << "Executing Store: " << termcolor::white << instrStr << std::endl;
    );
    if (!busy) return;
    return;
};

void OMemoryUnit::post()
{
    if (!busy) return;
    busy = false;
    instrStr = "";
    if (opcode == LW)
    {
        processor->getCDB()->broadcast(destination, lsqTag, result);
        return;
    }
    if (opcode == SW)
    {
        processor->getCDB()->broadcast(address, lsqTag, store_val);
        return;
    }
    return;
}

void OMemoryUnit::nextTick()
{
    run();
}

void OMemoryUnit::cycleReset()
{
    cycle = 2;
}

void OMemoryUnit::flush()
{
    lsqTag = "";
    opcode = NOP;
    store_val = 0;
    address = 0;
    destination = $noreg;
    cycle = 0;
    result = 0;
}