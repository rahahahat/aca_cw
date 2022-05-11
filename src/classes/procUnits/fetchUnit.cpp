#include "procUnit.h"
#include "fetchUnit.h"
#include "processor.h"
#include "pipeline.h"
#include "termcolor.h"
#include "branch.h"

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
    if (processor->PC >= processor->instrMemSize) return NULL;
    std::string instr = processor->instructionMemory[processor->PC];
    std::cout << termcolor::bold << termcolor::green
    << "Fetching Instruction: " << instr << " (" << processor->PC << ")"<< termcolor::reset
    << std::endl;

    instrPtr->fetched_at_pc = processor->PC;
    instrPtr->instrString = instr;
    processor->PC++;
    instrPtr->stage = DECODE;
    instrPtr->pred = processor->PC;

    std::string btb_str = instr + "-" + std::to_string((processor->PC - 1));
    if (processor->getBTB()->hit(btb_str) != -1)
    {
        int prediction = processor->getPredictor()->predict(btb_str);
        std::cout << "Prediction: " << prediction << std::endl;
        if (prediction != -1) 
        {
            instrPtr->pred = prediction;
            processor->PC = prediction;

        }
    }

    return instrPtr;
};