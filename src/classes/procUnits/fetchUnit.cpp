#include "procUnit.h"
#include "fetchUnit.h"
#include "processor.h"
#include "pipeline.h"

FetchUnit::FetchUnit() {};

void FetchUnit::run(Instructions::Instruction* instrPtr)
{
    std::cout << "Yooooooooooo" << std::endl;
    fetch(instrPtr);
}

Instructions::Instruction* FetchUnit::fetch(Instructions::Instruction *instrPtr)
{
    if (!processor->getSB()->isPCValid()) return NULL;
    std::string instr = processor->instructionMemory[processor->PC];
    std::cout << termcolor::bold << termcolor::green
    << "Issuing Instruction: " << instr << termcolor::reset
    << std::endl;
    // processor->getPipeline()->stepMode();
    instrPtr->instrString = instr;
    processor->PC++;
    instrPtr->stage = ISSUE;
    return instrPtr;
};