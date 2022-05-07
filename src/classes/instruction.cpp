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
};

Register Instruction::getDestination()
{
    if (opcode == SW) return $noreg;
    if (opcode == BEQ || opcode == BGTE || opcode == BL || opcode == BNE) return $pc;
    if (type == IType) return rt;
    if (type == RType) return rd;
    return $noreg;
};