#include "procUnit.h"
#include "memoryUnit.h"
#include "processor.h"
#include "pipeline.h"
#include "cdb.h"

// void MemoryUnit::load(Instructions::Instruction *instrPtr)
// {
//     instrPtr->temp = processor->DataMemory[instrPtr->immediateOrAddress];
//     return;
// };

// void MemoryUnit::store(Instructions::Instruction *instrPtr)
// {
//     processor->DataMemory[instrPtr->temp] = processor->registers[instrPtr->rt];
//     return;
// };

// void MemoryUnit::writeback(Instructions::Instruction *instrPtr)
// {
//     if (isInstrBranch(instrPtr))
//     {
//         processor->PC = instrPtr->immediateOrAddress;
//         return;
//     }

//     switch(instrPtr->type)
//     {
//         case IType:
//             processor->registers[instrPtr->rt] = instrPtr->temp;
//             break;
//         case RType:
//             processor->registers[instrPtr->rd] = instrPtr->temp;
//             break;
//         case JType:
//             processor->PC = instrPtr->immediateOrAddress;
//             break;
//     }
//     return;
// }

OMemoryUnit::OMemoryUnit()
{
    busy = false;
    address = -1;
    destination = $noreg;
    store_val = 0;
    opcode = NOP;
    lsqTag = "~";
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
    // std::string stage;

    // std::cout << termcolor::bold << termcolor::blue
    // << "Memory Accessing instructions: " 
    // << instrPtr->instrString
    // << " (" << stage << ")"
    // << " (" << instrPtr->tag << ")" 
    // << termcolor::reset << std::endl;

    // processor->getPipeline()->stepMode();

    pre();
    if (opcode == LW && busy) load();
    if (opcode == SW && busy) store();
    post();
    return;
}

void OMemoryUnit::load()
{
    result = processor->DataMemory[address];
    cycle--;
    return;
};

void OMemoryUnit::store()
{
    processor->DataMemory[address] = store_val;
    return;
};

void OMemoryUnit::post()
{
    if (!busy) return;
    if (opcode == LW)
    {
        processor->getCDB()->broadcast(destination, lsqTag, result);
    }
    // processor->getLsq()->removeFromQueue(lsqTag);
    busy = false;
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