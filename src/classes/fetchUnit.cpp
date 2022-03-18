#include "procUnits.h"

FetchUnit::FetchUnit(Pipeline *pl)
{
    pipeline = pl;
    return;
};

Instructions::Instruction* FetchUnit::fetch(Instructions::Instruction *instrPtr)
{
    std::string instr = processor->instructionMemory[processor->PC];
    std::cout 
    << termcolor::green
    << termcolor::bold
    << "Fetching Instruction: "
    << termcolor::reset 
    << instr
    << std::endl;
    instrPtr->instrString = instr;
    processor->PC++;
    return instrPtr;
};
