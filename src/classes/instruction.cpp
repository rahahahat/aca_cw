#include <iostream>
#include <string>
#include <stdexcept>
#include "util.h"
#include "instruction.h"

using namespace Instructions;

Instruction::Instruction()
{
    immediateOrAddress = -1;
    pc_no_pred = -1;
    pred = -1;

    stage = FETCH;
    rs = $noreg;
    rt = $noreg;
    rd = $noreg;
    opcode = NOP;
    type = Nop;

    tag = "~";
    instrString = "";
};

Instruction::Instruction(std::string instr)
{
    immediateOrAddress = -1;
    pc_no_pred = -1;
    pred = -1;

    stage = FETCH;
    rs = $noreg;
    rt = $noreg;
    rd = $noreg;
    opcode = NOP;
    type = Nop;

    tag = "~";
    instrString = instr;
};

Register Instruction::getDestination()
{
    if (opcode == SW) return $noreg;
    if (isOpBranch(opcode)) return $noreg;
    if (type == IType) return rt;
    if (type == RType) return rd;
    return $noreg;
};