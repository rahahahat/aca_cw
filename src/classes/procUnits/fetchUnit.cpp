#include "procUnit.h"
#include "fetchUnit.h"
#include "processor.h"
#include "pipeline.h"
#include "termcolor.h"
#include "branch.h"
#include "util.h"

FetchUnit::FetchUnit() {};

void FetchUnit::run(Instructions::Instruction* instrPtr)
{

    fetch(instrPtr);
}

Instructions::Instruction* FetchUnit::fetch(Instructions::Instruction *instrPtr)
{
    bool print = config->debug->print;
    LinkedList<Instructions::Instruction>* instrQ = processor->getPipeline()->getInstrQ();

    if (processor->getPipeline()->stalled()) return NULL;
    if (instrQ->size > config->capacity->instrQ) return NULL;
    if (!processor->getSB()->isPCValid()) return NULL;
    if (processor->PC >= processor->instrMemSize) return NULL;

    std::string instr = processor->instructionMemory[processor->PC];
    IF_PRINT(
        std::cout << termcolor::bold << termcolor::bright_blue
        << "Fetching Instruction: " << termcolor::white << instr << " (" << processor->PC << ")"<< termcolor::reset
        << std::endl;
    );
    instrPtr->fetched_at_pc = processor->PC;
    instrPtr->instrString = instr;
    processor->PC++;
    instrPtr->stage = DECODE;
    instrPtr->pred = processor->PC;

    std::string btb_str = instr + "-" + std::to_string((processor->PC - 1));
    if (processor->getPredictor()->hit(btb_str) != -1)
    {
        int prediction = processor->getPredictor()->predict(btb_str);
        std::string predStr = prediction ? "Branch Taken" : "Branch Not-Taken";
        IF_PRINT(
            std::cout << termcolor::bold << termcolor::bright_blue << "Prediction: " << termcolor::white << predStr << termcolor::reset << std::endl;
        );
        if (prediction != -1) 
        {
            instrPtr->pred = prediction;
            processor->PC = prediction;
        }
    }
    instrQ->add(instrPtr);
    IF_PRINT(
        std::cout << termcolor::bold <<  termcolor::bright_blue << "Instruction Queue Size: " << termcolor::white << instrQ->size << termcolor::reset << std::endl;
    );
    return instrPtr;
};