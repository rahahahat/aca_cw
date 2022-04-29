#include <iostream>
#include <string>
#include <stdexcept>

#include "instruction.h"

using namespace Instructions;

Instruction::Instruction()
{
    rd = $noreg;
    rs = $noreg;
    rt = $noreg;
    instrString = "";
    stage = ISSUE;
    isReadyToExecute = false;
    immediateOrAddress = -1;
    num_cycles = 2;
    curr_cycle = 2;
};

Instruction::Instruction(std::string instr)
{
    rd = $noreg;
    rs = $noreg;
    rt = $noreg;
    instrString = instr;
    stage = ISSUE;
    isReadyToExecute = false;
    immediateOrAddress = -1;
    num_cycles = 2;
    curr_cycle = 2;
};

int Instruction::getCurrCycle()
{
    return curr_cycle;
}

void Instruction::setNumCycle(int cycles)
{
    // if (num_cycles == 0)
    // {
    curr_cycle = cycles;
    num_cycles = cycles;
    // }
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