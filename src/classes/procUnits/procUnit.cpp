#include "procUnit.h"
#include "constants.h"
#include "termcolor.h"
#include "processor.h"
#include "pipeline.h"

ProcUnit::ProcUnit()
{
    processor = Processor::getProcessorInstance();
    return;
}

bool ProcUnit::isInstrBranch(Instructions::Instruction *instrPtr)
{
    Opcodes opcode = instrPtr->opcode;
    return (opcode == BEQ || opcode == BGTE || opcode == BNE || opcode == BL); 
}