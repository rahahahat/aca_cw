#include "procUnit.h"
#include "fetchUnit.h"
#include "processor.h"
#include "pipeline.h"
#include "termcolor.h"

FetchUnit::FetchUnit() {};

void FetchUnit::run(Instructions::Instruction* instrPtr)
{
    fetch(instrPtr);
}

Instructions::Instruction* FetchUnit::fetch(Instructions::Instruction *instrPtr)
{
    if (!processor->getSB()->isPCValid())
    {
        return NULL;
    } 
    std::string instr = processor->instructionMemory[processor->PC];
    std::cout << termcolor::bold << termcolor::green
    << "Fetching Instruction: " << instr << " (" << processor->PC << ")"<< termcolor::reset
    << std::endl;
    instrPtr->fetched_at_pc = processor->PC;
    instrPtr->instrString = instr;
    processor->PC++;
    instrPtr->stage = ISSUE;
    return instrPtr;
};