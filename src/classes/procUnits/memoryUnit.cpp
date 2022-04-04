#include "procUnit.h"
#include "memoryUnit.h"
#include "processor.h"
#include "pipeline.h"

void MemoryUnit::load(Instructions::Instruction *instrPtr)
{
    instrPtr->temp = processor->DataMemory[instrPtr->immediateOrAddress];
    return;
};

void MemoryUnit::store(Instructions::Instruction *instrPtr)
{
    processor->DataMemory[instrPtr->temp] = processor->registers[instrPtr->rt];
    instrPtr->stage = DONE;
    return;
};

void MemoryUnit::writeback(Instructions::Instruction *instrPtr)
{
    switch(instrPtr->type)
    {
        case IType:
            processor->registers[instrPtr->rd] = instrPtr->temp;
            break;
        case RType:
            processor->registers[instrPtr->rt] = instrPtr->temp;
            break;
        case JType:
            processor->PC = instrPtr->immediateOrAddress;
            break;
    }
    return;
}

void OMemoryUnit::pre(Instructions::Instruction *instrPtr)
{
    if (instrPtr->opcode == LW || instrPtr->opcode == SW)
    {
        can_run = lsq->isOpValid(instrPtr);
        return;
    }
    can_run = true;
    return;
}

void OMemoryUnit::run(Instructions::Instruction *instrPtr)
{
    if (!can_run) return;
    if (instrPtr->opcode == SW)
    {
        store(instrPtr);
        return;
    }
    if (instrPtr->opcode == LW && instrPtr->stage == MEMORYACCESS)
    {
        load(instrPtr);
        return;
    }
    writeback(instrPtr);
    return;
}

void OMemoryUnit::post(Instructions::Instruction *instrPtr)
{
    can_run = false;
    if (instrPtr->stage == WRITEBACK)
    {
        if (isInstrBranch(instrPtr)) processor->getPipeline()->resumeFetch();
        instrPtr->stage = DONE;
    }
    if (instrPtr->stage == MEMORYACCESS)
    {
        instrPtr->stage = WRITEBACK;
        if (instrPtr->opcode == SW) 
        {
            instrPtr->stage = DONE;
            processor->getRS()->remove(instrPtr);
            return;
        }
        return;
    }
    if (instrPtr->type == RType) processor->getSB()->validate(instrPtr->rd, instrPtr->temp, instrPtr->tag);
    if (instrPtr->type == IType) processor->getSB()->validate(instrPtr->rt, instrPtr->temp, instrPtr->tag);
    processor->getRS()->remove(instrPtr);
    return;
}

// void MemoryUnit::memref(Instructions::Instruction *instrPtr) 
// {
//     std::cout 
//     << termcolor::green
//     << termcolor::bold
//     << "Memory Accessing Instruction: "
//     << termcolor::reset 
//     << instrPtr->instrString 
//     << std::endl;
//     switch (instrPtr->opcode)
//     {
//     case LW:
//         load();
//     case SW:
//         store();
//         break;
//     case HALT:
//         return;
//     }
// };

// void MemRefUnit::populateResultForwarder(Instructions::Instruction *instrPtr)
// {
//     Opcodes opcode = instrPtr->opcode;
//     if (opcode == LW) {
//         processor->resultForwarder->addValue(instrPtr->rt, instrPtr->temp);
//     }
//     return;
// }