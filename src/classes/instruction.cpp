#include <iostream>
#include <string>
#include <stdexcept>

#include "instruction.h"
#include "pipeline.h"
#include "pipestage.h"

using namespace Instructions;

Instruction::Instruction()
{
    rd = $noreg;
    rs = $noreg;
    rt = $noreg;
    instrString = "";
    stage = FETCH;
    isReadyToExecute = false;
    immediateOrAddress = -1;
};

Instruction::Instruction(std::string instr)
{
    rd = $noreg;
    rs = $noreg;
    rt = $noreg;
    instrString = instr;
    stage = FETCH;
    isReadyToExecute = false;
    immediateOrAddress = -1;
};

int Instruction::getCurrCycle()
{
    return curr_cycle;
}

void Instruction::nextPipeStage()
{
    switch (stage) {
        case FETCH:
            stage = DECODE;
            break;
        case DECODE:
            stage = EXECUTE;
            break;
        case EXECUTE:
            stage = MEMORYACCESS;
            break;
        case MEMORYACCESS:
            stage = WRITEBACK;
            break;
        case WRITEBACK:
            stage = DONE;
    };
};

void Instruction::setNumCycle(int cycles)
{
    if (num_cycles == 0)
    {
        num_cycles = cycles;
    }
    return;
}

void Instruction::decrementCycle()
{
    if (curr_cycle > 0)
    {
        curr_cycle--;
        return;
    }
    throw std::runtime_error("Instruction execution cycle cannot be below 0");
}

pipestage Instruction::getCurrentPipeStage()
{
    return stage;
};