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
    return;
};

void MemoryUnit::writeback(Instructions::Instruction *instrPtr)
{
    if (isInstrBranch(instrPtr))
    {
        processor->PC = instrPtr->immediateOrAddress;
        return;
    }

    switch(instrPtr->type)
    {
        case IType:
            processor->registers[instrPtr->rt] = instrPtr->temp;
            break;
        case RType:
            processor->registers[instrPtr->rd] = instrPtr->temp;
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
        can_run = processor->getLsq()->isOpValid(instrPtr);
        return;
    }
    can_run = true;
    return;
}

void OMemoryUnit::run(Instructions::Instruction *instrPtr)
{
    std::string stage;
    if (instrPtr->stage == MEMORYACCESS) stage = "Access";
    if (instrPtr->stage == WRITEBACK) stage = "Writeback";

    std::cout << termcolor::bold << termcolor::blue
    << "Memory Accessing instructions: " 
    << instrPtr->instrString
    << " (" << stage << ")"
    << termcolor::reset << std::endl;

    pre(instrPtr);

    if (!can_run) return;
    if (instrPtr->opcode == SW)
    {
        store(instrPtr);
    }
    else if (instrPtr->opcode == LW && instrPtr->stage == MEMORYACCESS)
    {
        load(instrPtr);
    } 
    else {
        writeback(instrPtr);
    }

    post(instrPtr);
    return;
}

void OMemoryUnit::post(Instructions::Instruction *instrPtr)
{
    can_run = false;
    if (instrPtr->stage == WRITEBACK)
    {
        if (isInstrBranch(instrPtr)) processor->getPipeline()->resumePipeline();
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
    if (instrPtr->type != JType) processor->getRS()->remove(instrPtr);
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