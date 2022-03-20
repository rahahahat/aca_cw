#include "procUnit.h"
#include "memoryUnit.h"
#include "processor.h"
#include "pipeline.h"

void MemoryUnit::load(Instructions::Instruction *instrPtr)
{
    instrPtr->temp = processor->DataMemory[instrPtr->temp];
    return;
};

void MemoryUnit::store(Instructions::Instruction *instrPtr)
{
    processor->DataMemory[instrPtr->temp] = processor->registers[instrPtr->rt];
    return;
};

MemoryUnit::MemoryUnit(Pipeline *pl) {};

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